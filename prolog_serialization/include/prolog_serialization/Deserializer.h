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

/** \file Deserializer.h
  * \brief Header file providing the Deserializer class interface
  */

#ifndef ROS_PROLOG_SERIALIZATION_DESERIALIZER_H
#define ROS_PROLOG_SERIALIZATION_DESERIALIZER_H

#include <iostream>

#include <prolog_common/Bindings.h>
#include <prolog_common/Clause.h>
#include <prolog_common/Program.h>
#include <prolog_common/Query.h>
#include <prolog_common/Term.h>

namespace prolog {
  namespace serialization {
    /** \brief Abstract Prolog deserializer
      */
    class Deserializer {
    public:
      /** \brief Default constructor
        */
      Deserializer();
      
      /** \brief Destructor
        */
      virtual ~Deserializer();
      
      /** \brief Deserialize some Prolog bindings (abstract declaration)
        */
      virtual Bindings deserializeBindings(std::istream& stream) const = 0;
      
      /** \brief Deserialize a Prolog clause (abstract declaration)
        */
      virtual Clause deserializeClause(std::istream& stream) const = 0;
      
      /** \brief Deserialize a Prolog program (abstract declaration)
        */
      virtual Program deserializeProgram(std::istream& stream) const = 0;
      
      /** \brief Deserialize a Prolog query (abstract declaration)
        */
      virtual Query deserializeQuery(std::istream& stream) const = 0;
      
      /** \brief Deserialize a Prolog term (abstract declaration)
        */
      virtual Term deserializeTerm(std::istream& stream) const = 0;
    };
  };
};

#endif
