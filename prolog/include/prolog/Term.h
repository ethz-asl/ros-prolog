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

#ifndef SEMANTIC_MAP_PROLOG_TERM_H
#define SEMANTIC_MAP_PROLOG_TERM_H

#include <string>
#include <vector>

#include <prolog/Value.h>

namespace semantic_map {
  namespace prolog {
    /** \brief Prolog term implementation
      */
    class Term {
    public:
      /** \brief Constructor
        */
      Term(const std::string& name, const std::vector<Value>& values);
      
      /** \brief Copy constructor
        * 
        * \param[in] src The source Prolog term which is being copied
        *   to this Prolog term.
        */
      Term(const Term& src);
      
      /** \brief Destructor
        */
      ~Term();
      
      /** \brief Retrieve the name of this Prolog term
        */
      const std::string& getName() const;
      
      /** \brief Retrieve the values of this Prolog term
        */
      const std::vector<Value>& getValues() const;
      
      /** \brief Retrieve the arity of this Prolog term
        */
      size_t getArity() const;
      
      /** \brief Retrieve a value of this Prolog term
        */
      const Value& getValue(size_t index) const;
      
      /** \brief Operator for retrieving a value of this Prolog term
        */
      const Value& operator[](size_t index) const;
      
    private:
      /** \brief The Prolog term's name
        */
      std::string name_;
      
      /** \brief The Prolog term's values
        */
      std::vector<Value> values_;
    };
  };
};

#endif
