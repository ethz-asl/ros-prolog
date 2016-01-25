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

#include "prolog_common/Compound.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Compound::Compound(const std::string& functor, const std::vector<Term>&
    arguments) {
  impl_.reset(new Impl(functor, arguments));
}

Compound::Compound(const Compound& src) :
  Term(src) {
}

Compound::Compound(const Term& src) :
  Term(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Compound::~Compound() {  
}

Compound::Impl::Impl(const std::string& functor, const std::vector<Term>&
    arguments) :
  functor_(functor),
  arguments_(arguments) {
  BOOST_ASSERT(!functor.empty());
  BOOST_ASSERT(!arguments.empty());  
}

Compound::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Compound::getFunctor() const {
  return boost::static_pointer_cast<Impl>(impl_)->functor_;
}

std::vector<Term> Compound::getArguments() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_;
}

size_t Compound::getArity() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.size();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

std::vector<Term>::iterator Compound::begin() {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.begin();
}

std::vector<Term>::const_iterator Compound::begin() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.begin();
}

std::vector<Term>::iterator Compound::end() {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.end();
}

std::vector<Term>::const_iterator Compound::end() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.end();
}

}
