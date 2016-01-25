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

#include "prolog_common/Solution.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Solution::NoSuchTerm::NoSuchTerm(const std::string& name) :
  ros::Exception("Solution contains no binding for ["+name+"].") {
}

Solution::ConversionError::ConversionError(const std::string& name) :
  ros::Exception("Failure to convert solution term bound to ["+name+"].") {
}

Solution::Solution() {
}

Solution::Solution(const Bindings& bindings) :
  impl_(new Impl(bindings)) {
}

Solution::Solution(const Solution& src) :
  impl_(src.impl_) {
}

Solution::~Solution() {  
}

Solution::Impl::Impl(const Bindings& bindings) :
  bindings_(bindings) {
}

Solution::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

Bindings Solution::getBindings() const {
  if (impl_.get())
    return impl_->bindings_;
  else
    return Bindings();
}

bool Solution::isValid() const {
  return impl_.get();
}

bool Solution::isEmpty() const {
  if (impl_.get())
    return impl_->bindings_.areEmpty();
  else
    return true;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Solution::clear() {
  if (impl_.get())
    impl_->bindings_.clear();
}

}
