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

#include <sstream>

#include <boost/lexical_cast.hpp>

#include <prolog_msgs/AbortQuery.h>
#include <prolog_msgs/GetAllSolutions.h>
#include <prolog_msgs/GetNextSolution.h>
#include <prolog_msgs/HasSolution.h>
#include <prolog_msgs/StartQuery.h>

#include <prolog_serialization/JSONDeserializer.h>
#include <prolog_serialization/JSONSerializer.h>

#include "prolog_client/Query.h"
#include "prolog_client/Solutions.h"

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Query::Query() {
}

Query::Query(const std::string& goal, Format format) :
  impl_(new Impl()) {
  impl_->goal_ = goal;
  impl_->format_ = format;
}

Query::Query(const Term& goal) :
  impl_(new Impl()) {
  std::ostringstream stream;  
  serialization::JSONSerializer serializer;
  
  serializer.serializeTerm(stream, goal);
  
  impl_->goal_ = stream.str();
  impl_->format_ = JSONFormat;
}

Query::Query(const Query& src) :
  impl_(src.impl_) {
}

Query::~Query() {  
}

Query::Impl::Impl() :
  mode_(BatchMode),
  format_(PrologFormat) {
}

Query::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Query::getIdentifier() const {
  if (impl_)
    return impl_->identifier_;
  else
    return std::string();
}

std::string Query::getGoal() const {
  if (impl_)
    return impl_->goal_;
  else
    return std::string();
}

std::string Query::getError() const {
  if (impl_)
    return impl_->error_;
  else
    return std::string();
}

Query::Format Query::getFormat() const {
  if (impl_)
    return impl_->format_;
  else
    return PrologFormat;
}

Solutions Query::getAllSolutions() const {
  Solutions solutions;
  
  if (impl_) {
    solutions.impl_.reset(new Solutions::Impl());

    impl_->allSolutions(solutions.impl_->bindings_);
  }

  return solutions;
}

Solutions Query::getIncrementalSolutions() const {
  Solutions solutions;
  
  if (impl_) {
    solutions.impl_.reset(new Solutions::Impl());
    solutions.impl_->query_ = impl_;
  }

  return solutions;
}

bool Query::isValid() const {
  return impl_.get();
}

bool Query::isEmpty() const {
  if (impl_)
    return impl_->goal_.empty();
  else
    return true;
}

bool Query::hasSolution() const {
  if (impl_)
    return impl_->hasSolution();
  else
    return false;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool Query::start(ServiceClient& client, Mode mode) {
  if (impl_)
    return impl_->start(client, mode);
  else
    return false;
}

bool Query::abort() {
  if (impl_)
    return impl_->abort();
  else
    return false;
}

bool Query::Impl::start(ServiceClient& client, Mode mode) {
  error_.clear();
  
  if (!identifier_.empty()) {
    error_ = "Query conflict: Another client may have started this query.";
    return false;
  }
  
  if (goal_.empty()) {
    error_ = "Query is empty.";
    return false;
  }
  
  if (!client.impl_ || !client.impl_->startQueryClient_.exists()) {
    error_ = "Failure to contact the Prolog server.";
    return false;
  }
    
  prolog_msgs::StartQuery::Request request;
  prolog_msgs::StartQuery::Response response;
  
  if (format_ == JSONFormat)
    request.mode = prolog_msgs::StartQuery::Request::FORMAT_JSON;
  else
    request.mode = prolog_msgs::StartQuery::Request::FORMAT_PROLOG;
  
  if (mode == IncrementalMode)
    request.mode = prolog_msgs::StartQuery::Request::MODE_INCREMENTAL;
  else
    prolog_msgs::StartQuery::Request::MODE_BATCH;
  
  request.goal = goal_;
  
  if (!client.impl_->startQueryClient_.call(request, response)) {
    error_ = "Service call failed.";
    return false;
  }
  
  if (!response.ok) {
    error_ = response.error;
    return false;
  }
  
  identifier_ = response.id;
  client_ = client;
  
  return true;
}

bool Query::Impl::hasSolution() {
  error_.clear();
  
  if (identifier_.empty()) {
    error_ = "Query identifier is invalid: Has this query been started, yet?";
    return false;
  }
  
  if (!client_.impl_ || !client_.impl_->hasSolutionClient_.exists()) {
    error_ = "Failure to contact the Prolog server.";
    return false;
  }
    
  prolog_msgs::HasSolution::Request request;
  prolog_msgs::HasSolution::Response response;
  
  request.id = identifier_;
  
  if (!client_.impl_->hasSolutionClient_.call(request, response)) {
    error_ = "Service call failed.";
    return false;
  }
  
  if (response.status != prolog_msgs::HasSolution::Response::STATUS_OK) {
    if (response.status == prolog_msgs::HasSolution::Response::
        STATUS_INVALID_ID)
      error_ = "Query identifier is invalid: "
        "Another client may have aborted this query.";
    else if (response.status == prolog_msgs::HasSolution::Response::
        STATUS_COMMAND_FAILED)
      error_ = "Command failed: "+response.error;
    else
      error_ = "Unknown status response: "+boost::lexical_cast<std::string>(
        response.status);
      
    return false;
  }

  return response.result;
}

bool Query::Impl::nextSolution(Bindings& bindings) {
  error_.clear();
  
  if (identifier_.empty()) {
    error_ = "Query identifier is invalid: Has this query been started, yet?";
    return false;
  }
  
  if (!client_.impl_ || !client_.impl_->getNextSolutionClient_.exists()) {
    error_ = "Failure to contact the Prolog server.";
    return false;
  }
    
  prolog_msgs::GetNextSolution::Request request;
  prolog_msgs::GetNextSolution::Response response;
  
  request.id = identifier_;
  
  if (!client_.impl_->getNextSolutionClient_.call(request, response)) {
    error_ = "Service call failed.";
    return false;
  }
  
  if (response.status != prolog_msgs::GetNextSolution::Response::STATUS_OK) {
    if (response.status == prolog_msgs::GetNextSolution::Response::
        STATUS_INVALID_ID)
      error_ = "Query identifier is invalid: "
        "Another client may have aborted this query.";
    else if (response.status == prolog_msgs::GetNextSolution::Response::
        STATUS_NO_SOLUTIONS) {
      error_ = "Query has no more solutions.";
      
      identifier_.clear();  
      client_ = ServiceClient();
    }
    else if (response.status == prolog_msgs::GetNextSolution::Response::
        STATUS_COMMAND_FAILED)
      error_ = "Command failed: "+response.error;
    else
      error_ = "Unknown status response: "+boost::lexical_cast<std::string>(
        response.status);
      
    return false;
  }

  std::istringstream stream(response.solution);
  serialization::JSONDeserializer deserializer;
  
  bindings.clear();
  
  try {
    bindings = deserializer.deserializeBindings(stream);
  }
  catch (const ros::Exception& exception) {
    error_ = std::string("Failure to deserialize solution: ")+
      exception.what();
    return false;
  }
  
  return true;
}

bool Query::Impl::allSolutions(std::list<Bindings>& bindings) {
  error_.clear();
  
  if (identifier_.empty()) {
    error_ = "Query identifier is invalid: Has this query been started, yet?";
    return false;
  }
  
  if (!client_.impl_ || !client_.impl_->getAllSolutionsClient_.exists()) {
    error_ = "Failure to contact the Prolog server.";
    return false;
  }
    
  prolog_msgs::GetAllSolutions::Request request;
  prolog_msgs::GetAllSolutions::Response response;
  
  request.id = identifier_;
  
  if (!client_.impl_->getAllSolutionsClient_.call(request, response)) {
    error_ = "Service call failed.";
    return false;
  }
  
  if (response.status != prolog_msgs::GetAllSolutions::Response::STATUS_OK) {
    if (response.status == prolog_msgs::GetAllSolutions::Response::
        STATUS_INVALID_ID)
      error_ = "Query identifier is invalid: "
        "Another client may have aborted this query.";
    else if (response.status == prolog_msgs::GetAllSolutions::Response::
        STATUS_NO_SOLUTIONS) {
      error_ = "Query has no more solutions.";
      
      identifier_.clear();  
      client_ = ServiceClient();
    }
    else if (response.status == prolog_msgs::GetAllSolutions::Response::
        STATUS_COMMAND_FAILED)
      error_ = "Command failed: "+response.error;
    else
      error_ = "Unknown status response: "+boost::lexical_cast<std::string>(
        response.status);
      
    return false;
  }

  bindings.clear();
  
  for (size_t index = 0; index < response.solutions.size(); ++index) {
    std::istringstream stream(response.solutions[index]);
    serialization::JSONDeserializer deserializer;
  
    Bindings solution;
    
    try {
      solution = deserializer.deserializeBindings(stream);
    }
    catch (const ros::Exception& exception) {
      error_ = std::string("Failure to deserialize solution: ")+
        exception.what();
      return false;
    }
    
    bindings.push_back(solution);
  }
  
  return true;
}

bool Query::Impl::abort() {
  error_.clear();
  
  if (identifier_.empty()) {
    error_ = "Query identifier is invalid: Has this query been started, yet?";
    return false;
  }
  
  if (!client_.impl_ || !client_.impl_->abortQueryClient_.exists()) {
    error_ = "Failure to contact the Prolog server.";
    return false;
  }
    
  prolog_msgs::AbortQuery::Request request;
  prolog_msgs::AbortQuery::Response response;
  
  request.id = identifier_;
  
  if (!client_.impl_->abortQueryClient_.call(request, response)) {
    error_ = "Service call failed.";
    return false;
  }
  
  if (response.status != prolog_msgs::AbortQuery::Response::STATUS_OK) {
    if (response.status == prolog_msgs::AbortQuery::Response::
        STATUS_INVALID_ID)
      error_ = "Query identifier is invalid: "
        "Another client may have aborted this query.";
    else
      error_ = "Unknown status response: "+boost::lexical_cast<std::string>(
        response.status);
      
    return false;
  }
  
  identifier_.clear();
  client_ = ServiceClient();
  
  return true;
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Bindings Query::operator()(ServiceClient& client) {
  Bindings bindings;
  
  if (impl_.get() && impl_->start(client, IncrementalMode) &&
      impl_->nextSolution(bindings))
    impl_->abort();
  
  return bindings;
}

}}
