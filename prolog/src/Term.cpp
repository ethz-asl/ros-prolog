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

#include <prolog/Exceptions.h>

#include "prolog/Term.h"

namespace semantic_map {

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Term::Term(const std::string& name, const std::vector<Value>& values) :
  name_(name),
  values_(values_) {
}

Term::Term(const Term& src) :
  name_(src.name_),
  values_(src.values_) {
}

Term::~Term() {  
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

const std::string& Term::getName() const {
  return name_;
}

const std::vector<Value>& Term::getValues() const {
  return values_;
}

size_t Term::getArity() const {
  return values_.size();
}

const Value& Term::getValue(size_t index) const {
  if (index < values_.size())
    return values_[index];
  else
    throw InvalidValueIndexException(index);
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

const Value& Term::operator[](size_t index) const {
  return getValue(index);
}

}

}
