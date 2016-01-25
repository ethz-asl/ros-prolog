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

#include <prolog_common/Fact.h>
#include <prolog_common/Rule.h>

#include "prolog_common/Clause.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Clause::Clause() {
}

Clause::Clause(const std::string& predicate, const std::vector<Term>&
    arguments, const std::list<Term>& goals) {
  if (!goals.empty())
    impl_.reset(new Rule::Impl(predicate, arguments, goals));
  else
    impl_.reset(new Fact::Impl(predicate, arguments));
}

Clause::Clause(const std::string& predicate, const std::initializer_list<
    Term>& arguments, const std::initializer_list<Term>& goals) {
  if (goals.size())
    impl_.reset(new Rule::Impl(predicate, arguments, goals));
  else
    impl_.reset(new Fact::Impl(predicate, arguments));
}

Clause::Clause(const Clause& src) :
  impl_(src.impl_) {
}

Clause::~Clause() {  
}

Clause::Impl::Impl() {
}

Clause::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Clause::isFact() const {
  return boost::dynamic_pointer_cast<Fact::Impl>(impl_).get();
}

bool Clause::isRule() const {
  return boost::dynamic_pointer_cast<Rule::Impl>(impl_).get();
}

bool Clause::isValid() const {
  return impl_.get();
}

}
