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

/** \file Engine.h
  * \brief Header file providing the Engine class interface
  */

#ifndef ROS_PROLOG_SWI_ENGINE_H
#define ROS_PROLOG_SWI_ENGINE_H

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <ros/exception.h>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog engine
      */    
    class Engine {
    private:
      /** \brief Forward declaration of the SWI-Prolog engine implementation
        */
      class Impl;
      
    public:
      /** \brief Exception thrown in case of an error to acquire a SWI-Prolog
        *   engine
        */ 
      class AcquisitionError :
        public ros::Exception {
      public:
        AcquisitionError(const std::string& description);
      };
      
      /** \brief Exception thrown in case of an error to release a SWI-Prolog
        *   engine
        */ 
      class ReleaseError :
        public ros::Exception {
      public:
        ReleaseError(const std::string& description);
      };
      
      /** \brief Scoped acquisition for the SWI-Prolog engine
        */ 
      class ScopedAcquisition {
      public:
        /** \brief Constructor
          */
        ScopedAcquisition(const Engine& engine);
        
        /** \brief Destructor
          */
        ~ScopedAcquisition();
        
      private:
        /** \brief The acquired SWI-Prolog engine
          */
        boost::shared_ptr<Impl> engine_;
        
        /** \brief Copy constructor (forbidden)
          */
        ScopedAcquisition(const ScopedAcquisition& src);
      };
      
      /** \brief Default constructor
        */
      Engine();
      
      /** \brief Copy constructor
        */
      Engine(const Engine& src);
      
      /** \brief Destructor
        */
      virtual ~Engine();
      
      /** \brief Retrieve the name of this SWI-Prolog engine
        */
      std::string getName() const;
          
      /** \brief Retrieve the global stack size of this SWI-Prolog
        *   engine in [MB]
        */
      size_t getGlobalStack() const;
          
      /** \brief Retrieve the local stack size of this SWI-Prolog
        *   engine in [MB]
        */
      size_t getLocalStack() const;
          
      /** \brief Retrieve the trail stack size of this SWI-Prolog
        *   engine in [MB]
        */
      size_t getTrailStack() const;
      
      /** \brief True, if this SWI-Prolog engine is aquired
        */
      bool isAcquired() const;
          
      /** \brief True, if this SWI-Prolog engine is valid
        */
      bool isValid() const;
          
      /** \brief Acquire this SWI-Prolog engine
        * 
        * This method attaches the SWI-Prolog engine to the calling thread.
        */
      void acquire();
      
      /** \brief Release this SWI-Prolog engine
        * 
        * This method releases the SWI-Prolog engine from the calling thread.
        */
      void release();
      
      /** \brief Shutdown this SWI-Prolog engine
        */
      void shutdown();
          
    private:
      friend class Context;
      
      /** \brief SWI-Prolog engine (implementation)
        */
      class Impl {
      public:
        Impl(const std::string& name, size_t globalStack = 0, size_t
          localStack = 0, size_t trailStack = 0);
        virtual ~Impl();
        
        bool isValid() const;
        
        void acquire();
        void release();
        void shutdown();
        
        std::string name_;
        
        size_t globalStack_;
        size_t localStack_;
        size_t trailStack_;
        
        void* engine_;
        
        bool acquired_;
        
        boost::mutex mutex_;
      };
      
      /** \brief The SWI-Prolog engine's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
