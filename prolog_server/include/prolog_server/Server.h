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
  * \brief Header file providing the Server class interface
  */

#ifndef ROS_PROLOG_SERVER_H
#define ROS_PROLOG_SERVER_H

#include <boost/unordered_map.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <roscpp_nodewrap/NodeImpl.h>

#include <roscpp_nodewrap/worker/Worker.h>

#include <prolog_msgs/CloseQuery.h>
#include <prolog_msgs/GetAllSolutions.h>
#include <prolog_msgs/GetNextSolution.h>
#include <prolog_msgs/HasSolution.h>
#include <prolog_msgs/OpenQuery.h>

#include <prolog_swi/Context.h>

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
    
      /** \brief True, if Prolog has been initialized
        */
      bool isPrologInitialized() const;
      
    protected:
      /** \brief Advertise a Prolog service
        */
      ServiceServer advertisePrologService(const std::string& name, const
        std::string& defaultServiceNamespace = std::string());
      
      /** \brief Create a Prolog engine
        */
      swi::Engine createPrologEngine(const std::string& name, size_t
        defaultGlobalStack = 256, size_t defaultLocalStack = 256, size_t
        defaultTrailStack = 256);
      
      /** \brief Generate a Prolog query identifier
        */ 
      std::string prologQueryIdentifier();
      
      /** \brief Initialize the Prolog server
        */
      void init();
      
      /** \brief Perform Prolog initialization
        */
      virtual void initProlog();
      
      /** \brief Cleanup the Prolog server
        */
      void cleanup();

      /** \brief Perform Prolog cleanup
        */
      virtual void cleanupProlog();
      
      /** \brief Open query service callback (abstract declaration)
        */
      virtual bool openQueryCallback(prolog_msgs::OpenQuery::Request&
        request, prolog_msgs::OpenQuery::Response& response) = 0;
      
      /** \brief Has solution service callback (abstract declaration)
        */
      virtual bool hasSolutionCallback(prolog_msgs::HasSolution::Request&
        request, prolog_msgs::HasSolution::Response& response) = 0;
      
      /** \brief Get all solutions service callback (abstract declaration)
        */
      virtual bool getAllSolutionsCallback(prolog_msgs::GetAllSolutions::
        Request& request, prolog_msgs::GetAllSolutions::Response&
        response) = 0;
      
      /** \brief Get next solution service callback (abstract declaration)
        */
      virtual bool getNextSolutionCallback(prolog_msgs::GetNextSolution::
        Request& request, prolog_msgs::GetNextSolution::Response&
        response) = 0;
      
      /** \brief Close query service callback (abstract declaration)
        */
      virtual bool closeQueryCallback(prolog_msgs::CloseQuery::Request&
        request, prolog_msgs::CloseQuery::Response& response) = 0;
      
    private:        
      /** \brief The Prolog context of this Prolog server
        */
      swi::Context context_;
      
      /** \brief The query identifier generator of this Prolog server
        */
      boost::uuids::random_generator queryIdentifierGenerator_;
    };
  };
};

#endif
