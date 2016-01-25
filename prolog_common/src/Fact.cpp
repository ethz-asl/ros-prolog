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

#include "prolog_common/Fact.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Fact::Fact(const std::string& predicate, const std::vector<Term>& arguments) {
  impl_.reset(new Impl(predicate, arguments));
}

Fact::Fact(const Fact& src) :
  Clause(src) {
}

Fact::Fact(const Clause& src) :
  Clause(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Fact::~Fact() {  
}

Fact::Impl::Impl(const std::string& predicate, const std::vector<Term>&
    arguments) :
  predicate_(predicate),
  arguments_(arguments) {
  BOOST_ASSERT(!predicate.empty());
  
  for (std::vector<Term>::const_iterator it = arguments.begin();
      it != arguments.end(); ++it)
    BOOST_ASSERT(!it->isCompound());
}

Fact::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Fact::getPredicate() const {
  return boost::static_pointer_cast<Impl>(impl_)->predicate_;
}

std::vector<Term> Fact::getArguments() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_;
}

size_t Fact::getArity() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.size();
}

}
