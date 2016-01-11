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

#include <prolog_client/Query.h>

#include "prolog_client/InteractiveClient.h"

NODEWRAP_EXPORT_CLASS(prolog_client, prolog::client::InteractiveClient)

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

InteractiveClient::InteractiveClient() :
  input_(io_, dup(STDIN_FILENO)) {
}

InteractiveClient::~InteractiveClient() {
  input_.close();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void InteractiveClient::init() {
  bool connected = false;
  
  std::cout << "Welcome to the ROS interactive Prolog client\n";
  std::cout << "\n";
  
  client_ = prologServiceClient("prolog", "/prolog_server");
  connected = client_.exists();

  if (!connected) {
    std::cout << "Establishing connection to the Prolog server...\n";
    std::cout << "\n";
    
    connected = client_.waitForExistence();
    
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
  io_.stop();
}

void InteractiveClient::startInput() {
  io_.reset();
  
  boost::asio::async_read_until(input_, buffer_, '\n',
    boost::bind(&InteractiveClient::handleInput, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  
  thread_ = boost::thread(boost::bind(&boost::asio::io_service::run, &io_));
}

void InteractiveClient::handleInput(const boost::system::error_code& error,
    size_t length) {
  if (!error) {
    boost::asio::streambuf::const_buffers_type buffer = buffer_.data();
    
    goal_ += std::string(boost::asio::buffers_begin(buffer),
      boost::asio::buffers_begin(buffer)+length-1);
    buffer_.consume(length);
    
    if (!goal_.empty() && (goal_[goal_.length()-1] == '.')) {
      if (goal_ == "halt.") {
        ros::shutdown();
        return;
      }
      else {
        Query query(std::string(goal_.begin(), --goal_.end()),
          Query::PrologFormat);
        goal_.clear();

        if (query.start(client_)) {
        }
        else
          std::cout << "Query failed: " << query.getError() << "\n";
      }
      
      std::cout << "?- " << std::flush;
    }
    else
      std::cout << "|    " << std::flush;
  
    startInput();
  }
}

}}
