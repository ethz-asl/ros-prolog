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

/** \file Integer.h
  * \brief Header file providing the Integer class interface
  */

#ifndef ROS_PROLOG_INTEGER_H
#define ROS_PROLOG_INTEGER_H

#include <prolog_common/Number.h>

namespace prolog {
  /** \brief Prolog integer
    */
  class Integer :
    public Number {
  public:
    /** \brief Constructor
      */
    Integer(int64_t value = 0);
      
    /** \brief Copy constructor
      */
    Integer(const Integer& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    Integer(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Integer();
    
    /** \brief Retrieve the value of this Prolog integer
      */
    int64_t getValue() const;
    
  protected:
    friend class Number;
    friend class Term;
    
    /** \brief Prolog integer (implementation)
      */
    class Impl :
      public Number::Impl {
    public:
      Impl(int64_t value);
      virtual ~Impl();
      
      int64_t value_;
    };
  };
};

#endif
