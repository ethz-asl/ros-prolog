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

#include <prolog_msgs/CloseQuery.h>
#include <prolog_msgs/GetAllSolutions.h>
#include <prolog_msgs/GetNextSolution.h>
#include <prolog_msgs/HasSolution.h>
#include <prolog_msgs/OpenQuery.h>

#include <prolog_common/Bindings.h>

#include <prolog_serialization/JSONDeserializer.h>
#include <prolog_serialization/JSONSerializer.h>

#include <prolog_client/QueryProxy.h>

#include "prolog_client/Query.h"

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Query::InvalidOperation::InvalidOperation(const std::string& description) :
  ros::Exception("Invalid operation: "+description) {
}

Query::NoSuchService::NoSuchService(const std::string& service) :
  ros::Exception("Failure to contact the Prolog server: Prolog service ["+
    service+"] seems not to be advertised.") {
}

Query::ServiceCallFailed::ServiceCallFailed(const std::string& service) :
  ros::Exception("Service call failed for Prolog service ["+service+"].") {
}

Query::InvalidIdentifier::InvalidIdentifier() :
  ros::Exception("Prolog query identifier is empty: "
    "Has this query been opened, yet?")  {
}

Query::InvalidIdentifier::InvalidIdentifier(const std::string& identifier) :
  ros::Exception("Prolog query identifier ["+identifier+"] is invalid: "
    "Another client may have interfered with this query.")  {
}

Query::QueryFailed::QueryFailed(const std::string& description) :
  ros::Exception("Prolog query failed: "+description) {
}

Query::DeserializationFailed::DeserializationFailed(const std::string&
    description) :
  ros::Exception("Failure to deserialize Prolog solution: "+description) {
}

Query::UnknownResponse::UnknownResponse(int status) :
  ros::Exception("Unknown status response from the Prolog server: "+
    boost::lexical_cast<std::string>(status)) {
}

Query::Query() {
}

Query::Query(const std::string& goal) :
  impl_(new Impl()) {
  impl_->query_ = goal;
  impl_->format_ = PrologFormat;
}

Query::Query(const prolog::Query& query) :
  impl_(new Impl()) {
  std::ostringstream stream;  
  serialization::JSONSerializer serializer;
  
  serializer.serializeQuery(stream, query);
  
  impl_->query_ = stream.str();
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
  close();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Query::getIdentifier() const {
  if (impl_.get())
    return impl_->identifier_;
  else
    return std::string();
}

Query::Format Query::getFormat() const {
  if (impl_.get())
    return impl_->format_;
  else
    return PrologFormat;
}

Solution Query::getNextSolution(bool close) const {
  if (impl_.get())
    return impl_->getNextSolution(close);
  else
    return Solution();
}

std::list<Solution> Query::getAllSolutions() const {
  if (impl_.get())
    return impl_->getAllSolutions();
  else
    return std::list<Solution>();
}

QueryProxy Query::getProxy() const {
  QueryProxy proxy;
  
  if (impl_.get())
    proxy.impl_.reset(new QueryProxy::Impl(*this));

  return proxy;
}

bool Query::isValid() const {
  return impl_.get();
}

bool Query::isOpen() const {
  if (impl_.get())
    return !impl_->identifier_.empty();
  else
    return false;
}

bool Query::isEmpty() const {
  if (impl_.get())
    return impl_->query_.empty();
  else
    return true;
}

bool Query::hasSolution() const {
  if (impl_.get())
    return impl_->hasSolution();
  else
    return false;
}

Solution Query::Impl::getNextSolution(bool close) {
  if (identifier_.empty())
    throw InvalidIdentifier();
  
  if (!client_.impl_->getNextSolutionClient_.exists())
    throw NoSuchService(client_.impl_->getNextSolutionClient_.getService());
    
  prolog_msgs::GetNextSolution::Request request;
  prolog_msgs::GetNextSolution::Response response;
  
  request.id = identifier_;
  request.close = close;
  
  if (!client_.impl_->getNextSolutionClient_.call(request, response))
    throw ServiceCallFailed(client_.impl_->getNextSolutionClient_.
      getService());
  
  if (response.status != prolog_msgs::GetNextSolution::Response::STATUS_OK) {
    if (response.status == prolog_msgs::GetNextSolution::Response::
        STATUS_INVALID_ID)
      throw InvalidIdentifier(request.id);
    else if (response.status == prolog_msgs::GetNextSolution::Response::
        STATUS_QUERY_FAILED)
      throw QueryFailed(response.error);
    else if (response.status != prolog_msgs::GetNextSolution::Response::
        STATUS_NO_SOLUTIONS)
      throw UnknownResponse(response.status);
  }

  if (close) {
    identifier_.clear();
    client_ = ServiceClient();
  }
  
  Solution solution;
  
  if (response.status != prolog_msgs::GetNextSolution::Response::
      STATUS_NO_SOLUTIONS) {
    std::istringstream stream(response.solution);
    serialization::JSONDeserializer deserializer;
    
    try {
      solution = deserializer.deserializeBindings(stream);
    }
    catch (const ros::Exception& exception) {
      throw DeserializationFailed(exception.what());
    }
  }
  
  return solution;
}

std::list<Solution> Query::Impl::getAllSolutions() {
  if (identifier_.empty())
    throw InvalidIdentifier();
  
  if (!client_.impl_->getAllSolutionsClient_.exists())
    throw NoSuchService(client_.impl_->getAllSolutionsClient_.getService());
    
  prolog_msgs::GetAllSolutions::Request request;
  prolog_msgs::GetAllSolutions::Response response;
  
  request.id = identifier_;
  
  if (!client_.impl_->getAllSolutionsClient_.call(request, response))
    throw ServiceCallFailed(client_.impl_->getAllSolutionsClient_.
      getService());
  
  if (response.status != prolog_msgs::GetAllSolutions::Response::STATUS_OK) {
    if (response.status == prolog_msgs::GetAllSolutions::Response::
        STATUS_INVALID_ID)
      throw InvalidIdentifier(request.id);
    else if (response.status == prolog_msgs::GetAllSolutions::Response::
        STATUS_QUERY_FAILED)
      throw QueryFailed(response.error);
    else if (response.status != prolog_msgs::GetAllSolutions::Response::
        STATUS_NO_SOLUTIONS)
      throw UnknownResponse(response.status);
  }

  identifier_.clear();
  client_ = ServiceClient();
  
  std::list<Solution> solutions;
  
  if (response.status != prolog_msgs::GetAllSolutions::Response::
      STATUS_NO_SOLUTIONS) {
    for (size_t index = 0; index < response.solutions.size(); ++index) {
      std::istringstream stream(response.solutions[index]);
      serialization::JSONDeserializer deserializer;
    
      Solution solution;
      
      try {
        solution = deserializer.deserializeBindings(stream);
      }
      catch (const ros::Exception& exception) {
        throw DeserializationFailed(exception.what());
      }
      
      solutions.push_back(solution);
    }
  }
  
  return solutions;
}

bool Query::Impl::hasSolution() {
  if (identifier_.empty())
    throw InvalidIdentifier();
  
  if (!client_.impl_->hasSolutionClient_.exists())
    throw NoSuchService(client_.impl_->hasSolutionClient_.getService());
    
  prolog_msgs::HasSolution::Request request;
  prolog_msgs::HasSolution::Response response;
  
  request.id = identifier_;
  
  if (!client_.impl_->hasSolutionClient_.call(request, response))
    throw ServiceCallFailed(client_.impl_->hasSolutionClient_.getService());
  
  if (response.status != prolog_msgs::HasSolution::Response::STATUS_OK) {
    if (response.status == prolog_msgs::HasSolution::Response::
        STATUS_INVALID_ID)
      throw InvalidIdentifier(request.id);
    else if (response.status == prolog_msgs::HasSolution::Response::
        STATUS_QUERY_FAILED)
      throw QueryFailed(response.error);
    else
      throw UnknownResponse(response.status);
  }

  return response.result;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Query::open(ServiceClient& client, Mode mode) {
  if (impl_.get())
    impl_->open(client, mode);
}

void Query::close() {
  if (impl_.get())
    impl_->close();
}

Solution Query::once(ServiceClient& client) {
  Solution solution;
  
  if (impl_.get()) {
    impl_->open(client, IncrementalMode);
    solution = impl_->getNextSolution(true);
  }
  
  return solution;
}

std::list<Solution> Query::all(ServiceClient& client) {
  std::list<Solution> solutions;
  
  if (impl_.get()) {
    impl_->open(client, BatchMode);
    solutions = impl_->getAllSolutions();
  }
  
  return solutions;
}

QueryProxy Query::incremental(ServiceClient& client) {
  QueryProxy proxy;
  
  if (impl_.get()) {
    impl_->open(client, IncrementalMode);
    proxy.impl_.reset(new QueryProxy::Impl(*this));
  }
  
  return proxy;
}

void Query::Impl::open(ServiceClient& client, Mode mode) {
  if (!identifier_.empty())
    throw InvalidOperation("A Prolog client may have already "
      "opened this query.");
  
  if (query_.empty())
    throw InvalidOperation("Attempted to open an empty query.");
  
  if (!client.impl_)
    throw InvalidOperation("Attempted use of an invalid Prolog "
      "service client.");
    
  if (!client.impl_->openQueryClient_.exists())
    throw NoSuchService(client.impl_->openQueryClient_.getService());
    
  prolog_msgs::OpenQuery::Request request;
  prolog_msgs::OpenQuery::Response response;
  
  if (format_ == JSONFormat)
    request.mode = prolog_msgs::OpenQuery::Request::FORMAT_JSON;
  else
    request.mode = prolog_msgs::OpenQuery::Request::FORMAT_PROLOG;
  
  if (mode == IncrementalMode)
    request.mode = prolog_msgs::OpenQuery::Request::MODE_INCREMENTAL;
  else
    prolog_msgs::OpenQuery::Request::MODE_BATCH;
  
  request.query = query_;
  
  if (!client.impl_->openQueryClient_.call(request, response))
    throw ServiceCallFailed(client.impl_->openQueryClient_.getService());
  
  if (!response.ok)
    QueryFailed(response.error);
  
  identifier_ = response.id;
  client_ = client;
}

void Query::Impl::close() {
  if (!identifier_.empty()) {
    if (!client_.impl_->closeQueryClient_.exists())
      throw NoSuchService(client_.impl_->closeQueryClient_.getService());
      
    prolog_msgs::CloseQuery::Request request;
    prolog_msgs::CloseQuery::Response response;
    
    request.id = identifier_;
    
    if (!client_.impl_->closeQueryClient_.call(request, response))
      throw ServiceCallFailed(client_.impl_->closeQueryClient_.getService());
    
    if (response.status != prolog_msgs::CloseQuery::Response::STATUS_OK) {
      if (response.status != prolog_msgs::CloseQuery::Response::
          STATUS_INVALID_ID)
        throw UnknownResponse(response.status);
    }
    
    identifier_.clear();
  }
  
  client_ = ServiceClient();
}

}}
