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
#include <prolog_swi/Exception.h>
#include <prolog_swi/Query.h>

using namespace prolog;

TEST(Prolog, Exception) {
  swi::Context context;
  Bindings bindings;
  
  EXPECT_TRUE(context.init());
  
  swi::Exception exception;
  
  EXPECT_TRUE(exception.isEmpty());
  
  swi::Query query("undefined");
  
  EXPECT_TRUE(query.open());
  EXPECT_ANY_THROW(query.nextSolution(bindings));
}
