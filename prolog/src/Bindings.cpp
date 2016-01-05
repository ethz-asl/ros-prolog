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

#include "prolog/Bindings.h"

namespace semantic_map {

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Bindings::Bindings() {
}

Bindings::Bindings(const Bindings& src) :
  values_(src.values_) {
}

Bindings::~Bindings() {  
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

const boost::unordered_map<std::string, Value>& Bindings::getValues() const {
  return values_;
}

const Value& Bindings::getValue(const std::string& name) const {
  boost::unordered_map<std::string, Value>::const_iterator it =
    values_.find(name);
  
  if (it != values_.end())
    return it->second;
  else
    throw UnboundVariableException(name);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Bindings::addValue(const std::string& name, const Value& value) {
  values_.insert(std::make_pair(name, value));
}

boost::unordered_map<std::string, Value>::iterator Bindings::begin() {
  return values_.begin();
}

boost::unordered_map<std::string, Value>::const_iterator Bindings::begin()
    const {
  return values_.begin();
}

boost::unordered_map<std::string, Value>::iterator Bindings::end() {
  return values_.end();
}

boost::unordered_map<std::string, Value>::const_iterator Bindings::end()
    const {
  return values_.end();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

const Value& Bindings::operator[](const std::string& name) const {
  return getValue(name);
}

}

}
