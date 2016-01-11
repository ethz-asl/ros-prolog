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

#include "prolog_common/Float.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Float::Float(double value) {
  impl_.reset(new Impl(value));
}

Float::Float(const Float& src) :
  Number(src) {
}

Float::Float(const Term& src) :
  Number(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

Float::~Float() {  
}

Float::Impl::Impl(double value) :
  value_(value) {
}

Float::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

double Float::getValue() const {
  return boost::static_pointer_cast<Impl>(impl_)->value_;
}

}
