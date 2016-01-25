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

/** \file Bindings.h
  * \brief Header file providing the Bindings class interface
  */

#ifndef ROS_PROLOG_BINDINGS_H
#define ROS_PROLOG_BINDINGS_H

#include <string>

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog bindings
    */
  class Bindings {
  public:
    /** \brief Definition of the Prolog bindings iterator type
      */
    typedef boost::unordered_map<std::string, Term>::iterator Iterator;
    
    /** \brief Definition of the Prolog bindings const-iterator type
      */
    typedef boost::unordered_map<std::string, Term>::const_iterator
      ConstIterator;
    
    /** \brief Default constructor
      */
    Bindings();
    
    /** \brief Copy constructor
      */
    Bindings(const Bindings& src);
    
    /** \brief Destructor
      */
    ~Bindings();
    
    /** \brief Retrieve a term of these Prolog bindings
      */
    Term getTerm(const std::string& name) const;
    
    /** \brief True, if these Prolog bindings contain a given term
      */
    bool contain(const std::string& name) const;
    
    /** \brief True, if these Prolog bindings are empty
      */
    bool areEmpty() const;
    
    /** \brief Retrieve the begin iterator of these Prolog bindings
      */ 
    boost::unordered_map<std::string, Term>::iterator begin();
    
    /** \brief Retrieve the begin const-iterator of these Prolog bindings
      */ 
    boost::unordered_map<std::string, Term>::const_iterator begin() const;

    /** \brief Retrieve the end iterator of these Prolog bindings
      */ 
    boost::unordered_map<std::string, Term>::iterator end();
    
    /** \brief Retrieve the end const-iterator of these Prolog bindings
      */ 
    boost::unordered_map<std::string, Term>::const_iterator end() const;

    /** \brief Add a term to these Prolog bindings
      */
    void addTerm(const std::string& name, const Term& term);
    
    /** \brief Clear these Prolog bindings
      */
    void clear();
    
    /** \brief Operator for retrieving a term of these Prolog bindings
      */
    Term operator[](const std::string& name) const;
    
  private:
    /** \brief Prolog bindings (implementation)
      */
    class Impl {
    public:
      Impl();
      ~Impl();
      
      boost::unordered_map<std::string, Term> terms_;
    };
    
    /** \brief The Prolog bindings' implementation
      */
    boost::shared_ptr<Impl> impl_;
  };
};

#endif
