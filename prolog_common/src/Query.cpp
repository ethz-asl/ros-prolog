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

#include "prolog_common/Query.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Query::Query() :
  impl_(new Impl()) {
}

Query::Query(const std::string& predicate, const std::vector<Term>&
    arguments) :
  impl_(new Impl("user", predicate, arguments)) {
}

Query::Query(const std::string& predicate, const std::initializer_list<Term>&
    arguments) :
  impl_(new Impl("user", predicate, arguments)) {
}

Query::Query(const std::string& module, const std::string& predicate, const
    std::vector<Term>& arguments) :
  impl_(new Impl(module, predicate, arguments)) {
}

Query::Query(const std::string& module, const std::string& predicate, const
    std::initializer_list<Term>& arguments) :
  impl_(new Impl(module, predicate, arguments)) {
}

Query::Query(const Query& src) :
  impl_(src.impl_) {
}

Query::~Query() {  
}

Query::Impl::Impl(const std::string& module, const std::string& predicate,
    const std::vector<Term>& arguments) :
  module_(module),
  predicate_(predicate),
  arguments_(arguments) {
  BOOST_ASSERT(!predicate.empty());
}

Query::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Query::getModule() const {
  return impl_->module_;
}

std::string Query::getPredicate() const {
  return impl_->predicate_;
}

std::vector<Term> Query::getArguments() const {
  return impl_->arguments_;
}

size_t Query::getArity() const {
  return impl_->arguments_.size();
}

bool Query::isGoal() const {
  for (std::vector<Term>::const_iterator it = impl_->arguments_.begin();
      it != impl_->arguments_.end(); ++it) {
    if (it->isVariable())
      return false;
  }
    
  return true;
}

bool Query::isValid() const {
  return !impl_->predicate_.empty();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

std::vector<Term>::iterator Query::begin() {
  impl_->arguments_.begin();
}

std::vector<Term>::const_iterator Query::begin() const {
  impl_->arguments_.begin();
}

std::vector<Term>::iterator Query::end() {
  impl_->arguments_.end();
}

std::vector<Term>::const_iterator Query::end() const {
  impl_->arguments_.end();
}

}
