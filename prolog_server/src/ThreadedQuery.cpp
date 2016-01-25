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

#include <boost/thread/locks.hpp>

#include <ros/console.h>

#include <prolog_swi/Frame.h>

#include "prolog_server/ThreadedQuery.h"

namespace prolog { namespace server {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

ThreadedQuery::ThreadedQuery() {
}

ThreadedQuery::ThreadedQuery(const ThreadedQuery& src) :
  impl_(src.impl_) {
}

ThreadedQuery::~ThreadedQuery() {
}

ThreadedQuery::Impl::Impl(const swi::Query& query, const swi::Engine& engine,
    Mode mode) :
  query_(query),
  engine_(engine),
  mode_(mode) {
}

ThreadedQuery::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string ThreadedQuery::getError() const {
  if (impl_.get()) {
    boost::mutex::scoped_lock lock(impl_->mutex_);
    
    return impl_->error_;
  }
  else
    return std::string();
}

bool ThreadedQuery::getNextSolution(Bindings& bindings, std::string& error,
    bool block) const {
  if (impl_.get()) {
    boost::mutex::scoped_lock lock(impl_->mutex_);
    
    if (impl_->solutions_.empty() && impl_->query_.isOpen() && block)
      impl_->condition_.wait(impl_->mutex_);
    
    if (!impl_->solutions_.empty()) {
      bindings = impl_->solutions_.front();

      impl_->solutions_.pop_front();
      impl_->condition_.notify_all();
      
      return true;
    }
    else {
      error = impl_->error_;
      
      return false;
    }
  }
  else
    return false;
}

bool ThreadedQuery::hasSolution(std::string& error, bool block) const {
  if (impl_.get()) {
    boost::mutex::scoped_lock lock(impl_->mutex_);
    
    if (impl_->solutions_.empty() && impl_->query_.isOpen() && block)
      impl_->condition_.wait(impl_->mutex_);
    
    if (impl_->solutions_.empty()) {
      error = impl_->error_;
      
      return false;
    }
    else
      return true;
  }
  else
    return false;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool ThreadedQuery::Impl::execute(const nodewrap::WorkerEvent& event) {  
  boost::mutex::scoped_lock lock(mutex_);
    
  if (!query_.isValid()) {
    error_ = "ThreadedQuery is invalid.";
    ROS_ERROR_STREAM(error_);
    
    return false;
  }

  boost::shared_ptr<swi::Engine::ScopedAcquisition> acquisition;
  
  try {
    acquisition.reset(new swi::Engine::ScopedAcquisition(engine_));
  }
  catch (const ros::Exception& exception) {
    error_ = exception.what();
    ROS_ERROR_STREAM(error_);
    
    return false;
  }  
  
  swi::Frame frame;
  
  if (!frame.open()) {
    error_ = "Failure to open foreign frame.";
    ROS_ERROR_STREAM(error_);
    
    return false;
  }
  
  try {
    query_.open();
  }
  catch (ros::Exception& exception) {
    error_ = std::string("Failure to open query: ")+
      exception.what();
    ROS_ERROR_STREAM(error_);
    
    return false;
  }
  
  bool result = true;
  
  while (result && !event.isWorkerCanceled()) {
    Bindings bindings;
    
    try {
      mutex_.unlock();
      
      result = query_.nextSolution(bindings);
      
      mutex_.lock();
    }
    catch (ros::Exception& exception) {
      mutex_.lock();
      
      error_ = std::string("Failure to generate solution: ")+
        exception.what();
      ROS_ERROR_STREAM(error_);

      query_.close();
      condition_.notify_all();
      
      return false;
    }
    
    if (result) {
      solutions_.push_back(bindings);
      
      condition_.notify_all();

      if (mode_ == IncrementalMode) {
        while (!condition_.timed_wait(mutex_, boost::posix_time::
            milliseconds(25))) {
          if (event.isWorkerCanceled()) {
            query_.close();
            condition_.notify_all();
            
            return false;
          }
        }
      }
    }    
  }
  
  query_.close();
  condition_.notify_all();
  
  return false;
}

}}
