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

/** \file Query.h
  * \brief Header file providing the Query class interface
  */

#ifndef ROS_PROLOG_QUERY_H
#define ROS_PROLOG_QUERY_H

#include <initializer_list>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog query
    */    
  class Query {
  public:
    /** \brief Default constructor
      */
    Query();
      
    /** \brief Constructor (overloaded version taking a predicate and 
      *   a vector of arguments)
      */
    Query(const std::string& predicate, const std::vector<Term>& arguments = 
      std::vector<Term>());
    
    /** \brief Constructor (overloaded version taking a predicate and 
      *   an initializer list of arguments)
      */
    Query(const std::string& predicate, const std::initializer_list<Term>&
      arguments);
    
    /** \brief Constructor (overloaded version taking a module, a predicate,
      *   and a vector of arguments)
      */
    Query(const std::string& module, const std::string& predicate, const
      std::vector<Term>& arguments = std::vector<Term>());
    
    /** \brief Constructor (overloaded version taking a module, a predicate,
      *   and an initializer list of arguments)
      */
    Query(const std::string& module, const std::string& predicate, const
      std::initializer_list<Term>& arguments);
    
    /** \brief Copy constructor
      */
    Query(const Query& src);
    
    /** \brief Destructor
      */
    virtual ~Query();
    
    /** \brief Retrieve the module of this Prolog query
      */
    std::string getModule() const;
    
    /** \brief Retrieve the predicate of this Prolog query
      */
    std::string getPredicate() const;
    
    /** \brief Retrieve the arguments of this Prolog query
      */
    std::vector<Term> getArguments() const;
    
    /** \brief Retrieve the arity of this Prolog query
      */
    size_t getArity() const;
    
    /** \brief True, if this Prolog query is a goal
      * 
      * A goal is represented as a query with arity zero or without
      * variable arguments.
      */
    bool isGoal() const;
    
    /** \brief True, if this Prolog query is valid
      */
    bool isValid() const;
    
    /** \brief Retrieve the argument begin iterator of this Prolog query
      */ 
    std::vector<Term>::iterator begin();
    
    /** \brief Retrieve the argument begin const-iterator of this Prolog
      *   query
      */ 
    std::vector<Term>::const_iterator begin() const;

    /** \brief Retrieve the argument end iterator of this Prolog query
      */ 
    std::vector<Term>::iterator end();
    
    /** \brief Retrieve the argument end const-iterator of this Prolog
      *   query
      */ 
    std::vector<Term>::const_iterator end() const;
    
  protected:
    /** \brief Prolog query (implementation)
      */
    class Impl {
    public:
      Impl(const std::string& module = std::string(), const std::string&
        predicate = std::string(), const std::vector<Term>& arguments =
        std::vector<Term>());
      virtual ~Impl();
      
      std::string module_;
      std::string predicate_;
      std::vector<Term> arguments_;
    };
    
    /** \brief The Prolog query's implementation
      */
    boost::shared_ptr<Impl> impl_;
  };
};

#endif
