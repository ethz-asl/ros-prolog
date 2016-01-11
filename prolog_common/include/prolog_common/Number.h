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

/** \file Number.h
  * \brief Header file providing the Number class interface
  */

#ifndef ROS_PROLOG_NUMBER_H
#define ROS_PROLOG_NUMBER_H

#include <prolog_common/Term.h>

namespace prolog {
  class Float;
  class Integer;
  
  /** \brief Prolog number
    */
  class Number :
    public Term {
  public:
    /** \brief Constructor
      */
    Number();
      
    /** \brief Copy constructor
      */
    Number(const Number& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    Number(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Number();
    
    /** \brief True, if this Prolog number is a float
      */
    bool isFloat() const;
    
    /** \brief True, if this Prolog number is an integer
      */
    bool isInteger() const;
    
  protected:
    friend class Term;
    
    /** \brief Prolog number (implementation)
      */
    class Impl :
      public Term::Impl {
    public:
      Impl();
      virtual ~Impl();
    };
  };
};

#endif
