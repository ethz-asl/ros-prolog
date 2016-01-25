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

#include <gtest/gtest.h>

#include <prolog_swi/Context.h>
#include <prolog_swi/Query.h>

#include <prolog_test/CurrentAtomQuery.h>
#include <prolog_test/FileSearchPathQuery.h>

using namespace prolog;

TEST(Prolog, Query) {
  swi::Context context;
  swi::Query query;
  Bindings bindings;
  
  EXPECT_TRUE(context.init());
  
  query = swi::Query("true");

  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));

  query = swi::Query("false");

  EXPECT_TRUE(query.open());
  EXPECT_FALSE(query.nextSolution(bindings));
  
  query = Query("atom", {"true"});
  
  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  
  query = swi::Query("current_atom(Atom)");
  
  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  do {
    EXPECT_TRUE(bindings.contain("Atom"));
  }
  while (query.nextSolution(bindings));
  
  query = Query("current_atom", {"Atom"});
  
  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  do {
    EXPECT_TRUE(bindings.contain("Atom"));
  }
  while (query.nextSolution(bindings));

  query = test::CurrentAtomQuery("Atom");
  
  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  do {
    EXPECT_TRUE(bindings.contain("Atom"));
  }
  while (query.nextSolution(bindings));
  
  query = swi::Query("file_search_path(Alias, Path)");
  
  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  do {
    EXPECT_TRUE(bindings.contain("Alias"));
    EXPECT_TRUE(bindings.contain("Path"));
  }
  while (query.nextSolution(bindings));
  
  query = Query("file_search_path", {"Alias", "Path"});

  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  do {
    EXPECT_TRUE(bindings.contain("Alias"));
    EXPECT_TRUE(bindings.contain("Path"));
  }
  while (query.nextSolution(bindings));
  
  query = test::FileSearchPathQuery("Alias", "Path");
  
  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  do {
    EXPECT_TRUE(bindings.contain("Alias"));
    EXPECT_TRUE(bindings.contain("Path"));
  }
  while (query.nextSolution(bindings));
  
  query = swi::Query("sort([b, a], List)");

  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  EXPECT_TRUE(bindings.contain("List"));
  EXPECT_TRUE(bindings["List"].isList());
  
  query = Query("sort", {{"b", "a"}, "List"});

  EXPECT_TRUE(query.open());
  EXPECT_TRUE(query.nextSolution(bindings));
  EXPECT_TRUE(bindings.contain("List"));
  EXPECT_TRUE(bindings["List"].isList());
}
