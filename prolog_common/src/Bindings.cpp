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

#include "prolog_common/Bindings.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Bindings::Bindings() :
  impl_(new Impl()) {
}

Bindings::Bindings(const Bindings& src) :
  impl_(src.impl_) {
}

Bindings::~Bindings() {  
}

Bindings::Impl::Impl() {
}

Bindings::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

Term Bindings::getTerm(const std::string& name) const {
  boost::unordered_map<std::string, Term>::const_iterator it =
    impl_->terms_.find(name);
  
  if (it != impl_->terms_.end())
    return it->second;
  else
    return Term();
}

bool Bindings::contain(const std::string& name) const {
  return (impl_->terms_.find(name) != impl_->terms_.end());
}

bool Bindings::areEmpty() const {
  return impl_->terms_.empty();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

boost::unordered_map<std::string, Term>::iterator Bindings::begin() {
  return impl_->terms_.begin();
}

boost::unordered_map<std::string, Term>::const_iterator Bindings::begin()
    const {
  return impl_->terms_.begin();
}

boost::unordered_map<std::string, Term>::iterator Bindings::end() {
  return impl_->terms_.end();
}

boost::unordered_map<std::string, Term>::const_iterator Bindings::end()
    const {
  return impl_->terms_.end();
}

void Bindings::addTerm(const std::string& name, const Term& term) {
  impl_->terms_.insert(std::make_pair(name, term));
}

void Bindings::clear() {
  impl_->terms_.clear();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Term Bindings::operator[](const std::string& name) const {
  return getTerm(name);
}

}
