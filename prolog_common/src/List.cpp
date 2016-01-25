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

#include "prolog_common/List.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

List::List(const std::list<Term>& elements) {
  impl_.reset(new Impl(elements));
}

List::List(const List& src) :
  Term(src) {
}

List::List(const Term& src) :
  Term(src) {
  BOOST_ASSERT(boost::dynamic_pointer_cast<Impl>(impl_));
}

List::~List() {  
}

List::Impl::Impl(const std::list<Term>& elements) :
  elements_(elements) {
}

List::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t List::getNumElements() const {
  return boost::static_pointer_cast<Impl>(impl_)->elements_.size();
}

std::list<Term> List::getElements() const {
  return boost::static_pointer_cast<Impl>(impl_)->elements_;
}

bool List::isEmpty() const {
  return boost::static_pointer_cast<Impl>(impl_)->elements_.empty();
}
      
/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

std::list<Term>::iterator List::begin() {
  return boost::static_pointer_cast<Impl>(impl_)->elements_.begin();
}

std::list<Term>::const_iterator List::begin() const {
  return boost::static_pointer_cast<Impl>(impl_)->elements_.begin();
}

std::list<Term>::iterator List::end() {
  return boost::static_pointer_cast<Impl>(impl_)->elements_.end();
}

std::list<Term>::const_iterator List::end() const {
  return boost::static_pointer_cast<Impl>(impl_)->elements_.end();
}

void List::append(const Term& element) {
  boost::static_pointer_cast<Impl>(impl_)->elements_.push_back(element);
}

void List::append(const List& list) {
  std::list<Term>& elements = boost::static_pointer_cast<Impl>(
    impl_)->elements_;
  std::list<Term>& listElements = boost::static_pointer_cast<Impl>(
    list.impl_)->elements_;
  
  elements.insert(elements.end(), listElements.begin(), listElements.end());
}

void List::clear() {
  boost::static_pointer_cast<Impl>(impl_)->elements_.clear();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

List& List::operator+=(const Term& element) {
  append(element);
  
  return *this;
}

List& List::operator+=(const List& list) {
  append(list);
  
  return *this;
}

List List::operator+(const Term& element) const {
  List result(*this);
  
  result.append(element);
  
  return result;
}

List List::operator+(const List& list) const {
  List result(*this);
  
  result.append(list);
  
  return result;
}

}
