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

#include "prolog_server/ServiceServer.h"

namespace prolog { namespace server {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

ServiceServer::ServiceServer() {
}

ServiceServer::ServiceServer(const ServiceServer& src) :
  impl_(src.impl_) {
}

ServiceServer::~ServiceServer() {  
}

ServiceServer::Impl::Impl() {
}

ServiceServer::Impl::~Impl() {
  shutdown();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool ServiceServer::isValid() const {
  if (impl_)
    return impl_->isValid();
  else
    return false;
}

bool ServiceServer::Impl::isValid() const {
  return openQueryServer_ &&
    getAllSolutionsServer_ &&
    getNextSolutionServer_ &&
    hasSolutionServer_ &&
    closeQueryServer_;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void ServiceServer::shutdown() {
  if (impl_)
    impl_->shutdown();
}

void ServiceServer::Impl::shutdown() {
  openQueryServer_.shutdown();
  getAllSolutionsServer_.shutdown();
  getNextSolutionServer_.shutdown();
  hasSolutionServer_.shutdown();
  closeQueryServer_.shutdown();
}

}}
