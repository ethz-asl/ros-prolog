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

/** \file Client.h
  * \brief Header file providing the Client class interface
  */

#ifndef ROS_PROLOG_CLIENT_H
#define ROS_PROLOG_CLIENT_H

#include <roscpp_nodewrap/NodeImpl.h>

#include <prolog_client/ServiceClient.h>

namespace prolog {
  namespace client {
    /** \brief Prolog client implementation
      */  
    class Client :
      public nodewrap::NodeImpl {
    public:
      /** \brief Default constructor
        */
      Client();
      
      /** \brief Destructor
        */
      virtual ~Client();
    
    protected:
      /** \brief Create a Prolog service client
        */
      ServiceClient prologServiceClient(const std::string& name, const
        std::string& defaultServiceNamespace, bool defaultPersistent =
        false);
    };
  };
};

#endif
