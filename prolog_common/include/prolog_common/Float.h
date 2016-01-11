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

/** \file Float.h
  * \brief Header file providing the Float class interface
  */

#ifndef ROS_PROLOG_FLOAT_H
#define ROS_PROLOG_FLOAT_H

#include <prolog_common/Number.h>

namespace prolog {
  /** \brief Prolog float
    */
  class Float :
    public Number {
  public:
    /** \brief Constructor
      */
    Float(double value = 0.0);
      
    /** \brief Copy constructor
      */
    Float(const Float& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    Float(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Float();
    
    /** \brief Retrieve the value of this Prolog float
      */
    double getValue() const;
    
  protected:
    friend class Number;
    friend class Term;
    
    /** \brief Prolog float (implementation)
      */
    class Impl :
      public Number::Impl {
    public:
      Impl(double value);
      virtual ~Impl();
      
      double value_;
    };
  };
};

#endif
