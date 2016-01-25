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

#include <SWI-Prolog.h>

#include "prolog_swi/Bindings.h"

namespace prolog { namespace swi {

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

bool Bindings::areEmpty() const {
  return impl_->terms_.empty();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Bindings::addTerm(const std::string& name, const Term& term) {
  impl_->terms_.insert(std::make_pair(name, term));
}

void Bindings::clear() {
  impl_->terms_.clear();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Bindings::operator prolog::Bindings() const {
  return impl_->operator prolog::Bindings();
}

Bindings::Impl::operator prolog::Bindings() const {
  prolog::Bindings bindings;
  
  for (boost::unordered_map<std::string, Term>::const_iterator
      it = terms_.begin(); it != terms_.end(); ++it)
    bindings.addTerm(it->first, Term(it->second));
  
  return bindings;
}

}}
