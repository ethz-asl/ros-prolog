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

/** \file Atom.h
  * \brief Header file providing the Atom class interface
  */

#ifndef ROS_PROLOG_ATOM_H
#define ROS_PROLOG_ATOM_H

#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog atom
    */
  class Atom :
    public Term {
  public:
    /** \brief Constructor (overloaded version taking a character
      *   array name)
      */
    Atom(const char* name);
    
    /** \brief Constructor (overloaded version taking a string name)
      */
    Atom(const std::string& name);
      
    /** \brief Copy constructor
      */
    Atom(const Atom& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    Atom(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Atom();
    
    /** \brief Retrieve the name of this Prolog atom
      */
    std::string getName() const;
    
  protected:
    friend class Term;
    
    /** \brief Prolog atom (implementation)
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
