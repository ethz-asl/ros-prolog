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

/** \file MultiThreadedServer.h
  * \brief Header file providing the MultiThreadedServer class interface
  */

#ifndef ROS_PROLOG_SERVER_MULTI_THREADED_SERVER_H
#define ROS_PROLOG_SERVER_MULTI_THREADED_SERVER_H

#include <list>

#include <roscpp_nodewrap/worker/Worker.h>

#include <prolog_swi/Engine.h>

#include <prolog_server/Server.h>
#include <prolog_server/ThreadedQuery.h>

#include <roscpp_nodewrap/Nodelet.h>

namespace prolog {
  namespace server {
    /** \brief Multi-threaded Prolog server implementation
      */  
    class MultiThreadedServer :
      public Server {
    public:
      /** \brief Default constructor
        */
      MultiThreadedServer();
      
      /** \brief Destructor
        */
      virtual ~MultiThreadedServer();
    
    protected:
      /** \brief Initialize the multi-threaded Prolog server
        */
      void init();
      
      /** \brief Cleanup the multi-threaded Prolog server
        */
      void cleanup();

      /** \brief Open query service callback (implementation)
        */
      bool openQueryCallback(prolog_msgs::OpenQuery::Request& request,
        prolog_msgs::OpenQuery::Response& response);
      
      /** \brief Has solution service callback (implementation)
        */
      bool hasSolutionCallback(prolog_msgs::HasSolution::Request&
        request, prolog_msgs::HasSolution::Response& response);
      
      /** \brief Get all solutions service callback (implementation)
        */
      bool getAllSolutionsCallback(prolog_msgs::GetAllSolutions::Request&
        request, prolog_msgs::GetAllSolutions::Response& response);
      
      /** \brief Get next solution service callback (implementation)
        */
      bool getNextSolutionCallback(prolog_msgs::GetNextSolution::Request&
        request, prolog_msgs::GetNextSolution::Response& response);
      
      /** \brief Close query service callback (implementation)
        */
      bool closeQueryCallback(prolog_msgs::CloseQuery::Request& request,
        prolog_msgs::CloseQuery::Response& response);
    
    private:      
      /** \brief The Prolog service server of this multi-threaded Prolog
        *   server
        */
      ServiceServer serviceServer_;
      
      /** \brief The Prolog engines of this multi-threaded Prolog server
        */
      std::list<swi::Engine> engines_;
      
      /** \brief The Prolog queries of this multi-threaded Prolog server
        */
      boost::unordered_map<std::string, ThreadedQuery> queries_;
      
      /** \brief The workers of this multi-threaded Prolog server
        */
      boost::unordered_map<std::string, nodewrap::Worker> workers_;
      
    };
  };
};

#endif
