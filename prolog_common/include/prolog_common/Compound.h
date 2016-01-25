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

/** \file Compound.h
  * \brief Header file providing the Compound class interface
  */

#ifndef ROS_PROLOG_COMPOUND_H
#define ROS_PROLOG_COMPOUND_H

#include <string>
#include <vector>

#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog compound
    */
  class Compound :
    public Term {
  public:
    /** \brief Constructor
      */
    Compound(const std::string& functor, const std::vector<Term>& arguments);
      
    /** \brief Copy constructor
      */
    Compound(const Compound& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    Compound(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~Compound();
    
    /** \brief Retrieve the functor of this Prolog compound term
      */
    std::string getFunctor() const;
    
    /** \brief Retrieve the arguments of this Prolog compound term
      */
    std::vector<Term> getArguments() const;
    
    /** \brief Retrieve the arity of this Prolog compound term
      */
    size_t getArity() const;
    
    /** \brief Retrieve the argument begin iterator of this Prolog
      *   compound term
      */ 
    std::vector<Term>::iterator begin();
    
    /** \brief Retrieve the argument begin const-iterator of this Prolog
      *   compound term
      */ 
    std::vector<Term>::const_iterator begin() const;

    /** \brief Retrieve the argument end iterator of this Prolog compound
      *   term
      */ 
    std::vector<Term>::iterator end();
    
    /** \brief Retrieve the argument end const-iterator of this Prolog
      *   compound term
      */ 
    std::vector<Term>::const_iterator end() const;
    
  protected:
    friend class Term;
    
    /** \brief Prolog compound (implementation)
      */
    class Impl :
      public Term::Impl {
    public:
      Impl(const std::string& functor, const std::vector<Term>& arguments);
      virtual ~Impl();
      
      std::string functor_;
      std::vector<Term> arguments_;
    };
  };
};

#endif
