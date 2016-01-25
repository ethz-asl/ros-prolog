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

/** \file JSONDeserializer.h
  * \brief Header file providing the JSONDeserializer class interface
  */

#ifndef ROS_PROLOG_SERIALIZATION_JSON_DESERIALIZER_H
#define ROS_PROLOG_SERIALIZATION_JSON_DESERIALIZER_H

#include <ros/exception.h>

#include <prolog_serialization/Deserializer.h>

namespace Json {
  class Value;
}

namespace prolog {
  namespace serialization {
    /** \brief Prolog deserializer for the JSON format
      */
    class JSONDeserializer :
      public Deserializer {
    public:
      /** \brief Exception thrown in case of a parse error
        */ 
      class ParseError :
        public ros::Exception {
      public:
        ParseError(const std::string& description);
      };
      
      /** \brief Exception thrown in case of a conversion error
        */ 
      class ConversionError :
        public ros::Exception {
      public:
        ConversionError(const std::string& description);
      };
      
      /** \brief Default constructor
        */
      JSONDeserializer();
      
      /** \brief Destructor
        */
      virtual ~JSONDeserializer();
      
      /** \brief Deserialize some Prolog bindings (implementation)
        */
      Bindings deserializeBindings(std::istream& stream) const;
      
      /** \brief Deserialize a Prolog clause (implementation)
        */
      Clause deserializeClause(std::istream& stream) const;
      
      /** \brief Deserialize a Prolog program (implementation)
        */
      Program deserializeProgram(std::istream& stream) const;
      
      /** \brief Deserialize a Prolog query (implementation)
        */
      Query deserializeQuery(std::istream& stream) const;
      
      /** \brief Deserialize a Prolog term (implementation)
        */
      Term deserializeTerm(std::istream& stream) const;
      
      /** \brief Deserialize a JSON value
        */
      Json::Value deserializeValue(std::istream& stream) const;
      
      /** \brief Convert a JSON value to some Prolog bindings
        */
      Bindings valueToBindings(const Json::Value& value) const;
      
      /** \brief Convert a JSON value to a Prolog clause
        */
      Clause valueToClause(const Json::Value& value) const;
      
      /** \brief Convert a JSON value to a Prolog program
        */
      Program valueToProgram(const Json::Value& value) const;
      
      /** \brief Convert a JSON value to a Prolog query
        */
      Query valueToQuery(const Json::Value& value) const;
      
      /** \brief Convert a JSON value to a Prolog term
        */
      Term valueToTerm(const Json::Value& value) const;
    };
  };
};

#endif
