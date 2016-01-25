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

#include <list>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <prolog_common/Bindings.h>

#include <prolog_serialization/JSONDeserializer.h>
#include <prolog_serialization/JSONSerializer.h>

#include "prolog_server/MultiThreadedServer.h"

NODEWRAP_EXPORT_CLASS(prolog_server, prolog::server::MultiThreadedServer)

namespace prolog { namespace server {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

MultiThreadedServer::MultiThreadedServer() {
}

MultiThreadedServer::~MultiThreadedServer() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void MultiThreadedServer::init() {
  Server::init();
    
  if (isPrologInitialized()) {
    serviceServer_ = advertisePrologService("prolog");
    
    size_t numEngines = getParam(ros::names::append("prolog",
      "num_engines"), 4);
    
    for (size_t index = 0; index < numEngines; ++index) {
      engines_.push_back(createPrologEngine("pooled_engine_"+
        boost::lexical_cast<std::string>(index)));
    }
  }
}

void MultiThreadedServer::cleanup() {
  serviceServer_.shutdown();
  engines_.clear();

  workers_.clear();
  queries_.clear();
  
  Server::cleanup();
}

bool MultiThreadedServer::openQueryCallback(prolog_msgs::OpenQuery::Request&
    request, prolog_msgs::OpenQuery::Response& response) {
  if (request.query.empty()) {
    response.ok = false;
    response.error = "Query is empty.";
    
    NODEWRAP_ERROR_STREAM(response.error);
      
    return true;
  }
  
  if (engines_.empty()) {
    response.ok = false;
    response.error = "No Prolog engine available, pool exhausted.";
      
    NODEWRAP_ERROR_STREAM(response.error);
      
    return true;
  }
  
  ThreadedQuery query;
  std::string queryIdentifier = prologQueryIdentifier();  
  ThreadedQuery::Mode queryMode = ThreadedQuery::BatchMode;
  
  if (request.mode == prolog_msgs::OpenQuery::Request::MODE_INCREMENTAL)
    queryMode = ThreadedQuery::IncrementalMode;
  
  if (request.format == prolog_msgs::OpenQuery::Request::FORMAT_JSON) {
    std::istringstream stream(request.query);
    serialization::JSONDeserializer deserializer;
    
    try {
      query.impl_.reset(new ThreadedQuery::Impl(deserializer.
        deserializeQuery(stream), engines_.front(), queryMode));
    }
    catch (const ros::Exception& exception) {      
      response.ok = false;
      response.error = std::string("Failure to create query: ")+
        exception.what();
        
      NODEWRAP_ERROR_STREAM(response.error);
        
      return true;
    }
  }
  else {
    try {
      query.impl_.reset(new ThreadedQuery::Impl(request.query,
        engines_.front(), queryMode));
    }
    catch (const ros::Exception& exception) {
      response.ok = false;
      response.error = std::string("Failure to create query: ")+
        exception.what();
        
      NODEWRAP_ERROR_STREAM(response.error);
      
      return true;
    }
  }
    
  nodewrap::Worker worker;
  nodewrap::WorkerOptions workerOptions;
  
  workerOptions.frequency = 0.0;
  workerOptions.callback = boost::bind(&ThreadedQuery::Impl::execute,
    query.impl_, _1);
  workerOptions.autostart = true;
  workerOptions.synchronous = false;
  workerOptions.privateCallbackQueue = true;
  
  try {
    worker = addWorker("query_"+queryIdentifier, workerOptions);
  }
  catch (const ros::Exception& exception) {
    response.ok = false;
    response.error = std::string("Failure to create worker: ")+
      exception.what();
    
    return true;
  }
  
  engines_.pop_front();
  queries_.insert(std::make_pair(queryIdentifier, query));
  workers_.insert(std::make_pair(queryIdentifier, worker));
  
  NODEWRAP_INFO_STREAM("Prolog query [" << queryIdentifier <<
    "] has been opened.");
  
  response.ok = true;
  response.id = queryIdentifier;
  
  return true;
}

bool MultiThreadedServer::hasSolutionCallback(prolog_msgs::HasSolution::
    Request& request, prolog_msgs::HasSolution::Response& response) {
  boost::unordered_map<std::string, ThreadedQuery>::iterator
    it = queries_.find(request.id);
  
  if (it == queries_.end()) {
    response.status = prolog_msgs::GetNextSolution::Response::
      STATUS_INVALID_ID;
    
    return true;
  }
  
  std::string error;
  
  response.result = it->second.hasSolution(error, true);
  
  if (!response.result && !error.empty()) {
    response.status = prolog_msgs::GetNextSolution::Response::
      STATUS_QUERY_FAILED;
    response.error = error;
    
    return true;
  }
  
  response.status = prolog_msgs::GetNextSolution::Response::STATUS_OK;
  
  return true;
}

bool MultiThreadedServer::getAllSolutionsCallback(prolog_msgs::
    GetAllSolutions::Request& request, prolog_msgs::GetAllSolutions::
    Response& response) {
  boost::unordered_map<std::string, ThreadedQuery>::iterator
    it = queries_.find(request.id);
  
  if (it == queries_.end()) {
    response.status = prolog_msgs::GetNextSolution::Response::
      STATUS_INVALID_ID;
    
    return true;
  }
  
  std::list<Bindings> solutions;
  Bindings bindings;
  std::string error;
  
  while (it->second.getNextSolution(bindings, error, true))
    solutions.push_back(bindings);
  
  boost::unordered_map<std::string, nodewrap::Worker>::iterator
    jt = workers_.find(request.id);
    
  if (jt != workers_.end()) {
    jt->second.cancel(true);
    workers_.erase(jt);
  }
  
  engines_.push_back(it->second.impl_->engine_);
  queries_.erase(it);
  
  NODEWRAP_INFO_STREAM("Prolog query [" << request.id <<
    "] has been closed.");
  
  if (!error.empty()) {
    response.status = prolog_msgs::GetNextSolution::Response::
      STATUS_QUERY_FAILED;
    response.error = error;
    
    return true;
  }
  
  if (solutions.empty()) {
    response.status = prolog_msgs::GetNextSolution::Response::
      STATUS_NO_SOLUTIONS;
      
    return true;
  }
    
  serialization::JSONSerializer serializer;

  for (std::list<Bindings>::const_iterator it = solutions.begin();
      it != solutions.end(); ++it) {
    std::ostringstream stream;
  
    serializer.serializeBindings(stream, *it);
  
    response.solutions.push_back(stream.str());
  }

  response.status = prolog_msgs::GetNextSolution::Response::STATUS_OK;
  
  return true;
}

bool MultiThreadedServer::getNextSolutionCallback(prolog_msgs::
    GetNextSolution::Request& request, prolog_msgs::GetNextSolution::
    Response& response) {
  boost::unordered_map<std::string, ThreadedQuery>::iterator
    it = queries_.find(request.id);
  
  if (it == queries_.end()) {
    response.status = prolog_msgs::GetNextSolution::Response::
      STATUS_INVALID_ID;
    
    return true;
  }
  
  Bindings bindings;
  std::string error;
  
  if (!it->second.getNextSolution(bindings, error, true)) {
    if (!error.empty()) {
      response.status = prolog_msgs::GetNextSolution::Response::
        STATUS_QUERY_FAILED;
      response.error = error;
    }
    else
      response.status = prolog_msgs::GetNextSolution::Response::
        STATUS_NO_SOLUTIONS;
    
    return true;
  }
  
  if (request.close) {
    boost::unordered_map<std::string, nodewrap::Worker>::iterator
      jt = workers_.find(request.id);
      
    if (jt != workers_.end()) {
      jt->second.cancel(true);
      workers_.erase(jt);
    }
    
    engines_.push_back(it->second.impl_->engine_);
    queries_.erase(it);
    
    NODEWRAP_INFO_STREAM("Prolog query [" << request.id <<
      "] has been closed.");
  }
  
  std::ostringstream stream;
  serialization::JSONSerializer serializer;
  
  serializer.serializeBindings(stream, bindings);

  response.solution = stream.str();    
  response.status = prolog_msgs::GetNextSolution::Response::STATUS_OK;
  
  return true;
}

bool MultiThreadedServer::closeQueryCallback(prolog_msgs::CloseQuery::
    Request& request, prolog_msgs::CloseQuery::Response& response) {
  boost::unordered_map<std::string, ThreadedQuery>::iterator
    it = queries_.find(request.id);
  
  if (it == queries_.end()) {
    response.status = prolog_msgs::CloseQuery::Response::STATUS_INVALID_ID;
    
    return true;
  }
  
  boost::unordered_map<std::string, nodewrap::Worker>::iterator
    jt = workers_.find(request.id);
  
  if (jt != workers_.end()) {
    jt->second.cancel(true);
    workers_.erase(jt);
  }
  
  engines_.push_back(it->second.impl_->engine_);
  queries_.erase(it);
  
  NODEWRAP_INFO_STREAM("Prolog query [" << request.id << 
    "] has been closed.");
  
  response.status = prolog_msgs::CloseQuery::Response::STATUS_OK;
  
  return true;
}

}}
