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

#include <prolog/Exceptions.h>

#include "prolog/Query.h"

namespace semantic_map {

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Query::Query(const std::string& functor, const std::list<std::string>&
    arguments) :
  string_(functor) {
}

Query::Query(const Query& src) :
  string_(src.string_) {
}

Query::~Query() {  
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

const std::string& Query::getString() const {
  return string_;
}

bool Query::isEmpty() const {
  return string_.empty();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Query::write(std::ostream& stream) const {
  stream << string_;
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Query& Query::operator|=(const Query& query) {
  if (!string_.empty() && !query.string_.empty())
    string_ += ";"+query.string_;
  else
    throw InvalidOperationException("Empty operand to query operator [or].");
  
  return *this;
}

Query& Query::operator&=(const Query& query) {
  if (!string_.empty() && !query.string_.empty())
    string_ += ","+query.string_;
  else
    throw InvalidOperationException("Empty operand to query operator [and].");
  
  return *this;
}

Query operator|(const Query& lhs, const Query& rhs) {
  Query result = lhs;
  
  result |= rhs;
  
  return result;
}

Query operator&(const Query& lhs, const Query& rhs) {
  Query result = lhs;
  
  result &= rhs;
  
  return result;
}

std::ostream& operator<<(std::ostream& stream, const Query& query) {
  query.write(stream);
  return stream;
}

}

}
