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

#include <boost/lexical_cast.hpp>
  
#include "prolog/Exceptions.h"

namespace semantic_map {

namespace prolog {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

InvalidOperationException::InvalidOperationException(const std::string&
    description) :
  Exception("Invalid Prolog operation: "+description) {
}

InvalidValueIndexException::InvalidValueIndexException(size_t index) :
  Exception("Invalid Prolog value index ["+boost::lexical_cast<std::string>(
    index)+"]") {
}

ParseErrorException::ParseErrorException(const std::string& description) :
  Exception("Prolog parse error: "+description) {
}

UnboundVariableException::UnboundVariableException(const std::string& name) :
  Exception("Prolog variable ["+name+"] is unbound") {
}

}

}
