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

#include "prolog_common/Integer.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Integer::Integer(int64_t value) {
  impl_.reset(new Impl(value));
}

Integer::Integer(const Integer& src) :
  Number(src) {
}

Integer::Integer(const Term& src) :
  Number(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Integer::~Integer() {  
}

Integer::Impl::Impl(int64_t value) :
  value_(value) {
}

Integer::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

int64_t Integer::getValue() const {
  return boost::static_pointer_cast<Impl>(impl_)->value_;
}

}
