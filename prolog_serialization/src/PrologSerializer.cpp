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
#include <prolog_common/Float.h>
#include <prolog_common/Integer.h>
#include <prolog_common/List.h>
#include <prolog_common/Number.h>
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
  
  if ((name[0] == toupper(name[0])) || (name[0] == '_') ||
      (name.find_first_of(" ") != std::string::npos))
    stream << "'" << name << "'";
  else
    stream << name;
}

void PrologSerializer::serializeCompound(std::ostream& stream, const Compound&
    compound) const {
  std::string functor = compound.getFunctor();
  std::list<Term> arguments = compound.getArguments();

  stream << functor;
  if (!arguments.empty()) {
    stream << "(";
    
    for (std::list<Term>::const_iterator it = arguments.begin();
        it != arguments.end(); ++it) {
      if (it != arguments.begin())
        stream << ", ";
      
      serializeTerm(stream, *it);
    }
      
    stream << ")";
  }
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

void PrologSerializer::serializeVariable(std::ostream& stream, const Variable&
    variable) const {
  stream << variable.getName();
}

}}
