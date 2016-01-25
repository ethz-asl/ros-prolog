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

/** \file QueryProxy.h
  * \brief Header file providing the QueryProxy class interface
  */

#ifndef ROS_PROLOG_CLIENT_QUERY_PROXY_H
#define ROS_PROLOG_CLIENT_QUERY_PROXY_H

#include <list>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/shared_ptr.hpp>

#include <ros/exception.h>

#include <prolog_common/Solution.h>

#include <prolog_client/Query.h>

namespace prolog {
  namespace client {
    /** \brief Prolog query proxy
      */
    class QueryProxy {
    protected:
      /** \brief Forward declartion of the implementation
        */
      class Impl;
      
    public:
      /** \brief Exception thrown in case of an attempted invalid operation
        */ 
      class InvalidOperation :
        public ros::Exception {
      public:
        InvalidOperation(const std::string& description);
      };
      
      /** \brief Prolog query proxy iterator
        * 
        * This iterator can be used to iterate the solutions of the
        * Prolog query linked to a query proxy.
        */
      class Iterator :
        public boost::iterator_facade<Iterator, Solution,
          boost::single_pass_traversal_tag> {
      public:
        /** \brief Default constructor
          */
        Iterator();
        
        /** \brief Copy constructor
          */
        Iterator(const Iterator& src);
        
        /** \brief Destructor
          */
        ~Iterator();
              
      private:
        friend class boost::iterator_core_access;
        
        friend class QueryProxy;
        
        /** \brief True, if this iterator equals another iterator
          */
        bool equal(const Iterator& iterator) const;
        
        /** \brief Increment this iterator
          */
        void increment();
        
        /** \brief Dereference this iterator
          */
        Solution& dereference() const;
        
        /** \brief The Prolog query proxy this iterator operates on
          */
        boost::shared_ptr<Impl> proxy_;
        
        /** \brief The solution list iterator of this Prolog solutions
          *   iterator
          */
        std::list<Solution>::iterator iterator_;
      };
      
      /** \brief Default constructor
        */
      QueryProxy();
      
      /** \brief Copy constructor
        */
      QueryProxy(const QueryProxy& src);
      
      /** \brief Destructor
        */
      virtual ~QueryProxy();
      
      /** \brief True, if this Prolog query proxy provides at least
        *   one solution
        */ 
      bool hasSolution() const;
      
      /** \brief True, if this Prolog query proxy is valid
        */ 
      bool isValid() const;
      
      /** \brief Retrieve the solution begin iterator provided by this
        *   Prolog query proxy
        */ 
      Iterator begin();
      
      /** \brief Retrieve the solution end iterator provided by this
        *   Prolog query proxy
        */ 
      Iterator end();
      
    protected:
      friend class Iterator;
      friend class Query;
      
      /** \brief Prolog query proxy (implementation)
        */
      class Impl {
      public:
        Impl(const Query& query);
        virtual ~Impl();
        
        Query query_;
        
        std::list<Solution> solutions_;
      };
      
      /** \brief The Prolog query proxy's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
