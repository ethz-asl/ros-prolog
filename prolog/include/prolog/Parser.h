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

/** \file Parser.h
  * \brief Header file providing the Parser class interface
  */

#ifndef SEMANTIC_MAP_PROLOG_PARSER_H
#define SEMANTIC_MAP_PROLOG_PARSER_H

#include <prolog/Bindings.h>

namespace semantic_map {
  namespace prolog {
    /** \brief Prolog JSON parser implementation
      */
    class Parser {
    public:
      /** \brief Default constructor
        */
      Parser();
      
      /** \brief Destructor
        */
      ~Parser();
      
      /** \brief Parse a JSON-formated expression
        */
      Bindings parse(const std::string& json) const;
    };
  };
};

#endif
