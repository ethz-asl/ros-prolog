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

#include <prolog_common/Float.h>
#include <prolog_common/Integer.h>

#include "prolog_common/Number.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Number::Number() {
}

Number::Number(const Number& src) :
  Term(src) {
}

Number::Number(const Term& src) :
  Term(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Number::~Number() {  
}

Number::Impl::Impl() {
}

Number::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Number::isFloat() const {
  return boost::dynamic_pointer_cast<Float::Impl>(impl_).get();
}

bool Number::isInteger() const {
  return boost::dynamic_pointer_cast<Integer::Impl>(impl_).get();
}

}
