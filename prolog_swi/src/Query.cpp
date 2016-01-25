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

#include <SWI-Prolog.h>

#include <prolog_common/Atom.h>
#include <prolog_common/Compound.h>
#include <prolog_common/List.h>
#include <prolog_common/Variable.h>

#include <prolog_swi/Context.h>
#include <prolog_swi/Exception.h>
#include <prolog_swi/Term.h>

#include "prolog_swi/Query.h"

namespace prolog { namespace swi {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Query::Query() {
}

Query::Query(const std::string& goal) :
  impl_(new Impl("user", "call", {Atom(goal)})) {
}

Query::Query(const std::string& predicate, const std::vector<prolog::Term>&
    arguments) :
  impl_(new Impl("user", predicate, arguments)) {
}

Query::Query(const std::string& predicate, const std::initializer_list<
    prolog::Term>& arguments) :
  impl_(new Impl("user", predicate, arguments)) {
}

Query::Query(const std::string& module, const std::string& predicate, const
    std::vector<prolog::Term>& arguments) :
  impl_(new Impl(module, predicate, arguments)) {
}

Query::Query(const std::string& module, const std::string& predicate, const
    std::initializer_list<prolog::Term>& arguments) :
  impl_(new Impl(module, predicate, arguments)) {
}

Query::Query(const prolog::Query& query) {    
  if (query.isValid())
    impl_.reset(new Impl(query.getModule(), query.getPredicate(),
      query.getArguments()));
}

Query::Query(const Query& src) :
  impl_(src.impl_) {
}

Query::~Query() {
}

Query::Impl::Impl(const std::string& module, const std::string& predicate,
    const std::vector<prolog::Term>& arguments) :
  module_(module),
  predicate_(predicate),
  arguments_(arguments),
  moduleHandle_(0),
  predicateHandle_(0),
  argumentsHandle_(0),
  handle_(0) {
  BOOST_ASSERT(!predicate.empty());  
}

Query::Impl::~Impl() {
  close();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t Query::getArity() const {
  if (impl_.get())
    return (impl_->arguments_.size());
  else
    return 0;
}

bool Query::isOpen() const {
  if (impl_.get())
    return impl_->handle_;
  else
    return false;
}

bool Query::isValid() const {
  return impl_.get();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool Query::open() {
  if (impl_.get())
    return impl_->open();
  else
    return false;
}

bool Query::nextSolution(prolog::Bindings& bindings) {
  bindings.clear();
  
  if (impl_.get())
    return impl_->nextSolution(bindings);
  else
    return false;
}

void Query::cut() {
  if (impl_.get())
    impl_->cut();
}

void Query::close() {
  if (impl_.get())
    impl_->close();
}

bool Query::Impl::open() {
  if (!handle_) {
    if (!moduleHandle_) {
      atom_t moduleAtom;
      
      if (!module_.empty())
        moduleAtom = PL_new_atom(module_.c_str());
      else
        moduleAtom = PL_new_atom("user");
        
      moduleHandle_ = PL_new_module(moduleAtom);
      PL_unregister_atom(moduleAtom);
      
      if (!moduleHandle_)
        throw Context::ResourceError();
    }
      
    if (!predicateHandle_) {
      atom_t predicateAtom = PL_new_atom(predicate_.c_str());
      functor_t predicateFunctor = PL_new_functor(predicateAtom,
        arguments_.size());
      
      predicateHandle_ = PL_pred(predicateFunctor, moduleHandle_);
      PL_unregister_atom(predicateAtom);
      
      if (!predicateHandle_)
        throw Context::ResourceError();
    }
    
    if (!argumentsHandle_) {
      boost::unordered_map<std::string, std::string> mappings;
      
      if ((predicate_ == "call") && (arguments_.size() == 1) &&
          arguments_.front().isAtom()) {
        Atom atom = arguments_.front();
      
        predicate_t predicate = PL_predicate("atom_to_term", 3, "user");
      
        if (!predicate)
          throw Context::ResourceError();
      
        term_t arguments = PL_new_term_refs(3);
      
        if (!arguments)
          throw Context::ResourceError();
        
        PL_put_atom(arguments, PL_new_atom(atom.getName().c_str()));
        
        qid_t query = PL_open_query(NULL, PL_Q_CATCH_EXCEPTION,
          predicate, arguments);
        
        if (!query)
          throw Context::ResourceError();
        
        if (PL_next_solution(query)) {
          BOOST_ASSERT(PL_is_list(arguments+2));
          
          arguments_ = {Term(arguments+1)};
          List bindings = (prolog::Term)Term(arguments+2);

          for (std::list<prolog::Term>::const_iterator it = bindings.begin();
              it != bindings.end(); ++it) {
            BOOST_ASSERT(it->isCompound());

            Compound mapping(*it);
            
            BOOST_ASSERT(mapping.getFunctor() == "=");
            BOOST_ASSERT(mapping.getArity() == 2);
          
            prolog::Term name = *mapping.begin();
            prolog::Term variable = *(++mapping.begin());
          
            BOOST_ASSERT(name.isAtom());
            BOOST_ASSERT(variable.isVariable());
            
            mappings.insert(std::make_pair(Variable(variable).getName(),
              Atom(name).getName()));
          }
          
          PL_close_query(query);
        }
        else {
          Exception exception(PL_exception(query));
          
          PL_close_query(query);
          
          if (!exception.isEmpty())
            throw exception;
        }
      }
      
      argumentsHandle_ = PL_new_term_refs(arguments_.size());

      if (!argumentsHandle_)
        throw Context::ResourceError();
      
      size_t index = 0;
      for (std::vector<prolog::Term>::const_iterator it = arguments_.begin();
          it != arguments_.end(); ++it, ++index) {
        Term argument;
      
        argument.impl_.reset(new Term::Impl(*it));
      
        PL_put_term(argumentsHandle_+index, PL_copy_term_ref(
          argument.impl_->handle_));
        
        generateBindings(*it, argumentsHandle_+index, mappings);
      }
    }
  
    handle_ = PL_open_query(NULL, PL_Q_CATCH_EXCEPTION, predicateHandle_,
      argumentsHandle_);
  }
  
  return handle_;
}

bool Query::Impl::nextSolution(prolog::Bindings& bindings) {
  bindings.clear();
  
  if (handle_) {
    if (PL_next_solution(handle_)) {
      bindings = bindings_;
    
      return true;
    }
    else {
      Exception exception(PL_exception(handle_));
      
      if (!exception.isEmpty())
        throw exception;
    }
  }
  
  return false;
}

void Query::Impl::cut() {
  if (handle_) {
    PL_cut_query(handle_);
    
    bindings_.clear();
    
    handle_ = 0;
  }
}

void Query::Impl::close() {
  if (handle_) {
    PL_close_query(handle_);
    
    bindings_.clear();
    
    handle_ = 0;
  }
}

void Query::Impl::generateBindings(const prolog::Term& argument, unsigned long
    handle, const boost::unordered_map<std::string, std::string>& mappings) {
  if (argument.isValid() && handle) {
    if (argument.isList()) {
      BOOST_ASSERT(PL_is_list(handle));
      
      List list(argument);
      
      term_t listHandle = PL_copy_term_ref(handle);

      for (std::list<prolog::Term>::const_iterator it = list.begin();
          it != list.end(); ++it) {
        term_t head = PL_new_term_ref();
      
        BOOST_ASSERT(PL_get_list(listHandle, head, listHandle));
        
        generateBindings(*it, head, mappings);
      }
    }
    else if (argument.isCompound()) {
      BOOST_ASSERT(PL_is_compound(handle));

      Compound compound(argument);
      
      atom_t functor;
      int arity;

      BOOST_ASSERT(PL_get_name_arity(handle, &functor, &arity));
      BOOST_ASSERT(compound.getFunctor() == PL_atom_chars(functor));
      BOOST_ASSERT(compound.getArity() == arity);
      
      size_t index = 0;
      
      for (std::vector<prolog::Term>::const_iterator it = compound.begin();
          it != compound.end(); ++it, ++index) {
        term_t argument = PL_new_term_ref();
      
        BOOST_ASSERT(PL_get_arg(index+1, handle, argument));
      
        generateBindings(*it, argument, mappings);
      }
    }
    else if (argument.isVariable()) {
      Variable variable(argument);
      
      boost::unordered_map<std::string, std::string>::const_iterator
        it = mappings.find(variable.getName());
      
      if (it != mappings.end())
        bindings_.addTerm(it->second, Term(handle));
      else
        bindings_.addTerm(variable.getName(), Term(handle));
    }
  }
}

}}
