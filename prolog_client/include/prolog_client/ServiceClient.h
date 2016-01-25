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

/** \file ServiceClient.h
  * \brief Header file providing the ServiceClient class interface
  */

#ifndef ROS_PROLOG_CLIENT_SERVICE_CLIENT_H
#define ROS_PROLOG_CLIENT_SERVICE_CLIENT_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <roscpp_nodewrap/ServiceClient.h>

namespace prolog {
  namespace client {
    class Query;
    
    /** \brief Prolog service client
      */
    class ServiceClient {
    public:
      /** \brief Default constructor
        */
      ServiceClient();
      
      /** \brief Copy constructor
        */
      ServiceClient(const ServiceClient& src);
      
      /** \brief Destructor
        */
      ~ServiceClient();
      
      /** \brief True, if this Prolog service client's services have
        *   been advertised and become available
        */ 
      bool exists() const;
      
      /** \brief True, if this Prolog service client is valid
        */ 
      bool isValid() const;
      
      /** \brief Wait for this Prolog service client's services to be
        *   advertised and available
        */
      bool waitForExistence(const ros::Duration& timeout =
        ros::Duration(-1));
      
      /** \brief Shutdown this Prolog service client
        */
      void shutdown();
      
    protected:
      friend class Client;
      friend class Query;
      
      /** \brief Prolog service client (implementation)
        */
      class Impl {
      public:
        Impl();
        ~Impl();

        bool exists() const;
        bool isValid() const;
        
        void shutdown();
        
        nodewrap::ServiceClient openQueryClient_;
        nodewrap::ServiceClient hasSolutionClient_;
        nodewrap::ServiceClient getAllSolutionsClient_;
        nodewrap::ServiceClient getNextSolutionClient_;
        nodewrap::ServiceClient closeQueryClient_;
      };
      
      /** \brief The Prolog service client's implementation
        */
      boost::shared_ptr<Impl> impl_;      
    };
  };
};

#endif
