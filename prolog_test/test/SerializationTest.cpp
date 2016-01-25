/******************************************************************************
 * Copyright (C) 2014 by Ralf Kaestner                                        *
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

#include <sstream>

#include <gtest/gtest.h>

#include <prolog_common/Bindings.h>
#include <prolog_common/Query.h>
#include <prolog_common/Term.h>

#include <prolog_serialization/JSONDeserializer.h>
#include <prolog_serialization/JSONSerializer.h>

using namespace prolog;

TEST(Prolog, Serialization) {
  std::stringbuf buffer; 
  std::istream istream(&buffer);  
  std::ostream ostream(&buffer);  
  
  serialization::JSONSerializer serializer;
  serialization::JSONDeserializer deserializer;
  
  serializer.serializeTerm(ostream, "atom");
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeTerm(istream).isAtom());
  
  serializer.serializeTerm(ostream, "Var");
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeTerm(istream).isVariable());
  
  serializer.serializeTerm(ostream, 42);
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeTerm(istream).isNumber());
  
  serializer.serializeTerm(ostream, 42.0);
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeTerm(istream).isNumber());
  
  serializer.serializeTerm(ostream, {"a", "b"});
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeTerm(istream).isList());
  
  serializer.serializeTerm(ostream, Term("f", {"a", "b"}));
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeTerm(istream).isCompound());

  serializer.serializeQuery(ostream, Query("predicate", {"a", "b"}));
  istream.clear();
  EXPECT_TRUE(deserializer.deserializeQuery(istream).isValid());
  
  Bindings bindings;
  
  bindings.addTerm("Atom", "atom");
  bindings.addTerm("Integer", 42);
  bindings.addTerm("Float", 42.0);
  bindings.addTerm("List", {"a", "b"});
  bindings.addTerm("Compound", Term("f", {"a", "b"}));
  
  serializer.serializeBindings(ostream, bindings);
  istream.clear();
  EXPECT_FALSE(deserializer.deserializeBindings(istream).areEmpty());
}
