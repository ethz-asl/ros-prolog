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
  * \brief Header file providing the example Bindings class interface
  */

#ifndef ROS_PROLOG_SWI_BINDINGS_H
#define ROS_PROLOG_SWI_BINDINGS_H

#include <string>

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include <prolog_common/Bindings.h>
#include <prolog_common/Term.h>

#include <prolog_swi/Term.h>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog bindings
      */  
    class Bindings {
    public:
      /** \brief Default constructor
        */
      Bindings();
      
      /** \brief Copy constructor
        */
      Bindings(const Bindings& src);
      
      /** \brief Destructor
        */
      virtual ~Bindings();
    
      /** \brief True, if these SWI-Prolog bindings are empty
        */
      bool areEmpty() const;
      
      /** \brief Add a term to these SWI-Prolog bindings
        */
      void addTerm(const std::string& name, const Term& term);
      
      /** \brief Clear these SWI-Prolog bindings
        */
      void clear();
            
      /** \brief Operator for converting these SWI-Prolog bindings to
        *   some Prolog bindings
        */
      operator prolog::Bindings() const;
      
    protected:
      friend class Query;
      
      /** \brief SWI-Prolog bindings (implementation)
        */  
      class Impl {
      public:
        Impl();
        virtual ~Impl();
        
        operator prolog::Bindings() const;
        
        boost::unordered_map<std::string, Term> terms_;
      };
      
      /** \brief The SWI-Prolog bindings' implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
