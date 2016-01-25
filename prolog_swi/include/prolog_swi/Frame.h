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

/** \file Frame.h
  * \brief Header file providing the example Frame class interface
  */

#ifndef ROS_PROLOG_SWI_FRAME_H
#define ROS_PROLOG_SWI_FRAME_H

#include <boost/shared_ptr.hpp>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog frame
      */  
    class Frame {
    public:
      /** \brief Default constructor
        */
      Frame();
      
      /** \brief Copy constructor
        */
      Frame(const Frame& src);
      
      /** \brief Destructor
        */
      virtual ~Frame();
    
      /** \brief True, if this SWI-Prolog frame is open
        */
      bool isOpen() const;
      
      /** \brief Open this SWI-Prolog frame
        */
      bool open();
      
      /** \brief Rewind this SWI-Prolog frame
        */
      void rewind();
      
      /** \brief Close this SWI-Prolog frame
        */
      void close();
      
    protected:
      /** \brief SWI-Prolog frame (implementation)
        */
      class Impl {
      public:
        Impl();
        virtual ~Impl();
        
        bool open();
        void rewind();
        void close();
        
        unsigned long handle_;
      };
      
      /** \brief The SWI-Prolog frame's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
