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

/** \file Variable.h
  * \brief Header file providing the Variable class interface
  */

#ifndef ROS_PROLOG_VARIABLE_H
#define ROS_PROLOG_VARIABLE_H

#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog variable
    */
  class Variable :
    public Term {
  public:
    /** \brief Default constructor
      */
    Variable(const char* name = "_");
    
    /** \brief Constructor (overloaded version taking a string name)
      */
    Variable(const std::string& name);
    
    /** \brief Copy constructor
      */
    Variable(const Variable& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    Variable(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Variable();
    
    /** \brief Retrieve the name of this Prolog variable
      */
    std::string getName() const;
    
    /** \brief True, if this Prolog variable is anonymous
      */
    bool isAnonymous() const;
    
  protected:
    friend class Term;
    
    /** \brief Prolog variable (implementation)
      */
    class Impl :
      public Term::Impl {
    public:
      Impl(const std::string& name);
      virtual ~Impl();
      
      std::string name_;
    };
  };
};

#endif
