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

#include <algorithm>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "prolog_server/Server.h"

namespace prolog { namespace server {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Server::Server() {
}

Server::~Server() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Server::isPrologInitialized() const {
  return context_.isInitialized();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

ServiceServer Server::advertisePrologService(const std::string& name,
    const std::string& defaultServiceNamespace) {
  ServiceServer server;

  server.impl_.reset(new ServiceServer::Impl());
  
  server.impl_->openQueryServer_ = advertiseService(
    ros::names::append(name, "open_query"),
    defaultServiceNamespace.empty() ? std::string("open_query") :
      ros::names::append(defaultServiceNamespace, "open_query"),
    &Server::openQueryCallback);
  server.impl_->hasSolutionServer_ = advertiseService(
    ros::names::append(name, "has_solution"),
    defaultServiceNamespace.empty() ? std::string("has_solution") :
      ros::names::append(defaultServiceNamespace, "has_solution"),
    &Server::hasSolutionCallback);
  server.impl_->getAllSolutionsServer_ = advertiseService(
    ros::names::append(name, "get_all_solutions"),
    defaultServiceNamespace.empty() ? std::string("get_all_solutions") :
      ros::names::append(defaultServiceNamespace, "get_all_solutions"),
    &Server::getAllSolutionsCallback);
  server.impl_->getNextSolutionServer_ = advertiseService(
    ros::names::append(name, "get_next_solution"),
    defaultServiceNamespace.empty() ? std::string("get_next_solution") :
      ros::names::append(defaultServiceNamespace, "get_next_solution"),
    &Server::getNextSolutionCallback);
  server.impl_->closeQueryServer_ = advertiseService(
    ros::names::append(name, "close_query"),
    defaultServiceNamespace.empty() ? std::string("close_query") :
      ros::names::append(defaultServiceNamespace, "close_query"),
    &Server::closeQueryCallback);
  
  return server;
}

swi::Engine Server::createPrologEngine(const std::string& name, size_t
    defaultGlobalStack, size_t defaultLocalStack, size_t defaultTrailStack) {
  std::string ns = ros::names::append(ros::names::append("prolog",
    "engines"), name);
    
  size_t globalStack = getParam(ros::names::append(ns, "global_stack"),
    (int)defaultGlobalStack);
  size_t localStack = getParam(ros::names::append(ns, "local_stack"),
    (int)defaultLocalStack);
  size_t trailStack = getParam(ros::names::append(ns, "trail_stack"),
    (int)defaultTrailStack);

  return context_.createEngine(name, globalStack, localStack, trailStack);
}

std::string Server::prologQueryIdentifier() {  
  std::string uuid = boost::lexical_cast<std::string>(
    queryIdentifierGenerator_());
  std::replace(uuid.begin(), uuid.end(), '-', '_');
  
  return uuid;
}

void Server::init() {
  initProlog();
}

void Server::initProlog() {
  std::string ns = "prolog";
    
  std::string executable = getParam(ros::names::append(ns, "executable"),
    context_.getExecutable());
  size_t globalStack = getParam(ros::names::append(ns, "global_stack"),
    (int)context_.getGlobalStack());
  size_t localStack = getParam(ros::names::append(ns, "local_stack"),
    (int)context_.getLocalStack());
  size_t trailStack = getParam(ros::names::append(ns, "trail_stack"),
    (int)context_.getTrailStack());
  
  context_.setExecutable(executable);
  context_.setGlobalStack(globalStack);
  context_.setLocalStack(localStack);
  context_.setTrailStack(trailStack);
    
  if (context_.init())
    NODEWRAP_INFO_STREAM("Prolog context has been initialized, "
      "reporting version [" << context_.getVersion() << "].");
  else
    NODEWRAP_ERROR_STREAM("Failure to initialize Prolog.");
}

void Server::cleanup() {
  cleanupProlog();
}

void Server::cleanupProlog() {
  if (context_.cleanup())
    NODEWRAP_INFO_STREAM("Prolog context has been cleaned up.");
}

}}
