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

#include <boost/algorithm/string.hpp>

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

Clause JSONDeserializer::deserializeClause(std::istream& stream) const {
  Json::Value value = deserializeValue(stream);
  
  return valueToClause(value);
}

Program JSONDeserializer::deserializeProgram(std::istream& stream) const {
  Json::Value value = deserializeValue(stream);
  
  return valueToProgram(value);
}

Query JSONDeserializer::deserializeQuery(std::istream& stream) const {
  Json::Value value = deserializeValue(stream);
  
  return valueToQuery(value);
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
  else {
    std::string messages = reader.getFormattedErrorMessages();
    std::string description;
    std::vector<std::string> lines;
    
    boost::split(lines, messages, boost::is_any_of("\n"));
    
    for (size_t index = 0; index < lines.size(); ++index) {
      if (!lines[index].empty() && (lines[index][0] == '*'))
        lines[index][0] = ' ';
      
      boost::algorithm::trim(lines[index]);
      
      if (!lines[index].empty()) {
        if (!description.empty())
          description += ": ";
        description += lines[index];
      }
    }
    
    throw ParseError(description);
  }
}

Bindings JSONDeserializer::valueToBindings(const Json::Value& value) const {
  if (value.isObject()) {
    Bindings bindings;
    
    Json::Value::Members names = value.getMemberNames();
    
    for (Json::Value::Members::iterator it = names.begin();
        it != names.end(); ++it)
      bindings.addTerm(*it, valueToTerm(value[*it]));
    
    return bindings;
  }
  else
    throw ConversionError("Invalid value type.");
}

Program JSONDeserializer::valueToProgram(const Json::Value& value) const {
  if (value.isArray()) {
    std::list<Clause> clauses;
    
    for (Json::Value::iterator it = value.begin(); it != value.end(); ++it)
      clauses.push_back(valueToClause(*it));
    
    return Program(clauses);
  }
  else
    throw ConversionError("Invalid value type.");
}

Query JSONDeserializer::valueToQuery(const Json::Value& value) const {
  if (value.isObject()) {
    std::string module;
    std::vector<Term> arguments;
    
    if ((value.size() < 2) || (value.size() > 3))
      throw ParseError("Invalid number of object members.");
      
    if (value.isMember("module")) {
      if (value["module"].isString())
        module = value["module"].asString();
      else
        throw ParseError("Member [module] has invalid value type.");
    }
    
    if (!value.isMember("predicate"))
      throw ParseError("Missing object member named [predicate].");
    if (!value["predicate"].isString())
      throw ParseError("Member [predicate] has invalid value type.");
    
    if (value.isMember("arguments")) {
      if (value["arguments"].isArray()) {
        Json::Value argumentsValue = value["arguments"];
    
        for (Json::Value::iterator it = argumentsValue.begin();
            it != argumentsValue.end(); ++it)
          arguments.push_back(valueToTerm(*it));    
      }
      else
        throw ParseError("Member [arguments] has invalid value type.");
    }
    
    return Query(module, value["predicate"].asString(), arguments);
  }
  else
    throw ConversionError("Invalid value type.");
}

Clause JSONDeserializer::valueToClause(const Json::Value& value) const {
  if (value.isObject()) {
    std::vector<Term> arguments;
    std::list<Term> goals;
    
    if ((value.size() < 1) || (value.size() > 3))
      throw ParseError("Invalid number of object members.");
      
    if (!value.isMember("predicate"))
      throw ParseError("Missing object member named [predicate].");
    if (!value["predicate"].isString())
      throw ParseError("Member [predicate] has invalid value type.");
    
    if (value.isMember("arguments")) {
      if (value["arguments"].isArray()) {
        Json::Value argumentsValue = value["arguments"];
    
        for (Json::Value::iterator it = argumentsValue.begin();
            it != argumentsValue.end(); ++it)
          arguments.push_back(valueToTerm(*it));    
      }
      else
        throw ParseError("Member [arguments] has invalid value type.");
    }
    
    if (value.isMember("goals")) {
      if (value["goals"].isArray()) {
        Json::Value goalsValue = value["goals"];
    
        for (Json::Value::iterator it = goalsValue.begin();
            it != goalsValue.end(); ++it)
          goals.push_back(valueToTerm(*it));    
      }
      else
        throw ParseError("Member [goals] has invalid value type.");
    }
    
    return Clause(value["predicate"].asString(), arguments, goals);
  }
  else
    throw ConversionError("Invalid value type.");
}

Term JSONDeserializer::valueToTerm(const Json::Value& value) const {
  if (value.isObject()) {
    std::vector<Term> arguments;
    
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
