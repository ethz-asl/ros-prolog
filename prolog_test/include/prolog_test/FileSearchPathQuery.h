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

/** \file FileSearchPathQuery.h
  * \brief Header file providing the FileSearchPathQuery class interface
  */

#ifndef ROS_PROLOG_TEST_FILE_SEARCH_PATH_QUERY_H
#define ROS_PROLOG_TEST_FILE_SEARCH_PATH_QUERY_H

#include <prolog_common/Query.h>

namespace prolog {
  namespace test {
    /** \brief Prolog file_search_path/2 test query
      */
    class FileSearchPathQuery :
      public Query {
    public:
      /** \brief Default constructor
        */
      FileSearchPathQuery(const Term& alias = "Alias", const Term&
        path = "Path");
      
      /** \brief Destructor
        */
      virtual ~FileSearchPathQuery();
    };
  };
};

#endif
