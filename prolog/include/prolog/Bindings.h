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

#ifndef SEMANTIC_MAP_PROLOG_BINDINGS_H
#define SEMANTIC_MAP_PROLOG_BINDINGS_H

#include <string>

#include <boost/unordered_map.hpp>

#include <prolog/Value.h>

namespace semantic_map {
  namespace prolog {
    /** \brief Prolog bindings implementation
      */
    class Bindings {
    public:
      /** \brief Default constructor
        */
      Bindings();
      
      /** \brief Copy constructor
        * 
        * \param[in] src The source Prolog bindings which are being copied
        *   to these Prolog bindings.
        */
      Bindings(const Bindings& src);
      
      /** \brief Destructor
        */
      ~Bindings();
      
      /** \brief Retrieve the values of these Prolog bindings
        */
      const boost::unordered_map<std::string, Value>& getValues() const;
      
      /** \brief Retrieve a value of these Prolog bindings
        */
      const Value& getValue(const std::string& name) const;
      
      /** \brief Add a value to these Prolog bindings
        */
      void addValue(const std::string& name, const Value& value);
      
      /** \brief Retrieve the begin iterator of these Prolog bindings
        */ 
      boost::unordered_map<std::string, Value>::iterator begin();
      
      /** \brief Retrieve the begin const-iterator of these Prolog bindings
        */ 
      boost::unordered_map<std::string, Value>::const_iterator begin() const;

      /** \brief Retrieve the end iterator of these Prolog bindings
        */ 
      boost::unordered_map<std::string, Value>::iterator end();
      
      /** \brief Retrieve the end const-iterator of these Prolog bindings
        */ 
      boost::unordered_map<std::string, Value>::const_iterator end() const;
  
      /** \brief Operator for retrieving a value of these Prolog bindings
        */
      const Value& operator[](const std::string& name) const;
      
    private:
      /** \brief The values of these Prolog bindings
        */
      boost::unordered_map<std::string, Value> values_;
    };
  };
};

#endif
