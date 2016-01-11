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

#include <json/reader.h>
#include <json/value.h>

#include "prolog_serialization/JSONDeserializer.h"

namespace prolog { namespace serialization {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

JSONDeserializer::ParseError::ParseError(const std::string& description) :
  Exception("Failure to parse JSON expression: "+description) {
}

JSONDeserializer::ConversionError::ConversionError(const std::string&
    description) :
  Exception("Failure to convert JSON value to Prolog term: "+description) {
}

JSONDeserializer::JSONDeserializer() {
}

JSONDeserializer::~JSONDeserializer() {  
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

Bindings JSONDeserializer::deserializeBindings(std::istream& stream) const {
  Json::Value value = deserializeValue(stream);
  
  return valueToBindings(value);
}

Term JSONDeserializer::deserializeTerm(std::istream& stream) const {
  Json::Value value = deserializeValue(stream);
  
  return valueToTerm(value);
}

Json::Value JSONDeserializer::deserializeValue(std::istream& stream) const {
  Json::Reader reader;
  Json::Value root;
  
  if (reader.parse(stream, root, false)) {
    if (root.isNull())
      throw ParseError("No root value found.");
      
    return root;
  }
  else
    throw ParseError(reader.getFormattedErrorMessages());
}

Bindings JSONDeserializer::valueToBindings(const Json::Value& value) const {
  if (!value.isObject()) {
    Bindings bindings;
    
    Json::Value::Members names = value.getMemberNames();
    
    for (Json::Value::Members::iterator it = names.begin();
        it != names.end(); ++it)
      bindings.addTerm(*it, valueToTerm(value[*it]));
  }
  else
    throw ConversionError("Invalid value type.");
}

Term JSONDeserializer::valueToTerm(const Json::Value& value) const {
  if (value.isObject()) {
    std::list<Term> arguments;
    
    if (value.size() != 2)
      throw ParseError("Invalid number of object members.");
      
    if (!value.isMember("functor"))
      throw ParseError("Missing object member named [functor].");
    if (!value["functor"].isString())
      throw ParseError("Member [functor] has invalid value type.");
    
    if (!value.isMember("arguments"))
      throw ParseError("Missing object member named [arguments].");
    if (!value["arguments"].isArray())
      throw ParseError("Member [arguments] has invalid value type.");
    
    Json::Value argumentsValue = value["arguments"];
    
    for (Json::Value::iterator it = argumentsValue.begin();
        it != argumentsValue.end(); ++it)
      arguments.push_back(valueToTerm(*it));
    
    return Term(value["functor"].asString(), arguments);
  }
  else if (value.isArray()) {
    std::list<Term> elements;
    
    for (Json::Value::iterator it = value.begin(); it != value.end(); ++it)
      elements.push_back(valueToTerm(*it));
    
    return Term(elements);
  }
  else if (value.isString())
    return Term(value.asString());
  else if (value.isDouble())
    return Term(value.asDouble());
  else if (value.isIntegral())
    return Term(static_cast<int64_t>(value.asInt64()));
  else
    throw ConversionError("Invalid value type.");
}

}}
