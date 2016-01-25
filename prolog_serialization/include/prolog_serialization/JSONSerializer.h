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

/** \file JSONSerializer.h
  * \brief Header file providing the JSONSerializer class interface
  */

#ifndef ROS_PROLOG_SERIALIZATION_JSON_SERIALIZER_H
#define ROS_PROLOG_SERIALIZATION_JSON_SERIALIZER_H

#include <string>

#include <prolog_serialization/Serializer.h>

namespace Json {
  class Value;
}

namespace prolog {
  namespace serialization {
    /** \brief Prolog serializer for the JSON format
      */
    class JSONSerializer :
      public Serializer {
    public:
      /** \brief Definition of the JSON serializer output format enumerable
        */
      enum OutputFormat {
        UnstyledOutput,
        StyledOutput
      };
      
      /** \brief Default constructor
        */
      JSONSerializer(OutputFormat outputFormat = UnstyledOutput, const
        std::string& outputIndent = "\t");
      
      /** \brief Copy constructor
        */
      JSONSerializer(const JSONSerializer& src);
      
      /** \brief Destructor
        */
      virtual ~JSONSerializer();
      
      /** \brief Set the output format of this JSON serializer
        */
      void setOutputFormat(OutputFormat outputFormat);
      
      /** \brief Retrieve the output format of this JSON serializer
        */
      OutputFormat getOutputFormat() const;
      
      /** \brief Set the styled output indent of this JSON serializer
        */
      void setOutputIndent(const std::string& indent);
      
      /** \brief Retrieve the styled output indent of this JSON serializer
        */
      const std::string& getOutputIndent() const;
      
      /** \brief Serialize some Prolog bindings (implementation)
        */
      void serializeBindings(std::ostream& stream, const Bindings&
        bindings) const;
      
      /** \brief Serialize a Prolog clause (implementation)
        */
      void serializeClause(std::ostream& stream, const Clause& clause) const;
      
      /** \brief Serialize a Prolog program (implementation)
        */
      void serializeProgram(std::ostream& stream, const Program& program)
        const;
      
      /** \brief Serialize a Prolog query (implementation)
        */
      void serializeQuery(std::ostream& stream, const Query& query) const;
      
      /** \brief Serialize a Prolog term (implementation)
        */
      void serializeTerm(std::ostream& stream, const Term& term) const;
      
      /** \brief Serialize a JSON value
        */
      void serializeValue(std::ostream& stream, const Json::Value& value)
        const;
      
      /** \brief Convert a Prolog atom to a JSON value
        */
      Json::Value atomToValue(const Atom& atom) const;
      
      /** \brief Convert some Prolog bindings to a JSON value
        */
      Json::Value bindingsToValue(const Bindings& bindings) const;
      
      /** \brief Convert a Prolog clause to a JSON value
        */
      Json::Value clauseToValue(const Clause& clause) const;
      
      /** \brief Convert a Prolog compound to a JSON value
        */
      Json::Value compoundToValue(const Compound& compound) const;
      
      /** \brief Convert a Prolog fact to a JSON value
        */
      Json::Value factToValue(const Fact& fact) const;
      
      /** \brief Convert a Prolog float to a JSON value
        */
      Json::Value floatToValue(const Float& number) const;
      
      /** \brief Convert a Prolog integer to a JSON value
        */
      Json::Value integerToValue(const Integer& number) const;
      
      /** \brief Convert a Prolog number to a JSON value
        */
      Json::Value numberToValue(const Number& number) const;
      
      /** \brief Convert a Prolog list to a JSON value
        */
      Json::Value listToValue(const List& list) const;
      
      /** \brief Convert a Prolog program to a JSON value
        */
      Json::Value programToValue(const Program& program) const;
      
      /** \brief Convert a Prolog query to a JSON value
        */
      Json::Value queryToValue(const Query& query) const;
      
      /** \brief Convert a Prolog rule to a JSON value
        */
      Json::Value ruleToValue(const Rule& rule) const;
      
      /** \brief Convert a Prolog term to a JSON value
        */
      Json::Value termToValue(const Term& term) const;
      
      /** \brief Convert a Prolog variable to a JSON value
        */
      Json::Value variableToValue(const Variable& variable) const;
      
    private:
      OutputFormat outputFormat_;
      std::string outputIndent_;
    };
  };
};

#endif
