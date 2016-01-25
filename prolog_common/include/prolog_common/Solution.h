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

/** \file Solution.h
  * \brief Header file providing the Solution class interface
  */

#ifndef ROS_PROLOG_SOLUTION_H
#define ROS_PROLOG_SOLUTION_H

#include <string>

#include <boost/type_traits.hpp>

#include <ros/exception.h>

#include <prolog_common/Bindings.h>

namespace prolog {
  /** \brief Prolog solutions
    */
  class Solution {
  public:
    /** \brief Exception thrown in case of an attempted access of
      *   an unbound term
      */ 
    class NoSuchTerm :
      public ros::Exception {
    public:
      NoSuchTerm(const std::string& name);
    };
      
    /** \brief Exception thrown in case of a failure to convert a
      *   Prolog term to a value
      */ 
    class ConversionError :
      public ros::Exception {
    public:
      ConversionError(const std::string& name);
    };
      
    /** \brief Default constructor
      */
    Solution();
    
    /** \brief Constructor (overloaded version taking some Prolog bindings)
      */
    Solution(const Bindings& bindings);
    
    /** \brief Copy constructor
      */
    Solution(const Solution& src);
    
    /** \brief Destructor
      */
    virtual ~Solution();
    
    /** \brief Retrieve the bindings of this Prolog solution
      */
    Bindings getBindings() const;
    
    /** \brief Retrieve a value of this Prolog solution by name
      */
    template <typename T> T getValue(const std::string& name) const;
    
    /** \brief True, if this Prolog solution is valid
      */ 
    bool isValid() const;
    
    /** \brief True, if this Prolog solution is empty
      */ 
    bool isEmpty() const;
    
    /** \brief Clear this Prolog solution
      */ 
    void clear();
    
  protected:
    /** \brief Prolog solution (implementation)
      */
    class Impl {
    public:
      Impl(const Bindings& bindings);
      virtual ~Impl();
      
      Bindings bindings_;
    };
    
    /** \brief Convert a term to a value (overloaded version returning
      *   a string value)
      */ 
    template <typename T> static void termToValue(const std::string&
      name, const Term& term, T& value, typename boost::enable_if<boost::
      is_base_of<std::string, T> >::type* = 0);
    
    /** \brief Convert a term to a value (overloaded version returning
      *   an integral value)
      */ 
    template <typename T> static void termToValue(const std::string&
      name, const Term& term, T& value, typename boost::enable_if<boost::
      is_integral<T> >::type* = 0);
    
    /** \brief Convert a term to a value (overloaded version returning
      *   a floating point value)
      */ 
    template <typename T> static void termToValue(const std::string&
      name, const Term& term, T& value, typename boost::enable_if<boost::
      is_floating_point<T> >::type* = 0);
    
    /** \brief Convert a term to a value (overloaded version returning
      *   a list value)
      */ 
    template <template <typename...> class C, typename T> static void
      termToValue(const std::string& name, const Term& term, C<T>& value,
      typename boost::disable_if<boost::is_base_of<std::string, C<T> > >::
      type* = 0);
    
    /** \brief The Prolog solution's implementation
      */
    boost::shared_ptr<Impl> impl_;
  };
};

#include <prolog_common/Solution.tpp>

#endif
