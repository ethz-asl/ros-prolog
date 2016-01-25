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

#include <prolog_common/Solution.h>

using namespace prolog;

TEST(Prolog, Solution) {
  Bindings bindings;
  
  bindings.addTerm("Atom", "atom");
  bindings.addTerm("Integer", 42);
  bindings.addTerm("Float", 42.0);
  bindings.addTerm("List", {0, 1, 2});

  Solution solution(bindings);
  
  EXPECT_EQ("atom", solution.getValue<std::string>("Atom"));
  EXPECT_ANY_THROW(solution.getValue<int>("Atom"));
  
  EXPECT_EQ(42, solution.getValue<int>("Integer"));
  EXPECT_EQ(42, solution.getValue<int64_t>("Integer"));
  EXPECT_EQ(42, solution.getValue<unsigned int>("Integer"));
  EXPECT_EQ(42, solution.getValue<size_t>("Integer"));
  EXPECT_EQ(true, solution.getValue<bool>("Integer"));
  EXPECT_ANY_THROW(solution.getValue<double>("Integer"));
  
  EXPECT_EQ(42.0, solution.getValue<float>("Float"));
  EXPECT_EQ(42.0, solution.getValue<double>("Float"));
  EXPECT_ANY_THROW(solution.getValue<int>("Float"));
  
  typedef std::list<int> List;
  List list = {0, 1, 2};
  EXPECT_EQ(list, solution.getValue<List>("List"));
  
  typedef std::vector<int> Vector;
  Vector vector = {0, 1, 2};
  EXPECT_EQ(vector, solution.getValue<Vector>("List"));

  EXPECT_ANY_THROW(solution.getValue<int>("List"));
  EXPECT_ANY_THROW(solution.getValue<double>("List"));  
  EXPECT_ANY_THROW(solution.getValue<std::string>("List"));  
}
