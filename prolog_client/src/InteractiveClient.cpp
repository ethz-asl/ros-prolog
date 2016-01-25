/******************************************************************************
 * Copyright (C) 2016 by Ralf Kaestner                                        *
 * ralf.kaestner@gmail.com                                                    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include <iostream>
#include <sstream>

#include <prolog_serialization/PrologSerializer.h>

#include <prolog_client/Query.h>

#include "prolog_client/InteractiveClient.h"

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

InteractiveClient::InteractiveClient() :
  solutionMode_(IncrementalSolutions),
  inputStream_(ioService_, dup(STDIN_FILENO)),
  inputColumn_(0) {
}

InteractiveClient::~InteractiveClient() {
  inputStream_.close();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void InteractiveClient::init() {
  bool connected = false;
  
  std::cout << "Welcome to the ROS interactive Prolog client.\n";
  std::cout << "\n";
  
  serviceClient_ = prologServiceClient("prolog", "/prolog_server");
  connected = serviceClient_.exists();
  
  solutionMode_ = static_cast<SolutionMode>(getParam(ros::names::append(
    "prolog", "solution_mode"), (int)solutionMode_));

  if (!connected) {
    std::cout << "Establishing connection to the Prolog server...\n";
    std::cout << "\n";
    
    connected = serviceClient_.waitForExistence();
    
    std::cout << "\n";
  }
  
  if (connected) {
    std::cout << "Please enter your query after the prompt,\n";
    std::cout << "or type 'halt.' to exit the client.\n";
    std::cout << "\n";
    std::cout << "?- " << std::flush;
  
    startInput();
  }
  else {
    std::cout << "Failure to contact the Prolog server.\n";
    std::cout << "Has the Prolog server been launched?\n";
    
    ros::shutdown();
  }
}

void InteractiveClient::cleanup() {
  ioService_.stop();
  
  query_.close();
}

void InteractiveClient::startInput() {
  ioService_.reset();
  
  boost::asio::async_read_until(inputStream_, inputBuffer_, '\n',
    boost::bind(&InteractiveClient::handleInput, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  
  ioThread_ = boost::thread(boost::bind(&boost::asio::io_service::run,
    &ioService_));
}

void InteractiveClient::handleInput(const boost::system::error_code& error,
    size_t length) {
  if (!error) {
    boost::asio::streambuf::const_buffers_type buffer = inputBuffer_.data();
    
    input_ += std::string(boost::asio::buffers_begin(buffer),
      boost::asio::buffers_begin(buffer)+length-1);
    
    inputBuffer_.consume(length);
    
    if (query_.isOpen()) {
      if (input_ == ".") {
        queryProxy_ = QueryProxy();
        iterator_ = QueryProxy::Iterator();
        
        std::cout << "?- " << std::flush;
      }
      else if (iterator_ != queryProxy_.end()) {
        std::cout << "\x1B[A";
        if (inputColumn_)
          std::cout << "\x1B[" << inputColumn_ << "C";
        std::cout << ";\x1B[K\n";
        
        if (!iterator_->isEmpty()) {
          std::ostringstream stream;
          
          serialization::PrologSerializer serializer;
          serializer.serializeBindings(stream, iterator_->
            getBindings());
          
          size_t pos = stream.str().rfind('\n');
          
          if (pos != std::string::npos)
            inputColumn_ = stream.str().length()-pos-1;
          else
            inputColumn_ = stream.str().length();                
          
          std::cout << stream.str() << std::flush;
        }
        else {
          inputColumn_ = 4;
          std::cout << "true" << std::flush;
        }

        ++iterator_;
        
        if (iterator_ == queryProxy_.end()) {
          queryProxy_ = QueryProxy();
          iterator_ = QueryProxy::Iterator();
          
          std::cout << ".\n?- " << std::flush;
        }
        else
          std::cout << " " << std::flush;
      }
      else {
        queryProxy_ = QueryProxy();
        iterator_ = QueryProxy::Iterator();
        
        std::cout << ".\n?- " << std::flush;
      }
    }
    else if (!input_.empty() && (input_[input_.length()-1] == '.')) {
      if (input_ == "halt.") {
        cleanup();
        ros::shutdown();
        
        return;
      }
      else {
        query_ = Query(std::string(input_.begin(), --input_.end()));
        
        try {
          if (solutionMode_ == FirstSolution) {
            Solution solution = query_.once(serviceClient_);
            
            if (!solution.isEmpty()) {
              serialization::PrologSerializer serializer;
              serializer.serializeBindings(std::cout, solution.getBindings());
            }
            else if (solution.isValid())
              std::cout << "true";
            else
              std::cout << "false";
                        
            std::cout << ".\n?- " << std::flush;
          }
          else if (solutionMode_ == AllSolutions) {
            std::list<Solution> solutions = query_.all(serviceClient_);
            
            if (!solutions.empty()) {
              for (std::list<Solution>::const_iterator it = solutions.begin();
                  it != solutions.end(); ++it) {
                if (it != solutions.begin())
                  std::cout << ";\n";
                
                if (!it->isEmpty()) {
                  serialization::PrologSerializer serializer;
                  serializer.serializeBindings(std::cout, it->getBindings());
                }
                else if (it->isValid())
                  std::cout << "true";
              }
            }
            else
              std::cout << "false";
                        
            std::cout << ".\n?- " << std::flush;
          }
          else {
            queryProxy_ = query_.incremental(serviceClient_);
            
            iterator_ = queryProxy_.begin();
            
            if (iterator_ != queryProxy_.end()) {
              if (!iterator_->isEmpty()) {
                std::ostringstream stream;
                
                serialization::PrologSerializer serializer;
                serializer.serializeBindings(stream, iterator_->
                  getBindings());
                
                size_t pos = stream.str().rfind('\n');
                
                if (pos != std::string::npos)
                  inputColumn_ = stream.str().length()-pos-1;
                else
                  inputColumn_ = stream.str().length();
                
                std::cout << stream.str() << std::flush;
              }
              else {
                inputColumn_ = 4;
                std::cout << "true" << std::flush;
              }

              ++iterator_;
              
              if (iterator_ == queryProxy_.end()) {
                queryProxy_ = QueryProxy();
                iterator_ = QueryProxy::Iterator();
                
                std::cout << ".\n?- " << std::flush;
              }
              else
                std::cout << " " << std::flush;
            }
            else {
              queryProxy_ = QueryProxy();
              iterator_ = QueryProxy::Iterator();
              
              std::cout << "false.\n?- " << std::flush;
            }
          }
        }
        catch (const ros::Exception& exception) {
          query_.close();
          
          queryProxy_ = QueryProxy();
          iterator_ = QueryProxy::Iterator();
          
          std::cout << exception.what() << "\n?- " << std::flush;
        }
      }
    }
    else
      std::cout << "|    " << std::flush;
  
    input_.clear();
    startInput();
  }
}

}}
