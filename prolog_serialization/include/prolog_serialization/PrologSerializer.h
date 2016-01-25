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

/** \file PrologSerializer.h
  * \brief Header file providing the PrologSerializer class interface
  */

#ifndef ROS_PROLOG_SERIALIZATION_PROLOG_SERIALIZER_H
#define ROS_PROLOG_SERIALIZATION_PROLOG_SERIALIZER_H

#include <prolog_serialization/Serializer.h>

namespace prolog {
  namespace serialization {
    /** \brief Prolog serializer for the Prolog syntax format
      */
    class PrologSerializer :
      public Serializer {
    public:
      /** \brief Default constructor
        */
      PrologSerializer();
      
      /** \brief Destructor
        */
      virtual ~PrologSerializer();
      
      /** \brief Serialize some Prolog bindings (implementation)
        */
      void serializeBindings(std::ostream& stream, const Bindings&
        bindings) const;
      
      /** \brief Serialize a Prolog clause (implementation)
        */
      void serializeClause(std::ostream& stream, const Clause& clause) const;
      
      /** \brief Serialize a Prolog program (implementation)
        */
      void serializeProgram(std::ostream& stream, const Program& program) const;
      
      /** \brief Serialize a Prolog query (implementation)
        */
      void serializeQuery(std::ostream& stream, const Query& query) const;
      
      /** \brief Serialize a Prolog term (implementation)
        */
      void serializeTerm(std::ostream& stream, const Term& term) const;
      
      /** \brief Serialize a Prolog atom
        */
      void serializeAtom(std::ostream& stream, const Atom& atom) const;
      
      /** \brief Serialize a Prolog compound
        */
      void serializeCompound(std::ostream& stream, const Compound& compound)
        const;
      
      /** \brief Serialize a Prolog fact
        */
      void serializeFact(std::ostream& stream, const Fact& fact) const;
      
      /** \brief Serialize a Prolog float
        */
      void serializeFloat(std::ostream& stream, const Float& number) const;
      
      /** \brief Serialize a Prolog integer
        */
      void serializeInteger(std::ostream& stream, const Integer& number)
        const;
      
      /** \brief Serialize a Prolog number
        */
      void serializeNumber(std::ostream& stream, const Number& number)
        const;
      
      /** \brief Serialize a Prolog list
        */
      void serializeList(std::ostream& stream, const List& list) const;
      
      /** \brief Serialize a Prolog rule
        */
      void serializeRule(std::ostream& stream, const Rule& rule) const;
      
      /** \brief Serialize a Prolog variable
        */
      void serializeVariable(std::ostream& stream, const Variable& variable)
        const;
    };
  };
};

#endif
