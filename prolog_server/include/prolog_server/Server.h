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

/** \file Server.h
  * \brief Header file providing the example Server class interface
  */

#ifndef ROS_PROLOG_SERVER_H
#define ROS_PROLOG_SERVER_H

#include <boost/uuid/uuid_generators.hpp>

#include <roscpp_nodewrap/NodeImpl.h>
#include <roscpp_nodewrap/Nodelet.h>

#include <prolog_msgs/AbortQuery.h>
#include <prolog_msgs/GetAllSolutions.h>
#include <prolog_msgs/GetNextSolution.h>
#include <prolog_msgs/HasSolution.h>
#include <prolog_msgs/StartQuery.h>

#include <prolog_server/ServiceServer.h>

namespace prolog {
  namespace server {
    /** \brief Prolog server implementation
      */  
    class Server :
      public nodewrap::NodeImpl {
    public:
      /** \brief Default constructor
        */
      Server();
      
      /** \brief Destructor
        */
      virtual ~Server();
    
    protected:
      /** \brief Advertise a Prolog service
        */
      ServiceServer advertisePrologService(const std::string& name, const
        std::string& defaultServiceNamespace = std::string());
      
      /** \brief Initialize the Prolog server
        */
      void init();
      
      /** \brief Cleanup the Prolog server
        */
      void cleanup();

    private:
      /** \brief The Prolog service server of this Prolog server
        */
      ServiceServer serviceServer_;
      
      /** \brief The query identifier generator of this Prolog server
        */
      boost::uuids::random_generator queryIdentifierGenerator_;
      
      /** \brief Start query service callback
        */
      bool startQueryCallback(prolog_msgs::StartQuery::Request& request,
        prolog_msgs::StartQuery::Response& response);
      
      /** \brief Has solution service callback
        */
      bool hasSolutionCallback(prolog_msgs::HasSolution::Request&
        request, prolog_msgs::HasSolution::Response& response);
      
      /** \brief Get all solutions service callback
        */
      bool getAllSolutionsCallback(prolog_msgs::GetAllSolutions::Request&
        request, prolog_msgs::GetAllSolutions::Response& response);
      
      /** \brief Get next solution service callback
        */
      bool getNextSolutionCallback(prolog_msgs::GetNextSolution::Request&
        request, prolog_msgs::GetNextSolution::Response& response);
      
      /** \brief Abort query service callback
        */
      bool abortQueryCallback(prolog_msgs::AbortQuery::Request& request,
        prolog_msgs::AbortQuery::Response& response);
      
      /** \brief Generate a Prolog query identifier
        */ 
      std::string generateQueryIdentifier();
    };
  };
};

#endif
