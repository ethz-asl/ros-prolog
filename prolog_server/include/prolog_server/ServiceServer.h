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

/** \file ServiceServer.h
  * \brief Header file providing the ServiceServer class interface
  */

#ifndef ROS_PROLOG_SERVER_SERVICE_SERVER_H
#define ROS_PROLOG_SERVER_SERVICE_SERVER_H

#include <boost/shared_ptr.hpp>

#include <roscpp_nodewrap/ServiceServer.h>

namespace prolog {
  namespace server {
    /** \brief Prolog service server
      */
    class ServiceServer {
    public:
      /** \brief Default constructor
        */
      ServiceServer();
      
      /** \brief Copy constructor
        */
      ServiceServer(const ServiceServer& src);
      
      /** \brief Destructor
        */
      ~ServiceServer();
      
      /** \brief True, if this Prolog service server is valid
        */ 
      bool isValid() const;
      
      /** \brief Shutdown this Prolog service server
        */
      void shutdown();
      
    protected:
      friend class Server;
      
      /** \brief Prolog service server (implementation)
        */
      class Impl {
      public:
        Impl();
        ~Impl();

        bool isValid() const;
        
        void shutdown();
        
        nodewrap::ServiceServer openQueryServer_;
        nodewrap::ServiceServer getAllSolutionsServer_;
        nodewrap::ServiceServer getNextSolutionServer_;
        nodewrap::ServiceServer hasSolutionServer_;
        nodewrap::ServiceServer closeQueryServer_;
      };
      
      /** \brief The Prolog service server's implementation
        */
      boost::shared_ptr<Impl> impl_;      
    };
  };
};

#endif
