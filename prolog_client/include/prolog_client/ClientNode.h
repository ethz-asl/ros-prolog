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

/** \file ClientNode.h
  * \brief Header file providing the example ClientNode class interface
  */

#ifndef SEMANTIC_MAP_PROLOG_CLIENT_NODE_H
#define SEMANTIC_MAP_PROLOG_CLIENT_NODE_H

#include <roscpp_nodewrap/NodeImpl.h>
#include <roscpp_nodewrap/Nodelet.h>

namespace semantic_map {
  namespace prolog {
    /** \brief Prolog client node implementation
      */  
    class ClientNode :
      public nodewrap::NodeImpl {
    public:
      /** \brief Default constructor
        */
      ClientNode();
      
      /** \brief Destructor
        */
      virtual ~ClientNode();
    
    protected:
      /** \brief Initialize the Prolog client node
        */
      void init();
      
      /** \brief Cleanup the Prolog client node
        */
      void cleanup();
    };
  };
};

#endif
