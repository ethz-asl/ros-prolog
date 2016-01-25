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

/** \file Term.h
  * \brief Header file providing the Term class interface
  */

#ifndef ROS_PROLOG_TERM_H
#define ROS_PROLOG_TERM_H

#include <initializer_list>
#include <list>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace prolog {
  class Atom;
  class Compound;
  class Float;
  class Integer;    
  class List;
  class Number;
  class Variable;
  
  /** \brief Prolog term
    */    
  class Term {
  public:
    /** \brief Default constructor
      */
    Term();
      
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog atom or variable by character array name)
      */
    Term(const char* name);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog atom or variable by string name)
      */
    Term(const std::string& name);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog integer from a 32-bit integer)
      */
    Term(int value);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog integer from a 64-bit integer)
      */
    Term(int64_t value);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog float)
      */
    Term(double value);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog list from a list of element terms)
      */
    Term(const std::list<Term>& elements);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog list from an initializer list of element terms)
      */
    Term(const std::initializer_list<Term>& elements);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog compound term from a functor and a vector of argument
      *   terms)
      */
    Term(const std::string& functor, const std::vector<Term>& arguments);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog compound term from a functor and an initializer list
      *   of a argument terms)
      */
    Term(const std::string& functor, const std::initializer_list<Term>&
      arguments);
    
    /** \brief Copy constructor
      */
    Term(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Term();
    
    /** \brief True, if this Prolog term is an atom
      */
    bool isAtom() const;
    
    /** \brief True, if this Prolog term is a compound term
      */
    bool isCompound() const;
    
    /** \brief True, if this Prolog term is a list
      */
    bool isList() const;
    
    /** \brief True, if this Prolog term is a number
      */
    bool isNumber() const;
    
    /** \brief True, if this Prolog term is a variable
      */
    bool isVariable() const;
    
    /** \brief True, if this Prolog term is valid
      */
    bool isValid() const;
    
    /** \brief Binary operator for constructing a Prolog compound term
      *   using conjunction
      */
    Compound operator&(const Term& term) const;
    
    /** \brief Binary operator for constructing a Prolog compound term
      *   using disjunction
      */
    Compound operator|(const Term& term) const;
    
  protected:
    /** \brief Prolog term (implementation)
      */
    class Impl {
    public:
      Impl();
      virtual ~Impl();
    };
    
    /** \brief The Prolog term's implementation
      */
    boost::shared_ptr<Impl> impl_;
  };
};

#endif
