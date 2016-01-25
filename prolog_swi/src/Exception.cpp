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

#include "prolog_swi/Exception.h"

namespace prolog { namespace swi {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Exception::Exception() :
  Term(PL_exception(0)),
  ros::Exception(messageToString(impl_->handle_)) {
}

Exception::Exception(unsigned long handle) :
  Term(handle),
  ros::Exception(messageToString(handle)) {
}

Exception::Exception(const Exception& src) :
  Term(src),
  ros::Exception(src) {
}

Exception::~Exception() throw() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void Exception::raise() {
  if (impl_.get() && impl_->handle_)
    PL_raise_exception(impl_->handle_);
}

std::string Exception::messageToString(unsigned long handle) {
  if (handle) {
    fid_t frame = PL_open_foreign_frame();
    
    if (frame) {
      term_t args = PL_new_term_refs(2);
      PL_put_term(args+0, handle); 
      
      atom_t moduleAtom = PL_new_atom("$messages");
      atom_t predicateAtom = PL_new_atom("message_to_string");
      
      module_t module = PL_new_module(moduleAtom);
      predicate_t predicate = PL_pred(PL_new_functor(predicateAtom, 2),
        module);

      PL_unregister_atom(moduleAtom);
      PL_unregister_atom(predicateAtom);

      qid_t query = PL_open_query(module, PL_Q_CATCH_EXCEPTION,
        predicate, args);

      if (query) {
        if (PL_next_solution(query)) {
          char* message;
          
          if (PL_get_chars(args, &message, CVT_ALL | CVT_WRITEQ | BUF_RING)) {
            PL_close_foreign_frame(frame);
            
            return message;
          }
        }
      }
      
      PL_close_foreign_frame(frame);
    }
    
    return std::string("Unknown error: Failure to retrieve error message.");
  }
  else
    return std::string();
}

}}
