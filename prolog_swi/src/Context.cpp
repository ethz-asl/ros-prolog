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

#include <SWI-Prolog.h>

#include "prolog_swi/Context.h"

namespace prolog { namespace swi {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Context::InvalidOperation::InvalidOperation(const std::string& description) :
  ros::Exception("Invalid operation on Prolog context: "+description) {
}

Context::ResourceError::ResourceError() :
  ros::Exception("Prolog context resource error.") {
}

Context::Context() :
  impl_(new Impl(SWIPL_EXECUTABLE, 256, 256, 256)) {
}

Context::Context(const Context& src) :
  impl_(src.impl_) {
}

Context::~Context() {
}

Context::Impl::Impl(const std::string& executable, size_t globalStack, size_t
    localStack, size_t trailStack) :
  executable_(executable),
  globalStack_(globalStack),
  localStack_(localStack),
  trailStack_(trailStack),
  argv_(0) {
}

Context::Impl::~Impl() {
  cleanup();
  
  if (argv_)
    delete argv_;
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void Context::setExecutable(const std::string& executable) {
  if (!PL_is_initialised(0, 0))
    impl_->executable_ = executable;
  else
    throw InvalidOperation("Initialized context is immutable.");
}

const std::string& Context::getExecutable() const {
  return impl_->executable_;
}

const std::string& Context::getVersion() const {
  return impl_->version_;
}

void Context::setGlobalStack(size_t stack) {
  if (!PL_is_initialised(0, 0))
    impl_->globalStack_ = stack;
  else
    throw InvalidOperation("Initialized context is immutable.");
}

size_t Context::getGlobalStack() const {
  return impl_->globalStack_;
}

void Context::setLocalStack(size_t stack) {
  if (!PL_is_initialised(0, 0))
    impl_->localStack_ = stack;
  else
    throw InvalidOperation("Initialized context is immutable.");
}

size_t Context::getLocalStack() const {
  return impl_->localStack_;
}

void Context::setTrailStack(size_t stack) {
  if (!PL_is_initialised(0, 0))
    impl_->trailStack_ = stack;
  else
    throw InvalidOperation("Initialized context is immutable.");
}

size_t Context::getTrailStack() const {
  return impl_->trailStack_;
}

bool Context::isInitialized() const {
  return PL_is_initialised(0, 0);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool Context::init() {
  return impl_->init();
}

bool Context::cleanup() {
  return impl_->cleanup();
}

Engine Context::createEngine(const std::string& name, size_t globalStack,
    size_t localStack, size_t trailStack) {
  Engine engine;
  
  if (PL_is_initialised(0, 0))
    engine.impl_.reset(new Engine::Impl(name, globalStack, localStack,
      trailStack));
      
  return engine;
}

bool Context::Impl::init() {
  if (PL_is_initialised(0, 0))
    return true;
  
  arguments_.clear();
  if (argv_) {
    delete argv_;
    argv_ = 0;
  }
    
  arguments_.push_back(executable_);
  
  arguments_.push_back("-G"+boost::lexical_cast<std::string>(
    globalStack_)+"M");
  arguments_.push_back("-L"+boost::lexical_cast<std::string>(
    localStack_)+"M");
  arguments_.push_back("-T"+boost::lexical_cast<std::string>(
    trailStack_)+"M");
  
  arguments_.push_back("--nosignals");
  arguments_.push_back("--nodebug");
  arguments_.push_back("--quiet");

  argv_ = new char*[arguments_.size()];
  
  for (size_t index = 0; index < arguments_.size(); ++index)
    argv_[index] = const_cast<char*>(arguments_[index].c_str());
  
  if (PL_initialise(arguments_.size(), argv_)) {
    size_t version = PL_query(PL_QUERY_VERSION);
    
    size_t major = version/10000;
    size_t minor = (version-major*10000)/100;
    size_t patch = version-major*10000-minor*100;
    
    version_ = boost::lexical_cast<std::string>(major)+"."+
      boost::lexical_cast<std::string>(minor)+"."+
      boost::lexical_cast<std::string>(patch);
      
    return true;
  }
  else {
    delete argv_;
    argv_ = 0;
    
    version_.clear();
    
    return false;
  }
}

bool Context::Impl::cleanup() {
  if (PL_is_initialised(0, 0)) {
    bool result = PL_cleanup(0);
    
    if (result) {
      if (argv_) {
        delete argv_;
        argv_ = 0;
      }
    }
    
    return result;
  }
  else
    return false;
}

}}
