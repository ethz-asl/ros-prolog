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

/** \file Context.h
  * \brief Header file providing the example Context class interface
  */

#ifndef ROS_PROLOG_SWI_CONTEXT_H
#define ROS_PROLOG_SWI_CONTEXT_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <ros/exception.h>

#include <prolog_swi/Engine.h>

namespace prolog {
  namespace swi {
    /** \brief SWI-Prolog context
      */  
    class Context {
    public:
      /** \brief Exception thrown in case of an invalid operation on
        *   a SWI-Prolog context
        */ 
      class InvalidOperation :
        public ros::Exception {
      public:
        InvalidOperation(const std::string& description);
      };
      
      /** \brief Exception thrown in case of a resource error in a
        *   SWI-Prolog context
        */ 
      class ResourceError :
        public ros::Exception {
      public:
        ResourceError();
      };
      
      /** \brief Default constructor
        */
      Context();
      
      /** \brief Copy constructor
        */
      Context(const Context& src);
      
      /** \brief Destructor
        */
      virtual ~Context();
    
      /** \brief Set the Prolog executable path of this SWI-Prolog
        *   context
        */
      void setExecutable(const std::string& executable);
          
      /** \brief Retrieve the Prolog executable path of this SWI-Prolog
        *   context
        */
      const std::string& getExecutable() const;
          
      /** \brief Retrieve Prolog version of this SWI-Prolog context
        */
      const std::string& getVersion() const;
          
      /** \brief Set the global stack size of this SWI-Prolog
        *   context in [MB]
        */
      void setGlobalStack(size_t stack);
      
      /** \brief Retrieve the global stack size of this SWI-Prolog
        *   context in [MB]
        */
      size_t getGlobalStack() const;
          
      /** \brief Set the local stack size of this SWI-Prolog
        *   context in [MB]
        */
      void setLocalStack(size_t stack);
      
      /** \brief Retrieve the local stack size of this SWI-Prolog
        *   context in [MB]
        */
      size_t getLocalStack() const;
          
      /** \brief Set the trail stack size of this SWI-Prolog
        *   context in [MB]
        */
      void setTrailStack(size_t stack);
      
      /** \brief Retrieve the trail stack size of this SWI-Prolog
        *   context in [MB]
        */
      size_t getTrailStack() const;
      
      /** \brief True, if this SWI-Prolog context has been initialized
        */
      bool isInitialized() const;
      
      /** \brief Initialize this SWI-Prolog context
        */
      bool init();
      
      /** \brief Cleanup this SWI-Prolog context
        */
      bool cleanup();
      
      /** \brief Create a SWI-Prolog engine for this SWI-Prolog context
        */
      Engine createEngine(const std::string& name, size_t globalStack = 0,
        size_t localStack = 0, size_t trailStack = 0);
      
    private:
      /** \brief SWI-Prolog context (implementation)
        */
      class Impl {
      public:
        Impl(const std::string& executable, size_t globalStack, size_t
          localStack, size_t trailStack);
        virtual ~Impl();
        
        bool init();
        bool cleanup();
        
        std::string executable_;
        std::string version_;
        
        size_t globalStack_;
        size_t localStack_;
        size_t trailStack_;

        std::vector<std::string> arguments_;
        char** argv_;
      };
      
      /** \brief The SWI-Prolog context's implementation
        */
      boost::shared_ptr<Impl> impl_;
    };
  };
};

#endif
