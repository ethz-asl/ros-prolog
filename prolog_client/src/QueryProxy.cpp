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

#include <prolog_common/Bindings.h>

#include <prolog_client/Query.h>

#include "prolog_client/QueryProxy.h"

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

QueryProxy::InvalidOperation::InvalidOperation(const std::string&
    description) :
  Exception("Invalid operation: "+description) {
}

QueryProxy::QueryProxy() {
}

QueryProxy::QueryProxy(const QueryProxy& src) :
  impl_(src.impl_) {
}

QueryProxy::~QueryProxy() {  
}

QueryProxy::Iterator::Iterator() {
}

QueryProxy::Iterator::Iterator(const Iterator& src) :
  proxy_(src.proxy_),
  iterator_(src.iterator_) {
}

QueryProxy::Iterator::~Iterator() {  
}

QueryProxy::Impl::Impl(const Query& query) :
  query_(query) {
  if (query_.isOpen()) {
    Solution solution = query.impl_->getNextSolution(false);
    
    if (solution.isValid())
      solutions_.push_back(solution);
  }
  else
    throw InvalidOperation("A query proxy may only be constructed "
      "for an open Prolog query.");
}

QueryProxy::Impl::~Impl() {
  query_.close();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool QueryProxy::hasSolution() const {
  if (impl_)
    return !impl_->solutions_.empty();
  else
    return false;
}

bool QueryProxy::isValid() const {
  return impl_.get();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

QueryProxy::Iterator QueryProxy::begin() {
  Iterator iterator;
  
  if (impl_) {
    iterator.proxy_ = impl_;
    iterator.iterator_ = impl_->solutions_.begin();
  }
  
  return iterator;
}

QueryProxy::Iterator QueryProxy::end() {
  Iterator iterator;
  
  if (impl_) {
    iterator.proxy_ = impl_;
    iterator.iterator_ = impl_->solutions_.end();
  }
  
  return iterator;
}

bool QueryProxy::Iterator::equal(const Iterator& iterator) const {
  if (!iterator.proxy_.get() && proxy_.get() &&
      (iterator_ == proxy_->solutions_.end()))
    return true;
  else if (!proxy_.get() && iterator.proxy_.get() &&
      (iterator.iterator_ == iterator.proxy_->solutions_.end()))
    return true;
  else
    return (!proxy_.get() && !iterator.proxy_.get()) ||
      (iterator_ == iterator.iterator_);
}

void QueryProxy::Iterator::increment() {
  if (!proxy_.get() || (iterator_ == proxy_->solutions_.end()))
    throw InvalidOperation("Attempted to increment an end iterator.");

  std::list<Solution>::iterator it = iterator_;
  
  if (++it != proxy_->solutions_.end()) {
    ++iterator_;
    return;
  }

  if (!proxy_->query_.isOpen()) {
    iterator_ = proxy_->solutions_.end();
    return;
  }

  Solution solution = proxy_->query_.impl_->getNextSolution(false);
  
  if (solution.isValid()) {
    proxy_->solutions_.push_back(solution);
    ++iterator_;
  }
  else
    iterator_ = proxy_->solutions_.end();
}

Solution& QueryProxy::Iterator::dereference() const {
   return *iterator_; 
}

}}
