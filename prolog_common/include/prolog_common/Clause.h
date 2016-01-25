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

/** \file Clause.h
  * \brief Header file providing the Clause class interface
  */

#ifndef ROS_PROLOG_CLAUSE_H
#define ROS_PROLOG_CLAUSE_H

#include <initializer_list>
#include <list>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <prolog_common/Term.h>

namespace prolog {
  class Fact;
  class Rule;
  
  /** \brief Prolog clause
    */    
  class Clause {
  public:
    /** \brief Default constructor
      */
    Clause();
      
    /** \brief Copy constructor
      */
    Clause(const Clause& src);
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog fact or rule using a predicate, a vector of argument
      *   terms, and a list of goal terms)
      */
    Clause(const std::string& predicate, const std::vector<Term>&
      arguments = std::vector<Term>(), const std::list<Term>&
      goals = std::list<Term>());
    
    /** \brief Constructor (overloaded version for constructing a
      *   Prolog fact or rule using a predicate, an initializer list
      *   of argument terms, and an initializer list of goal terms)
      */
    Clause(const std::string& predicate, const std::initializer_list<
      Term>& arguments, const std::initializer_list<Term>& goals =
      std::initializer_list<Term>());
    
    /** \brief Destructor
      */
    virtual ~Clause();
    
    /** \brief True, if this Prolog clause is a fact
      */
    bool isFact() const;
    
    /** \brief True, if this Prolog clause is a rule
      */
    bool isRule() const;
    
    /** \brief True, if this Prolog clause is valid
      */
    bool isValid() const;
    
  protected:
    /** \brief Prolog clause (implementation)
      */
    class Impl {
    public:
      Impl();
      virtual ~Impl();
    };
    
    /** \brief The Prolog clause's implementation
      */
    boost::shared_ptr<Impl> impl_;
  };
};

#endif
