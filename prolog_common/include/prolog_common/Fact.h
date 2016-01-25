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

/** \file Fact.h
  * \brief Header file providing the Fact class interface
  */

#ifndef ROS_PROLOG_FACT_H
#define ROS_PROLOG_FACT_H

#include <initializer_list>
#include <string>
#include <vector>

#include <prolog_common/Clause.h>
#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog fact
    */
  class Fact :
    public Clause {
  public:
    /** \brief Constructor
      */
    Fact(const std::string& predicate, const std::vector<Term>& arguments =
      std::vector<Term>());
      
    /** \brief Copy constructor
      */
    Fact(const Fact& src);
    
    /** \brief Copy constructor (overloaded version taking a clause)
      */
    Fact(const Clause& src);
    
    /** \brief Destructor
      */
    virtual ~Fact();
    
    /** \brief Retrieve the predicate of this Prolog fact
      */
    std::string getPredicate() const;
    
    /** \brief Retrieve the arguments of this Prolog fact
      */
    std::vector<Term> getArguments() const;
    
    /** \brief Retrieve the arity of this Prolog fact
      */
    size_t getArity() const;
    
  protected:
    friend class Clause;
    
    /** \brief Prolog fact (implementation)
      */
    class Impl :
      public Clause::Impl {
    public:
      Impl(const std::string& predicate, const std::vector<Term>& arguments);
      virtual ~Impl();
      
      std::string predicate_;
      std::vector<Term> arguments_;
    };
  };
};

#endif
