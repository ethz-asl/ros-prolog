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

/** \file Rule.h
  * \brief Header file providing the Rule class interface
  */

#ifndef ROS_PROLOG_RULE_H
#define ROS_PROLOG_RULE_H

#include <list>
#include <string>
#include <vector>

#include <prolog_common/Clause.h>
#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog rule
    */
  class Rule :
    public Clause {
  public:
    /** \brief Constructor
      */
    Rule(const std::string& predicate, const std::vector<Term>& arguments,
      const std::list<Term>& goals);
      
    /** \brief Copy constructor
      */
    Rule(const Rule& src);
    
    /** \brief Copy constructor (overloaded version taking a clause)
      */
    Rule(const Clause& src);
    
    /** \brief Destructor
      */
    virtual ~Rule();
    
    /** \brief Retrieve the predicate of this Prolog rule
      */
    std::string getPredicate() const;
    
    /** \brief Retrieve the arguments of this Prolog rule
      */
    std::vector<Term> getArguments() const;
    
    /** \brief Retrieve the goals of this Prolog rule
      */
    std::list<Term> getGoals() const;
    
    /** \brief Retrieve the arity of this Prolog rule
      */
    size_t getArity() const;
    
    /** \brief Retrieve the number of goals of this Prolog rule
      */
    size_t getNumGoals() const;
    
    /** \brief Append a goal term to this Prolog rule
      */
    void append(const Term& goal);
    
    /** \brief Unary operator for adding a goal term to this Prolog rule
      */
    Rule& operator&=(const Term& goal);
    
  protected:
    friend class Clause;
    
    /** \brief Prolog rule (implementation)
      */
    class Impl :
      public Clause::Impl {
    public:
      Impl(const std::string& predicate, const std::vector<Term>& arguments,
        const std::list<Term>& goals);
      virtual ~Impl();
      
      std::string predicate_;
      std::vector<Term> arguments_;
      std::list<Term> goals_;
    };
  };
};

#endif
