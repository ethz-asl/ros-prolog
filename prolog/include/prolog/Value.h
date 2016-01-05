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

/** \file Value.h
  * \brief Header file providing the Value class interface
  */

#ifndef SEMANTIC_MAP_PROLOG_VALUE_H
#define SEMANTIC_MAP_PROLOG_VALUE_H

#include <iostream>
#include <string>
#include <vector>

#include <boost/any.hpp>

namespace semantic_map {
  namespace prolog {
    class Term;

    class Value {
    public:
      /** \brief Definition of the Prolog value type enumerable
        */
      enum Type {
        Empty,
        Double,
        Integer,
        String,
        List,
        Term
      };
    
      /** \brief Default constructor
        */
      Value();
        
      /** \brief Constructor (overloaded version taking a double)
        */
      Value(double value);
        
      /** \brief Constructor (overloaded version taking an integer)
        */
      Value(int value);
        
      /** \brief Constructor (overloaded version taking a 64-bit integer)
        */
      Value(int64_t value);
        
      /** \brief Constructor (overloaded version taking a string)
        */
      Value(const std::string& value);
        
      /** \brief Constructor (overloaded version taking a Prolog term)
        */
      Value(const semantic_map::prolog::Term& value);
        
      /** \brief Constructor (overloaded version taking a list of values)
        */
      Value(const std::vector<Value>& value);

      /** \brief Copy constructor
        * 
        * \param[in] src The source Prolog value which is being copied
        *   to this Prolog value.
        */
      Value(const Value& src);
      
      /** \brief Destructor
        */
      ~Value();
      
      /** \brief Retrieve the type of this Prolog value
        */
      Type getType() const;
      
      /** \brief True, if the type of this Prolog value is empty
        */
      bool isEmpty() const;
      
      /** \brief True, if the type of this Prolog value is double
        */
      bool isDouble() const;
      
      /** \brief True, if the type of this Prolog value is integer
        */
      bool isInteger() const;
      
      /** \brief True, if the type of this Prolog value is string
        */
      bool isString() const;
      
      /** \brief True, if the type of this Prolog value is term
        */
      bool isTerm() const;
      
      /** \brief True, if the type of this Prolog value is list
        */
      bool isList() const;
      
      /** \brief Cast this Prolog value into a value const-reference
        */
      template <typename T> const T& cast() const;

      /** \brief Convert this Prolog value to its string representation
        */ 
      std::string toString() const;

      /** \brief Write this Prolog value to an output stream
        */ 
      void write(std::ostream& stream) const;
      
      /** \brief Conversion operator
        */
      template <typename T> operator T() const;

    private:
      /** \brief The value type
        */
      Type type_;
      
      /** \brief The type-erased value
        */
      boost::any value_;
    };

    /** \brief Operator for writing a Prolog value to an output stream
      */ 
    std::ostream& operator<<(std::ostream& stream, const Value& value);
  };
};

#include <prolog/Value.tpp>

#endif
