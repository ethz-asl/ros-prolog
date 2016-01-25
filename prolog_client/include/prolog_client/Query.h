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

/** \file Query.h
  * \brief Header file providing the Query class interface
  */

#ifndef ROS_PROLOG_CLIENT_QUERY_H
#define ROS_PROLOG_CLIENT_QUERY_H

#include <list>

#include <boost/shared_ptr.hpp>

#include <ros/exception.h>

#include <prolog_common/Query.h>
#include <prolog_common/Solution.h>

#include <prolog_client/ServiceClient.h>

namespace prolog {
  namespace client {
    class QueryProxy;
    
    /** \brief Prolog query
      */
    class Query {
    public:
      /** \brief Definition of the Prolog query mode enumerable type
        */
      enum Mode {
        BatchMode,
        IncrementalMode
      };
      
      /** \brief Definition of the Prolog query format enumerable type
        */
      enum Format {
        PrologFormat,
        JSONFormat
      };
    
      /** \brief Exception thrown in case of an attempted invalid operation
        */ 
      class InvalidOperation :
        public ros::Exception {
      public:
        InvalidOperation(const std::string& description);
      };
      
      /** \brief Exception thrown in case of a failure to contact the
        *   Prolog service server
        */ 
      class NoSuchService :
        public ros::Exception {
      public:
        NoSuchService(const std::string& service);
      };
      
      /** \brief Exception thrown in case of a failed service call
        */ 
      class ServiceCallFailed :
        public ros::Exception {
      public:
        ServiceCallFailed(const std::string& service);
      };
      
      /** \brief Exception thrown in case of the service server reporting
        *   an invalid query identifier
        */ 
      class InvalidIdentifier :
        public ros::Exception {
      public:
        InvalidIdentifier();
        InvalidIdentifier(const std::string& identifier);
      };
      
      /** \brief Exception thrown in case of the service server reporting
        *   a failed query
        */ 
      class QueryFailed :
        public ros::Exception {
      public:
        QueryFailed(const std::string& description);
      };
      
      /** \brief Exception thrown in case of a failure to deserialize
        *   the solution
        */ 
      class DeserializationFailed :
        public ros::Exception {
      public:
        DeserializationFailed(const std::string& description);
      };
      
      /** \brief Exception thrown in case of an unknown status response from
        *   the Prolog service server
        */ 
      class UnknownResponse :
        public ros::Exception {
      public:
        UnknownResponse(int status);
      };
      
      /** \brief Default constructor
        */
      Query();
      
      /** \brief Constructor (overloaded version taking a goal term in
        *   Prolog syntax)
        */
      Query(const std::string& goal);
      
      /** \brief Constructor (overloaded version taking a Prolog query)
        */
      Query(const prolog::Query& query);
      
      /** \brief Copy constructor
        */
      Query(const Query& src);
      
      /** \brief Destructor
        */
      virtual ~Query();
      
      /** \brief Retrieve the query identifier of this Prolog query
        */
      std::string getIdentifier() const;
      
      /** \brief Retrieve the format of this Prolog query
        */ 
      Format getFormat() const;
      
      /** \brief Retrieve the next solution of this Prolog query
        */ 
      Solution getNextSolution(bool close = false) const;
      
      /** \brief Retrieve all solutions of this Prolog query
        */ 
      std::list<Solution> getAllSolutions() const;
      
      /** \brief Retrieve a proxy for this Prolog query
        */ 
      QueryProxy getProxy() const;
      
      /** \brief True, if this Prolog query is valid
        */ 
      bool isValid() const;
    
      /** \brief True, if this Prolog query is open
        */ 
      bool isOpen() const;
      
      /** \brief True, if this Prolog query is empty
        */ 
      bool isEmpty() const;
      
      /** \brief True, if this Prolog query has at least one solution
        */ 
      bool hasSolution() const;
      
      /** \brief Open this Prolog query
        */ 
      void open(ServiceClient& client, Mode mode = BatchMode);
      
      /** \brief Close this Prolog query
        */ 
      void close();

      /** \brief Retrieve the first solution of this Prolog query
        * 
        * This method implicity opens this query in incremental mode,
        * requests its first solution, and then closes it.
        */ 
      Solution once(ServiceClient& client);
      
      /** \brief Retrieve all solutions of this Prolog query
        * 
        * This method implicity opens this query in batch mode,
        * requests all of its solutions, and then closes it.
        */ 
      std::list<Solution> all(ServiceClient& client);
      
      /** \brief Incrementally retrieve all solutions of this Prolog query
        * 
        * This method implicity opens this query in incremental mode and
        * returns a query proxy which can be used to iterate its solutions.
        * 
        * \note This query will automatically be closed once the query
        *   proxy goes out of scope and is destroyed.
        */ 
      QueryProxy incremental(ServiceClient& client);
      
    protected:
      friend class QueryProxy;
      friend class ServiceClient;
      
      /** \brief Prolog client query (implementation)
        */
      class Impl {
      public:
        Impl();
        virtual ~Impl();
        
        Solution getNextSolution(bool close);
        std::list<Solution> getAllSolutions();
        bool hasSolution();
        
        void open(ServiceClient& client, Mode mode);
        void close();
      
        std::string identifier_;
        
        std::string query_;
        Mode mode_;
        Format format_;
        
        ServiceClient client_;
      };
    
      /** \brief The Prolog query's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
