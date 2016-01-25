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
  * \brief Header file providing the example Query class interface
  */

#ifndef ROS_PROLOG_SWI_QUERY_H
#define ROS_PROLOG_SWI_QUERY_H

#include <initializer_list>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <prolog_common/Bindings.h>
#include <prolog_common/Query.h>
#include <prolog_common/Term.h>

#include <prolog_swi/Bindings.h>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog query
      */  
    class Query {
    public:
      /** \brief Default constructor
        */
      Query();
      
      /** \brief Constructor (overloaded version taking a goal term in
        *   Prolog syntax)
        */
      Query(const std::string& goal);
      
      /** \brief Constructor (overloaded version taking a predicate
        *   and a vector of term arguments)
        */
      Query(const std::string& predicate, const std::vector<prolog::Term>&
        arguments);
      
      /** \brief Constructor (overloaded version taking a predicate
        *   and an initializer list of term arguments)
        */
      Query(const std::string& predicate, const std::initializer_list<
        prolog::Term>& arguments);
      
      /** \brief Constructor (overloaded version taking a module, a predicate,
        *   and a vector of term arguments)
        */
      Query(const std::string& module, const std::string& predicate, const
        std::vector<prolog::Term>& arguments);
      
      /** \brief Constructor (overloaded version taking a module, a predicate,
        *   and an initializer list of term arguments)
        */
      Query(const std::string& module, const std::string& predicate,
        const std::initializer_list<prolog::Term>& arguments);
      
      /** \brief Constructor (overloaded version taking a Prolog query)
        */
      Query(const prolog::Query& query);
      
      /** \brief Copy constructor
        */
      Query(const Query& src);
      
      /** \brief Destructor
        */
      virtual ~Query();
    
      /** \brief Retrieve the arity of this SWI-Prolog query
        */
      size_t getArity() const;
      
      /** \brief True, if this SWI-Prolog query is open
        */
      bool isOpen() const;
      
      /** \brief True, if this SWI-Prolog query is valid
        */
      bool isValid() const;
      
      /** \brief Open this SWI-Prolog query
        */
      bool open();
      
      /** \brief Generate the next solution of this SWI-Prolog query
        */
      bool nextSolution(prolog::Bindings& bindings);
      
      /** \brief Cut this SWI-Prolog query
        */
      void cut();
      
      /** \brief Close this SWI-Prolog query
        */
      void close();
      
    protected:
      /** \brief SWI-Prolog query (implementation)
        */
      class Impl {
      public:
        Impl(const std::string& module, const std::string& predicate,
          const std::vector<prolog::Term>& arguments);
        virtual ~Impl();
        
        bool open();
        bool nextSolution(prolog::Bindings& bindings);
        void cut();
        void close();
        
        void generateBindings(const prolog::Term& argument, unsigned long
          handle, const boost::unordered_map<std::string, std::string>&
          mappings);

        std::string module_;
        std::string predicate_;
        std::vector<prolog::Term> arguments_;
        Bindings bindings_;
        
        void* moduleHandle_;
        void* predicateHandle_;
        unsigned long argumentsHandle_;
        
        unsigned long handle_;
      };
      
      /** \brief The SWI-Prolog query's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
