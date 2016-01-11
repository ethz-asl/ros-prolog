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

/** \file Solutions.h
  * \brief Header file providing the Solutions class interface
  */

#ifndef ROS_PROLOG_SOLUTIONS_H
#define ROS_PROLOG_SOLUTIONS_H

#include <list>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/shared_ptr.hpp>

#include <ros/exception.h>

#include <prolog_common/Bindings.h>

#include <prolog_client/Query.h>

namespace prolog {
  namespace client {
    /** \brief Prolog solutions
      */
    class Solutions {
    protected:
      /** \brief Forward declartion of the implementation
        */
      class Impl;
      
    public:
      /** \brief Exception thrown in case of an iterator increment on
        *   an end iterator
        */ 
      class StopIteration :
        public ros::Exception {
      public:
        StopIteration();
      };
      
      /** \brief Prolog solutions iterator
        */
      class Iterator :
        public boost::iterator_facade<Iterator, Bindings,
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
        
        friend class Solutions;
        
        /** \brief True, if this iterator equals another iterator
          */
        bool equal(const Iterator& iterator) const;
        
        /** \brief Increment this iterator
          */
        void increment();
        
        /** \brief Dereference this iterator
          */
        Bindings& dereference() const;
        
        /** \brief The Prolog solutions this iterator operates on
          */
        boost::shared_ptr<Impl> solutions_;
        
        /** \brief The bindings list iterator of this Prolog solutions
          *   iterator
          */
        std::list<Bindings>::iterator iterator_;
      };
      
      /** \brief Default constructor
        */
      Solutions();
      
      /** \brief Copy constructor
        */
      Solutions(const Solutions& src);
      
      /** \brief Destructor
        */
      virtual ~Solutions();
      
      /** \brief True, if these Prolog solutions are empty
        */ 
      bool areEmpty() const;
      
      /** \brief Retrieve the begin iterator of these Prolog solutions
        */ 
      Iterator begin();
      
      /** \brief Retrieve the end iterator of these Prolog solutions
        */
      Iterator end();
      
      /** \brief Clear these Prolog solutions
        */ 
      void clear();
      
    protected:
      friend class Iterator;
      friend class Query;
      
      /** \brief Prolog solutions (implementation)
        */
      class Impl {
      public:
        Impl();
        virtual ~Impl();
        
        std::list<Bindings> bindings_;
        
        boost::shared_ptr<Query::Impl> query_;
      };
      
      /** \brief The Prolog solutions' implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
