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

#include <algorithm>
#include <iterator>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <prolog/Term.h>

#include "prolog/Value.h"

namespace semantic_map {

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Value::Value() :
  type_(Empty) {
}

Value::Value(double value) :
  type_(Double),
  value_(value) {
}
        
Value::Value(int value) :
  type_(Integer),
  value_(static_cast<int64_t>(value)) {
}
  
Value::Value(int64_t value) :
  type_(Integer),
  value_(value) {
}
  
Value::Value(const std::string& value) :
  type_(String),
  value_(value) {
}
  
Value::Value(const semantic_map::prolog::Term& value) :
  type_(Term),
  value_(value) {
}
  
Value::Value(const std::vector<Value>& value) :
  type_(List),
  value_(value) {
}

Value::Value(const Value& src) :
  type_(src.type_),
  value_(src.value_) {
}

Value::~Value() {  
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

Value::Type Value::getType() const {
  return type_;
}

bool Value::isEmpty() const {
  return (type_ == Empty);
}

bool Value::isDouble() const {
  return (type_ == Double);
}

bool Value::isInteger() const {
  return (type_ == Integer);
}

bool Value::isString() const {
  return (type_ == String);
}

bool Value::isTerm() const {
  return (type_ == Term);
}

bool Value::isList() const {
  return (type_ == List);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

std::string Value::toString() const {
  if (type_ == Double)
    return boost::lexical_cast<std::string>(cast<double>());
  else if (type_ == Integer)
    return boost::lexical_cast<std::string>(cast<int64_t>());
  else if (type_ == String)
    return cast<std::string>();
  else if (type_ == Term) {
    const semantic_map::prolog::Term& term = cast<semantic_map::
      prolog::Term>();
    std::ostringstream stream;
    
    stream << term.getName() << "(";
    std::copy(term.getValues().begin(), term.getValues().end(),
      std::ostream_iterator<Value>(stream, ", "));
    
    return stream.str().substr(0, stream.str().size()-2)+")";
  }
  else if (type_ == List) {
    const std::vector<Value>& values = cast<std::vector<Value> >();
    std::ostringstream stream;
    
    std::copy(values.begin(), values.end(),
      std::ostream_iterator<Value>(stream, ", "));
    
    return "["+stream.str().substr(0, stream.str().size()-2)+"]";
  }
  else
    return std::string();
}

void Value::write(std::ostream& stream) const {
  stream << toString();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

std::ostream& operator<<(std::ostream& stream, const Value& value) {
  value.write(stream);
  return stream;
}

}

}
