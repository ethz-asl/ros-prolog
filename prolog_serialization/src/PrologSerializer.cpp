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

#include <prolog_common/Atom.h>
#include <prolog_common/Compound.h>
#include <prolog_common/Fact.h>
#include <prolog_common/Float.h>
#include <prolog_common/Integer.h>
#include <prolog_common/List.h>
#include <prolog_common/Number.h>
#include <prolog_common/Rule.h>
#include <prolog_common/Variable.h>

#include "prolog_serialization/PrologSerializer.h"

namespace prolog { namespace serialization {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

PrologSerializer::PrologSerializer() {
}

PrologSerializer::~PrologSerializer() {  
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void PrologSerializer::serializeBindings(std::ostream& stream, const Bindings&
    bindings) const {
  for (Bindings::ConstIterator it = bindings.begin();
       it != bindings.end(); ++it) {
    if (it != bindings.begin())
      stream << ",\n";
    
    stream << it->first << " = ";
    serializeTerm(stream, it->second);
  }
}

void PrologSerializer::serializeClause(std::ostream& stream, const Clause&
    clause) const {
  if (clause.isFact())
    serializeFact(stream, clause);
  else if (clause.isRule())
    serializeRule(stream, clause);
}

void PrologSerializer::serializeProgram(std::ostream& stream, const Program&
    program) const {
  for (std::list<Clause>::const_iterator it = program.begin();
       it != program.end(); ++it) {
    if (it != program.begin())
      stream << "\n";
    
    serializeClause(stream, *it);
  }
}

void PrologSerializer::serializeQuery(std::ostream& stream, const Query&
    query) const {
  std::string module = query.getModule();
  std::string predicate = query.getPredicate();

  if (!module.empty())
    stream << module << ":";
  stream << predicate;
  if (query.getArity()) {
    stream << "(";
    
    for (std::vector<Term>::const_iterator it = query.begin();
        it != query.end(); ++it) {
      if (it != query.begin())
        stream << ", ";
      
      serializeTerm(stream, *it);
    }
      
    stream << ")";
  }
  
  stream << ".";
}
    
void PrologSerializer::serializeTerm(std::ostream& stream, const Term& term)
    const {
  if (term.isAtom())
    serializeAtom(stream, term);
  else if (term.isCompound())
    serializeCompound(stream, term);
  else if (term.isList())
    serializeList(stream, term);
  else if (term.isNumber())
    serializeNumber(stream, term);
  else if (term.isVariable())
    serializeVariable(stream, term);
}

void PrologSerializer::serializeAtom(std::ostream& stream, const Atom& atom)
    const {
  std::string name = atom.getName();
  bool quoted = (name.empty() || (name[0] == '_'));
  
  if (!quoted) {
    for (size_t index = 0; index < name.length(); ++index) {
      if (!isalpha(name[index]) || !isgraph(name[index]) ||
          isspace(name[index])) {
        quoted = true;
        break;
      }
    }
  }
  
  if (quoted)
    stream << "'" << name << "'";
  else
    stream << name;
}

void PrologSerializer::serializeCompound(std::ostream& stream, const Compound&
    compound) const {
  std::string functor = compound.getFunctor();

  stream << functor;
  if (compound.getArity()) {
    stream << "(";
    
    for (std::vector<Term>::const_iterator it = compound.begin();
        it != compound.end(); ++it) {
      if (it != compound.begin())
        stream << ", ";
      
      serializeTerm(stream, *it);
    }
      
    stream << ")";
  }
}

void PrologSerializer::serializeFact(std::ostream& stream, const Fact& fact)
    const {
  std::string predicate = fact.getPredicate();
  std::vector<Term> arguments = fact.getArguments();

  stream << predicate;
  if (!arguments.empty()) {
    stream << "(";
    
    for (std::vector<Term>::const_iterator it = arguments.begin();
        it != arguments.end(); ++it) {
      if (it != arguments.begin())
        stream << ", ";
      
      serializeTerm(stream, *it);
    }
      
    stream << ")";
  }
  
  stream << ".";
}

void PrologSerializer::serializeFloat(std::ostream& stream, const Float&
    number) const {
  stream << number.getValue();
}

void PrologSerializer::serializeInteger(std::ostream& stream, const Integer&
    number) const {
  stream << number.getValue();
}

void PrologSerializer::serializeNumber(std::ostream& stream, const Number&
    number) const {
  if (number.isFloat())
    serializeFloat(stream, number);
  else if (number.isInteger())
    serializeInteger(stream, number);
}

void PrologSerializer::serializeList(std::ostream& stream, const List& list)
    const {
  std::list<Term> elements = list.getElements();

  stream << "[";
  
  for (std::list<Term>::const_iterator it = elements.begin();
      it != elements.end(); ++it) {
    if (it != elements.begin())
      stream << ", ";
    
    serializeTerm(stream, *it);
  }
  
  stream << "]";
}

void PrologSerializer::serializeRule(std::ostream& stream, const Rule& rule)
    const {
  std::string predicate = rule.getPredicate();
  std::vector<Term> arguments = rule.getArguments();
  std::list<Term> goals = rule.getGoals();

  stream << predicate;
  if (!arguments.empty()) {
    stream << "(";
    
    for (std::vector<Term>::const_iterator it = arguments.begin();
        it != arguments.end(); ++it) {
      if (it != arguments.begin())
        stream << ", ";
      
      serializeTerm(stream, *it);
    }
      
    stream << ")";
  }
  
  stream << " :- ";
  
  for (std::list<Term>::const_iterator it = goals.begin();
      it != goals.end(); ++it) {
    if (it != goals.begin())
      stream << ", ";
    
    serializeTerm(stream, *it);
  }
  
  stream << ".";
}

void PrologSerializer::serializeVariable(std::ostream& stream, const Variable&
    variable) const {
  stream << variable.getName();
}

}}
