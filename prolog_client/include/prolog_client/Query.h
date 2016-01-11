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

#ifndef ROS_PROLOG_QUERY_H
#define ROS_PROLOG_QUERY_H

#include <list>

#include <boost/shared_ptr.hpp>

#include <prolog_common/Bindings.h>
#include <prolog_common/Term.h>

#include <prolog_client/ServiceClient.h>

namespace prolog {
  namespace client {
    class Solutions;
    
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
    
      /** \brief Default constructor
        */
      Query();
      
      /** \brief Constructor (overloaded version taking a goal string)
        */
      Query(const std::string& goal, Format format = PrologFormat);
      
      /** \brief Constructor (overloaded version taking a Prolog goal term)
        */
      Query(const Term& goal);
      
      /** \brief Copy constructor
        */
      Query(const Query& src);
      
      /** \brief Destructor
        */
      virtual ~Query();
      
      /** \brief Retrieve the query identifier of this Prolog query
        */
      std::string getIdentifier() const;
      
      /** \brief Retrieve the goal of this Prolog query
        */ 
      std::string getGoal() const;
      
      /** \brief Retrieve the error message of this Prolog query
        */ 
      std::string getError() const;
      
      /** \brief Retrieve the format of this Prolog query
        */ 
      Format getFormat() const;
      
      /** \brief Retrieve all solutions of this Prolog query
        */ 
      Solutions getAllSolutions() const;
      
      /** \brief Retrieve the incremental solutions of this Prolog query
        */ 
      Solutions getIncrementalSolutions() const;
      
      /** \brief True, if this Prolog query is valid
        */ 
      bool isValid() const;
    
      /** \brief True, if this Prolog query is empty
        */ 
      bool isEmpty() const;
      
      /** \brief True, if this Prolog query has at least one solution
        */ 
      bool hasSolution() const;
      
      /** \brief Start this Prolog query
        */ 
      bool start(ServiceClient& client, Mode mode = BatchMode);
      
      /** \brief Abort this Prolog query
        */ 
      bool abort();
      
      /** \brief Operator for retrieving the first solution of this
        *   Prolog query
        * 
        * This operator implicity starts this query in incremental mode,
        * requests a single solution, and then aborts it.
        */ 
      Bindings operator()(ServiceClient& client);
      
    protected:
      friend class ServiceClient;
      friend class Solutions;
      
      /** \brief Prolog client query (implementation)
        */
      class Impl {
      public:
        Impl();
        virtual ~Impl();
        
        bool start(ServiceClient& client, Mode mode);
        bool hasSolution();
        bool nextSolution(Bindings& bindings);
        bool allSolutions(std::list<Bindings>& bindings);
        bool abort();
      
        std::string identifier_;          
        std::string goal_;
        std::string error_;
        
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
