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

#include <boost/lexical_cast.hpp>
#include <boost/thread/locks.hpp>

#include <ros/console.h>

#include <SWI-Prolog.h>

#include "prolog_swi/Engine.h"

namespace prolog { namespace swi {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Engine::AcquisitionError::AcquisitionError(const std::string& description) :
  ros::Exception("Failure to acquire engine: "+description) {
}

Engine::ReleaseError::ReleaseError(const std::string& description) :
  ros::Exception("Failure to release engine: "+description) {
}

Engine::ScopedAcquisition::ScopedAcquisition(const Engine& engine) :
  engine_(engine.impl_) {
  if (engine_.get())
    engine_->acquire();
  else
    throw AcquisitionError("Engine is invalid.");
}

Engine::ScopedAcquisition::ScopedAcquisition(const ScopedAcquisition& src) :
  engine_(src.engine_) {
}

Engine::ScopedAcquisition::~ScopedAcquisition() {
  if (engine_.get())
    engine_->release();
  else
    throw ReleaseError("Engine is invalid.");
}

Engine::Engine() {
}

Engine::Engine(const Engine& src) :
  impl_(src.impl_) {
}

Engine::~Engine() {  
}

Engine::Impl::Impl(const std::string& name, size_t globalStack, size_t
    localStack, size_t trailStack) :
  name_(name),
  globalStack_(globalStack),
  localStack_(localStack),
  trailStack_(trailStack),
  engine_(0),
  acquired_(false) {
  PL_thread_attr_t attributes;
  
  attributes.local_size = localStack_;
  attributes.global_size = globalStack_;
  attributes.trail_size = trailStack_;
  attributes.argument_size = 0;
  attributes.alias = 0;
  attributes.cancel = 0;
  attributes.flags = 0;
    
  engine_ = PL_create_engine(&attributes);
  
  if (engine_)
    ROS_INFO_STREAM("Prolog engine [" << name_ << "] has been created.");
  else
    ROS_ERROR_STREAM("Failure to create Prolog engine [" << name_ << "].");
}

Engine::Impl::~Impl() {
  shutdown();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string Engine::getName() const {
  if (impl_.get())
    return impl_->name_;
  else
    return std::string();
}

size_t Engine::getGlobalStack() const {
  if (impl_.get())
    return impl_->globalStack_;
  else
    return 0;
}

size_t Engine::getLocalStack() const {
  if (impl_.get())
    return impl_->localStack_;
  else
    return 0;
}

size_t Engine::getTrailStack() const {
  if (impl_.get())
    return impl_->trailStack_;
  else
    return 0;
}

bool Engine::isAcquired() const {
  if (impl_.get()) {
    boost::mutex::scoped_lock lock(impl_->mutex_);
  
    return impl_->acquired_;
  }
  else
    return false;
}

bool Engine::isValid() const {
  if (impl_.get())
    return impl_->isValid();
  else
    return false;
}

bool Engine::Impl::isValid() const {
  return engine_;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Engine::acquire() {
  if (impl_.get())
    impl_->acquire();
  else
    throw AcquisitionError("Engine is invalid.");
}

void Engine::release() {
  if (impl_.get())
    impl_->release();
  else
    throw ReleaseError("Engine is invalid.");
}

void Engine::shutdown() {
  if (impl_.get())
    impl_->shutdown();
}

void Engine::Impl::acquire() {
  if (engine_) {
    boost::mutex::scoped_lock lock(mutex_);
    
    int result = PL_set_engine(engine_, 0);
    
    if (result != PL_ENGINE_SET) {
      if (result == PL_ENGINE_INVAL)
        throw AcquisitionError("Engine is invalid.");
      else if (result == PL_ENGINE_INUSE)
        throw AcquisitionError("Engine is currently in use.");
      else
        throw AcquisitionError("Unknown response: "+
          boost::lexical_cast<std::string>(result));
    }
    else
      acquired_ = true;
  }
  else
    throw AcquisitionError("Engine is invalid.");
}

void Engine::Impl::release() {
  if (engine_) {
    boost::mutex::scoped_lock lock(mutex_);
    
    PL_engine_t engine;
    
    PL_set_engine(PL_ENGINE_CURRENT, &engine);
    
    if (engine == engine_) {
      PL_set_engine(0, 0);
      
      acquired_ = false;
    }
    else
      throw ReleaseError("Engine has not been acquired by the caller.");    
  }
  else
    throw ReleaseError("Engine is invalid.");
}

void Engine::Impl::shutdown() {
  if (engine_ && PL_destroy_engine(engine_)) {
    engine_ = 0;
    acquired_ = false;
    
    ROS_INFO_STREAM("Prolog engine [" << name_ << "] has been destroyed.");
  }
}

}}
