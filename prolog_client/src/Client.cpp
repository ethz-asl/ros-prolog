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

#include <prolog_msgs/CloseQuery.h>
#include <prolog_msgs/GetAllSolutions.h>
#include <prolog_msgs/GetNextSolution.h>
#include <prolog_msgs/HasSolution.h>
#include <prolog_msgs/OpenQuery.h>

#include "prolog_client/Client.h"

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Client::Client() {
}

Client::~Client() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

ServiceClient Client::prologServiceClient(const std::string& name, const
    std::string& defaultServiceNamespace, bool defaultPersistent) {
  ServiceClient client;

  client.impl_.reset(new ServiceClient::Impl());
  
  client.impl_->openQueryClient_ = serviceClient<prolog_msgs::
    OpenQuery>(ros::names::append(name, "open_query"),
    defaultServiceNamespace.empty() ? std::string("open_query") :
      ros::names::append(defaultServiceNamespace, "open_query"),
    defaultPersistent);
  client.impl_->hasSolutionClient_ = serviceClient<prolog_msgs::
    HasSolution>(ros::names::append(name, "has_solution"),
    defaultServiceNamespace.empty() ? std::string("has_solution") :
      ros::names::append(defaultServiceNamespace, "has_solution"),
    defaultPersistent);
  client.impl_->getAllSolutionsClient_ = serviceClient<prolog_msgs::
    GetAllSolutions>(ros::names::append(name, "get_all_solutions"),
    defaultServiceNamespace.empty() ? std::string("get_all_solutions") :
      ros::names::append(defaultServiceNamespace, "get_all_solutions"),
    defaultPersistent);
  client.impl_->getNextSolutionClient_ = serviceClient<prolog_msgs::
    GetNextSolution>(ros::names::append(name, "get_next_solution"),
    defaultServiceNamespace.empty() ? std::string("get_next_solution") :
      ros::names::append(defaultServiceNamespace, "get_next_solution"),
    defaultPersistent);
  client.impl_->closeQueryClient_ = serviceClient<prolog_msgs::
    CloseQuery>(ros::names::append(name, "close_query"),
    defaultServiceNamespace.empty() ? std::string("close_query") :
      ros::names::append(defaultServiceNamespace, "close_query"),
    defaultPersistent);
  
  return client;
}

}}
