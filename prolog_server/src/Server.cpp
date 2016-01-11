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

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "prolog_server/Server.h"

NODEWRAP_EXPORT_CLASS(prolog_server, prolog::server::Server)

namespace prolog { namespace server {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Server::Server() {
}

Server::~Server() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

ServiceServer Server::advertisePrologService(const std::string& name,
    const std::string& defaultServiceNamespace) {
  ServiceServer server;

  server.impl_.reset(new ServiceServer::Impl());
  
  server.impl_->startQueryServer_ = advertiseService(
    ros::names::append(name, "start_query"),
    defaultServiceNamespace.empty() ? std::string("start_query") :
      ros::names::append(defaultServiceNamespace, "start_query"),
    &Server::startQueryCallback);
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
  server.impl_->abortQueryServer_ = advertiseService(
    ros::names::append(name, "abort_query"),
    defaultServiceNamespace.empty() ? std::string("abort_query") :
      ros::names::append(defaultServiceNamespace, "abort_query"),
    &Server::abortQueryCallback);
  
  return server;
}

void Server::init() {
  serviceServer_ = advertisePrologService("prolog");
}

void Server::cleanup() {
}

bool Server::startQueryCallback(prolog_msgs::StartQuery::Request& request,
    prolog_msgs::StartQuery::Response& response) {
  return false;
}

bool Server::hasSolutionCallback(prolog_msgs::HasSolution::Request& request,
    prolog_msgs::HasSolution::Response& response) {
  return false;
}

bool Server::getAllSolutionsCallback(prolog_msgs::GetAllSolutions::Request&
    request, prolog_msgs::GetAllSolutions::Response& response) {
  return false;
}

bool Server::getNextSolutionCallback(prolog_msgs::GetNextSolution::Request&
    request, prolog_msgs::GetNextSolution::Response& response) {
  return false;
}

bool Server::abortQueryCallback(prolog_msgs::AbortQuery::Request& request,
    prolog_msgs::AbortQuery::Response& response) {
  return false;
}

std::string Server::generateQueryIdentifier() {  
  return boost::lexical_cast<std::string>(queryIdentifierGenerator_());

}

}}
