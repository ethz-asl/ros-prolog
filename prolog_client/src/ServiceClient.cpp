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

#include "prolog_client/ServiceClient.h"

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

ServiceClient::ServiceClient() {
}

ServiceClient::ServiceClient(const ServiceClient& src) :
  impl_(src.impl_) {
}

ServiceClient::~ServiceClient() {  
}

ServiceClient::Impl::Impl() {
}

ServiceClient::Impl::~Impl() {
  shutdown();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool ServiceClient::exists() const {
  if (impl_)
    return impl_->exists();
  else
    return false;
}

bool ServiceClient::isValid() const {
  if (impl_)
    return impl_->isValid();
  else
    return false;
}

bool ServiceClient::Impl::exists() const {
  return openQueryClient_.exists() &&
    getAllSolutionsClient_.exists() &&
    getNextSolutionClient_.exists() &&
    hasSolutionClient_.exists() &&
    closeQueryClient_.exists();
}

bool ServiceClient::Impl::isValid() const {
  return openQueryClient_ &&
    getAllSolutionsClient_ &&
    getNextSolutionClient_ &&
    hasSolutionClient_ &&
    closeQueryClient_;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool ServiceClient::waitForExistence(const ros::Duration& timeout) {
  if (impl_)
    return impl_->openQueryClient_.waitForExistence(timeout) &&
      impl_->getAllSolutionsClient_.waitForExistence(timeout) &&
      impl_->getNextSolutionClient_.waitForExistence(timeout) &&
      impl_->hasSolutionClient_.waitForExistence(timeout) &&
      impl_->closeQueryClient_.waitForExistence(timeout);
  else
    return false;
}

void ServiceClient::shutdown() {
  if (impl_)
    impl_->shutdown();
}

void ServiceClient::Impl::shutdown() {
  openQueryClient_.shutdown();
  getAllSolutionsClient_.shutdown();
  getNextSolutionClient_.shutdown();
  hasSolutionClient_.shutdown();
  closeQueryClient_.shutdown();
}

}}
