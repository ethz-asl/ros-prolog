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

#include "prolog_common/Program.h"

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Program::Program(const std::list<Clause>& clauses) {
  impl_.reset(new Impl(clauses));
}

Program::Program(const Program& src) :
  impl_(src.impl_) {
}

Program::~Program() {  
}

Program::Impl::Impl(const std::list<Clause>& clauses) :
  clauses_(clauses) {
}

Program::Impl::~Impl() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t Program::getNumClauses() const {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_.size();
}

std::list<Clause> Program::getClauses() const {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_;
}

bool Program::isEmpty() const {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_.empty();
}
      
/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

std::list<Clause>::iterator Program::begin() {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_.begin();
}

std::list<Clause>::const_iterator Program::begin() const {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_.begin();
}

std::list<Clause>::iterator Program::end() {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_.end();
}

std::list<Clause>::const_iterator Program::end() const {
  return boost::static_pointer_cast<Impl>(impl_)->clauses_.end();
}

void Program::append(const Clause& clause) {
  boost::static_pointer_cast<Impl>(impl_)->clauses_.push_back(clause);
}

void Program::append(const Program& program) {
  std::list<Clause>& clauses = boost::static_pointer_cast<Impl>(
    impl_)->clauses_;
  std::list<Clause>& programClauses = boost::static_pointer_cast<Impl>(
    program.impl_)->clauses_;
  
  clauses.insert(clauses.end(), programClauses.begin(), programClauses.end());
}

void Program::clear() {
  boost::static_pointer_cast<Impl>(impl_)->clauses_.clear();
}

/*****************************************************************************/
/* Operators                                                                 */
/*****************************************************************************/

Program& Program::operator+=(const Clause& clause) {
  append(clause);
  
  return *this;
}

Program& Program::operator+=(const Program& program) {
  append(program);
  
  return *this;
}

Program Program::operator+(const Clause& clause) const {
  Program result(*this);
  
  result.append(clause);
  
  return result;
}

Program Program::operator+(const Program& program) const {
  Program result(*this);
  
  result.append(program);
  
  return result;
}

}
