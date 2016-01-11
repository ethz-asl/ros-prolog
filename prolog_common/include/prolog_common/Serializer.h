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

/** \file Serializer.h
  * \brief Header file providing the Serializer class interface
  */

#ifndef ROS_PROLOG_SERIALIZER_H
#define ROS_PROLOG_SERIALIZER_H

#include <iostream>

#include <prolog_common/Bindings.h>
#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Abstract Prolog serializer
    */
  class Serializer {
  public:
    /** \brief Default constructor
      */
    Serializer();
    
    /** \brief Destructor
      */
    virtual ~Serializer();
    
    /** \brief Serialize some Prolog bindings (abstract declaration)
      */
    virtual void serializeBindings(std::ostream& stream, const Bindings&
      bindings) const = 0;
      
    /** \brief Serialize a Prolog term (abstract declaration)
      */
    virtual void serializeTerm(std::ostream& stream, const Term& term)
      const = 0;
  };
};

#endif
