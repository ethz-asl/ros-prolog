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

/** \file Term.h
  * \brief Header file providing the example Term class interface
  */

#ifndef ROS_PROLOG_SWI_TERM_H
#define ROS_PROLOG_SWI_TERM_H

#include <boost/shared_ptr.hpp>

#include <ros/exception.h>

#include <prolog_common/Term.h>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog term
      */
    class Term {
    public:
      /** \brief Exception thrown in case of a failure to convert a
        *   SWI-Prolog term to a Prolog term
        */ 
      class ConversionError :
        public ros::Exception {
      public:
        ConversionError();
      };
      
      /** \brief Default constructor
        */
      Term();
      
      /** \brief Copy constructor
        */
      Term(const Term& src);
      
      /** \brief Destructor
        */
      virtual ~Term();
    
      /** \brief True, if this SWI-Prolog term is empty
        */
      bool isEmpty() const;
      
      /** \brief True, if this SWI-Prolog term is valid
        */
      bool isValid() const;
      
      /** \brief Operator for converting this SWI-Prolog term to a
        *   Prolog term
        */
      operator prolog::Term() const;
      
    protected:
      friend class Bindings;
      friend class Query;
      
      /** \brief SWI-Prolog term (implementation)
        */
      class Impl {
      public:
        Impl(const std::string& goal);
        Impl(const prolog::Term& term);
        Impl(unsigned long handle);
        virtual ~Impl();
        
        operator prolog::Term() const;
        
        unsigned long handle_;
      };
      
      /** \brief Constructor (overloaded version taking a handle)
        */
      Term(unsigned long handle);
      
      /** \brief The SWI-Prolog term's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
