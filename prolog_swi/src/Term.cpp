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
#include <prolog_common/Float.h>
#include <prolog_common/Integer.h>
#include <prolog_common/List.h>
#include <prolog_common/Variable.h>

#include <prolog_swi/Context.h>
#include <prolog_swi/Exception.h>

#include "prolog_swi/Term.h"

namespace prolog { namespace swi {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Term::ConversionError::ConversionError() :
  ros::Exception("Failure to convert Prolog term.") {
}

Term::Term() {
}

Term::Term(unsigned long handle) :
  impl_(new Impl(handle)) {
}

Term::Term(const Term& src) :
  impl_(src.impl_) {
}

Term::~Term() {
}

Term::Impl::Impl(const std::string& goal) :
  handle_(0) {
  if (!goal.empty()) {
    handle_ = PL_new_term_ref();
    
    if (!handle_)
      throw Context::ResourceError();
    
    if (!PL_chars_to_term(goal.c_str(), handle_))
      throw Exception(handle_);
  }
}

Term::Impl::Impl(const prolog::Term& term) :
  handle_(0) {
  if (term.isValid()) {
    handle_ = PL_new_term_ref();
    
    if (!handle_)
      throw Context::ResourceError();
    
    if (term.isAtom()) {
      Atom atom(term);
      
      if (!PL_put_atom_chars(handle_, atom.getName().c_str()))
        throw Context::ResourceError();
    }
    else if (term.isCompound()) {
      Compound compound(term);
      std::vector<prolog::Term> arguments = compound.getArguments();
      
      atom_t atom = PL_new_atom(compound.getFunctor().c_str());
      
      if (!atom)
        throw Context::ResourceError();
      
      functor_t functor = PL_new_functor(atom, compound.getArity());
      
      if (!functor)
        throw Context::ResourceError();

      term_t args = PL_new_term_refs(compound.getArity());
      
      if (!args)
        throw Context::ResourceError();
      
      size_t index = 0;
      for (std::vector<prolog::Term>::const_iterator it = arguments.begin();
          it != arguments.end(); ++it, ++index) {
        Term argument;
      
        argument.impl_.reset(new Term::Impl(*it));

        PL_put_term(args+index, argument.impl_->handle_);
      }
      
      if (!PL_cons_functor_v(handle_, functor, args))
        throw Context::ResourceError();
    }
    else if (term.isNumber()) {
      Number number(term);
      
      if (number.isFloat()) {
        if (!PL_put_float(handle_, Float(number).getValue()))
          throw Context::ResourceError();
      }
      else if (number.isInteger()) {
        if (!PL_put_int64(handle_, Integer(number).getValue()))
          throw Context::ResourceError();
      }
    }
    else if (term.isList()) {
      List list(term);
      std::list<prolog::Term> elements = list.getElements();
      
      PL_put_nil(handle_);

      for (std::list<prolog::Term>::const_reverse_iterator it = elements.
          rbegin(); it != elements.rend(); ++it) {
        Term element;
      
        element.impl_.reset(new Term::Impl(*it));
        
        if (!PL_cons_list(handle_, element.impl_->handle_, handle_))
          throw Context::ResourceError();
      }
    }
    else if (term.isVariable())
      PL_put_variable(handle_);
  }
}

Term::Impl::Impl(unsigned long handle) :
  handle_(handle) {
}

Term::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Term::isEmpty() const {
  if (impl_.get())
    return !impl_->handle_;
  else
    return true;
}

bool Term::isValid() const {
  return impl_.get();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Term::operator prolog::Term() const {
  if (impl_.get())
    return impl_->operator prolog::Term();
  else
    return prolog::Term();
}

Term::Impl::operator prolog::Term() const {
  if (handle_) {
    if (PL_is_list(handle_)) {
      term_t list = PL_copy_term_ref(handle_);
      term_t head = PL_new_term_ref();
      
      std::list<prolog::Term> elements;
      
      while (PL_get_list(list, head, list))
        elements.push_back(Term(head));
      
      return List(elements);
    }
    else if (PL_is_compound(handle_)) {
      atom_t functor;
      term_t argument = PL_new_term_ref();
      int arity;

      if (!PL_get_name_arity(handle_, &functor, &arity))
        throw ConversionError();
      
      std::vector<prolog::Term> arguments;
      
      for (size_t index = 0; index < arity; ++index) {
        if (!PL_get_arg(index+1, handle_, argument))
          throw ConversionError();
        
        arguments.push_back(Term(argument));
      }
      
      return Compound(PL_atom_chars(functor), arguments);
    }
    else if (PL_is_float(handle_)) {
      double number;
      
      if (!PL_get_float(handle_, &number))
        throw ConversionError();
      
      return Float(number);
    }
    else if (PL_is_integer(handle_)) {
      long number;
      
      if (!PL_get_long(handle_, &number))
        throw ConversionError();
      
      return Integer(number);
    }
    else if (PL_is_variable(handle_)) {
      char* name;
      
      if (!PL_get_chars(handle_, &name, CVT_VARIABLE | BUF_RING))
        throw ConversionError();
      
      return Variable(name);
    }
    else if (PL_is_atom(handle_)) {
      atom_t atom;
      
      if (!PL_get_atom(handle_, &atom))
        throw ConversionError();
      
      return Atom(PL_atom_chars(atom));
    }
  }

  return prolog::Term();
}

}}
