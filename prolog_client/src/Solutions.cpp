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

#include <prolog_client/Query.h>

#include "prolog_client/Solutions.h"
#include <boost/iterator/iterator_concepts.hpp>

namespace prolog { namespace client {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Solutions::StopIteration::StopIteration() :
  Exception("Attempted increment of a solutions end iterator.") {
}

Solutions::Solutions() {
}

Solutions::Solutions(const Solutions& src) :
  impl_(src.impl_) {
}

Solutions::~Solutions() {  
}

Solutions::Iterator::Iterator() {
}

Solutions::Iterator::Iterator(const Iterator& src) :
  solutions_(src.solutions_),
  iterator_(src.iterator_) {
}

Solutions::Iterator::~Iterator() {  
}

Solutions::Impl::Impl() {
}

Solutions::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Solutions::areEmpty() const {
  if (impl_)
    return impl_->bindings_.empty();
  else
    return false;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

Solutions::Iterator Solutions::begin() {
  Iterator iterator;
  
  if (impl_) {
    iterator.solutions_ = impl_;
    iterator.iterator_ = impl_->bindings_.begin();
  }
  
  return iterator;
}

Solutions::Iterator Solutions::end() {
  Iterator iterator;
  
  if (impl_) {
    iterator.solutions_ = impl_;
    iterator.iterator_ = impl_->bindings_.end();
  }
  
  return iterator;
}

void Solutions::clear() {
  if (impl_)
    impl_->bindings_.clear();
}

bool Solutions::Iterator::equal(const Iterator& iterator) const {
  if (!iterator.solutions_.get() && solutions_.get() &&
      (iterator_ == solutions_->bindings_.end()))
    return true;
  else if (!solutions_.get() && iterator.solutions_.get() &&
      (iterator.iterator_ == iterator.solutions_->bindings_.end()))
    return true;
  else
    return (!solutions_.get() && !iterator.solutions_.get()) ||
      (iterator_ == iterator.iterator_);
}

void Solutions::Iterator::increment() {
  if (!solutions_.get() || (iterator_ == solutions_->bindings_.end()))
    throw StopIteration();

  std::list<Bindings>::iterator it = iterator_;
  
  if (++it != solutions_->bindings_.end()) {
    ++iterator_;
    return;
  }

  if (!solutions_->query_.get() || solutions_->query_->identifier_.empty()) {
    iterator_ = solutions_->bindings_.end();
    return;
  }

  Bindings bindings;
  
  if (solutions_->query_->nextSolution(bindings)) {
    solutions_->bindings_.push_back(bindings);
    ++iterator_;
  }
  else
    iterator_ = solutions_->bindings_.end();
}

Bindings& Solutions::Iterator::dereference() const {
   return *iterator_; 
}

}}
