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

#include "prolog_common/Variable.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Variable::Variable(const char* name) :
  Variable(std::string(name)) {
}

Variable::Variable(const std::string& name) {
  impl_.reset(new Impl(name));
}

Variable::Variable(const Variable& src) :
  Term(src) {
}

Variable::Variable(const Term& src) :
  Term(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Variable::~Variable() {  
}

Variable::Impl::Impl(const std::string& name) :
  name_(name) {
  BOOST_ASSERT(!name.empty());
  BOOST_ASSERT((name[0] == toupper(name[0])) ||
    ((name[0] == '_') && (name.length() > 1)));
}

Variable::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Variable::getName() const {
  return boost::static_pointer_cast<Impl>(impl_)->name_;
}

bool Variable::isAnonymous() const {
  return (boost::static_pointer_cast<Impl>(impl_)->name_ == "_");
}

}
