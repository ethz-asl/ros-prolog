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

/** \file Query.h
  * \brief Header file providing the Query class interface
  */

#ifndef SEMANTIC_MAP_PROLOG_QUERY_H
#define SEMANTIC_MAP_PROLOG_QUERY_H

#include <list>
#include <string>

namespace semantic_map {
  namespace prolog {
    /** \brief Prolog query implementation
      */
    class Query {
    public:
      /** \brief Default constructor
        */
      Query(const std::string& functor = std::string(), const
        std::list<std::string>& arguments = std::list<std::string>());
      
      /** \brief Copy constructor
        * 
        * \param[in] src The source Prolog query which is being copied
        *   to this Prolog query.
        */
      Query(const Query& src);
      
      /** \brief Destructor
        */
      ~Query();
      
      /** \brief Retrieve the string representation of this query
        */ 
      const std::string& getString() const;

      /** \brief True, if this Prolog query is empty
        */ 
      bool isEmpty() const;
      
      /** \brief Write this Prolog query to an output stream
        */ 
      void write(std::ostream& stream) const;
      
      /** \brief Unary "or" operator
        */ 
      Query& operator|=(const Query& query);
      
      /** \brief Unary "and" operator
        */ 
      Query& operator&=(const Query& query);
      
    private:
      /** \brief The Prolog query's string representation
        */
      std::string string_;
    };
    
    /** \brief Binary Prolog query "or" operator
      */ 
    Query operator|(const Query& lhs, const Query& rhs);
    
    /** \brief Binary Prolog query "and" operator
      */ 
    Query operator&(const Query& lhs, const Query& rhs);
    
    /** \brief Operator for writing a Prolog query to an output stream
      */ 
    std::ostream& operator<<(std::ostream& stream, const Query& query);
  };
};

#endif
