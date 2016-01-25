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

#include "prolog_common/Rule.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Rule::Rule(const std::string& predicate, const std::vector<Term>& arguments,
    const std::list<Term>& goals) {
  impl_.reset(new Impl(predicate, arguments, goals));
}

Rule::Rule(const Rule& src) :
  Clause(src) {
}

Rule::Rule(const Clause& src) :
  Clause(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Rule::~Rule() {  
}

Rule::Impl::Impl(const std::string& predicate, const std::vector<Term>&
    arguments, const std::list<Term>& goals) :
  predicate_(predicate),
  arguments_(arguments),
  goals_(goals) {
  BOOST_ASSERT(!predicate.empty());
  BOOST_ASSERT(!goals.empty());
  
  for (std::vector<Term>::const_iterator it = arguments.begin();
      it != arguments.end(); ++it)
    BOOST_ASSERT(!it->isCompound());
}

Rule::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Rule::getPredicate() const {
  return boost::static_pointer_cast<Impl>(impl_)->predicate_;
}

std::vector<Term> Rule::getArguments() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_;
}

std::list<Term> Rule::getGoals() const {
  return boost::static_pointer_cast<Impl>(impl_)->goals_;
}

size_t Rule::getArity() const {
  return boost::static_pointer_cast<Impl>(impl_)->arguments_.size();
}

size_t Rule::getNumGoals() const {
  return boost::static_pointer_cast<Impl>(impl_)->goals_.size();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Rule::append(const Term& goal) {
  if (impl_)
    boost::static_pointer_cast<Impl>(impl_)->goals_.push_back(goal);
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Rule& Rule::operator&=(const Term& goal) {
  append(goal);
  
  return *this;
}

}
