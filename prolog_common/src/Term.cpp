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

#include <prolog_common/Atom.h>
#include <prolog_common/Compound.h>
#include <prolog_common/Float.h>
#include <prolog_common/Integer.h>
#include <prolog_common/List.h>
#include <prolog_common/Number.h>
#include <prolog_common/Variable.h>

#include "prolog_common/Term.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Term::Term() {
}

Term::Term(const char* name) :
  Term(std::string(name)) {
}

Term::Term(const std::string& name) {
  if (!name.empty() && ((name[0] == toupper(name[0])) || (name[0] == '_')))
    impl_.reset(new Variable::Impl(name));
  else
    impl_.reset(new Atom::Impl(name));
}

Term::Term(int value) :
  Term(static_cast<int64_t>(value)) {
}

Term::Term(int64_t value) :
  impl_(new Integer::Impl(value)) {
}

Term::Term(double value) :
  impl_(new Float::Impl(value)) {
}

Term::Term(const std::list<Term>& elements) :
  impl_(new List::Impl(elements)) {
}

Term::Term(const std::initializer_list<Term>& elements) :
  impl_(new List::Impl(elements)) {
}

Term::Term(const std::string& functor, const std::vector<Term>& arguments) :
  impl_(new Compound::Impl(functor, arguments)) {
}

Term::Term(const std::string& functor, const std::initializer_list<Term>&
    arguments) :
  impl_(new Compound::Impl(functor, arguments)) {
}

Term::Term(const Term& src) :
  impl_(src.impl_) {
}

Term::~Term() {  
}

Term::Impl::Impl() {
}

Term::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Term::isAtom() const {
  return boost::dynamic_pointer_cast<Atom::Impl>(impl_).get();
}

bool Term::isCompound() const {
  return boost::dynamic_pointer_cast<Compound::Impl>(impl_).get();
}

bool Term::isList() const {
  return boost::dynamic_pointer_cast<List::Impl>(impl_).get();
}

bool Term::isNumber() const {
  return boost::dynamic_pointer_cast<Number::Impl>(impl_).get();
}

bool Term::isVariable() const {
  return boost::dynamic_pointer_cast<Variable::Impl>(impl_).get();
}

bool Term::isValid() const {
  return impl_.get();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Compound Term::operator&(const Term& term) const {
  return Compound("','", {*this, term});
}

Compound Term::operator|(const Term& term) const {
  return Compound("';'", {*this, term});
}

}
