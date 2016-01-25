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

/** \file Exception.h
  * \brief Header file providing the example Exception class interface
  */

#ifndef ROS_PROLOG_SWI_EXCEPTION_H
#define ROS_PROLOG_SWI_EXCEPTION_H

#include <ros/exception.h>

#include <prolog_swi/Query.h>
#include <prolog_swi/Term.h>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog exception
      */  
    class Exception :
      public Term,
      public ros::Exception {
    public:
      /** \brief Default constructor
        */
      Exception();
      
      /** \brief Copy constructor
        */
      Exception(const Exception& src);
      
      /** \brief Destructor
        */
      virtual ~Exception() throw();
      
      /** \brief Raise this SWI-Prolog exception
        */
      void raise();
      
    private:
      friend class Query;
      friend class Term;
      
      /** \brief Constructor (overloaded version taking a SWI-Prolog
        *   term handle)
        */
      Exception(unsigned long handle);
      
      /** \brief The handle of this SWI-Prolog exception
        */
      static std::string messageToString(unsigned long handle);
    };
  };
};

#endif
