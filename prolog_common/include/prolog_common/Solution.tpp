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

#include <prolog_common/Atom.h>
#include <prolog_common/Float.h>
#include <prolog_common/Integer.h>
#include <prolog_common/List.h>

namespace prolog {

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

template <typename T> T Solution::getValue(const std::string& name) const {
  if (impl_.get()) {
    Term term = this->impl_->bindings_.getTerm(name);
    
    if (term.isValid()) {
      T value;
      
      Solution::termToValue(name, term, value);
      
      return value;
    }
  }

  throw NoSuchTerm(name);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

template <typename T> void Solution::termToValue(const std::string&
    name, const Term& term, T& value, typename boost::enable_if<boost::
    is_base_of<std::string, T> >::type*) {
  if (term.isAtom()) {
    value = Atom(term).getName();
    
    return;
  }

  throw ConversionError(name);
}

template <typename T> void Solution::termToValue(const std::string&
    name, const Term& term, T &value, typename boost::enable_if<boost::
    is_integral<T> >::type*) {
  if (term.isNumber()) {
    Number number(term);
    
    if (number.isInteger()) {
      value = Integer(number).getValue();
      
      return;
    }
  }

  throw ConversionError(name);
}

template <typename T> void Solution::termToValue(const std::string&
    name, const Term& term, T& value, typename boost::enable_if<boost::
    is_floating_point<T> >::type*) {
  if (term.isNumber()) {
    Number number(term);
    
    if (number.isFloat()) {
      value = Float(number).getValue();
      
      return;
    }
  }

  throw ConversionError(name);
}

template <template <typename...> class C, typename T> void Solution::
    termToValue(const std::string& name, const Term& term, C<T>& value,
    typename boost::disable_if<boost::is_base_of<std::string, C<T>> >::
    type*) {
  if (term.isList()) {
    List list(term);
    
    for (std::list<Term>::const_iterator it = list.begin();
        it != list.end(); ++it) {
      T element;
      
      Solution::termToValue(name, *it, element);
    
      value.push_back(element);
    }
    
    return;
  }

  throw ConversionError(name);
}

}
