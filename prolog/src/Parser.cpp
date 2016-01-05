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

#include <json-glib/json-glib.h>

#include <prolog/Exceptions.h>
#include <prolog/Term.h>

#include "prolog/Parser.h"

namespace semantic_map {

namespace prolog {

/*****************************************************************************/
/* Unexposed methods                                                         */
/*****************************************************************************/

namespace detail {

static Value parseValue(JsonNode* node);
  
static std::vector<Value> parseArrayList(GList* nodes) {
  std::vector<Value> list(g_list_length(nodes));

  for (size_t i = 0; nodes; i++, nodes = g_list_next(nodes))
    list[i] = parseValue(reinterpret_cast<JsonNode*>(nodes->data));
  
  return list;
}

static Value parseArrayValue(JsonArray* array) {
  return parseArrayList(json_array_get_elements(array));
}

static Value parseFundamentalValue(JsonNode* node) {
  GType type = json_node_get_value_type(node);
  
  if (type == G_TYPE_STRING)
    return Value(json_node_get_string(node));
  else if (type == G_TYPE_DOUBLE)
    return Value(json_node_get_double(node));
  else if (type == G_TYPE_INT64)
    return Value(json_node_get_int(node));
  else
    throw ParseErrorException("Unexpected value fundamental type ["+
      std::string(g_type_name(type))+"] in JSON expression.");
}

static Value parseObjectValue(JsonObject* object) {
  JsonArray* array = json_object_get_array_member(object, "term");
  
  if (!array)
    throw ParseErrorException("Malformed JSON object expression: "
      "Value for [term] is invalid.");

  if (json_array_get_length(array) < 2)
    throw ParseErrorException("Malformed JSON object expression: "
      "Insufficient number of arguments.");

  const gchar* functor = json_array_get_string_element(array, 0);

  if (!functor)
    throw ParseErrorException("Malformed JSON object expression: "
      "Functor name is invalid.");
  
  return Term(functor, parseArrayList(g_list_next(
    json_array_get_elements(array))));
}

static Value parseValue(JsonNode* node) {
  JsonNodeType type = json_node_get_node_type(node);

  if (type == JSON_NODE_OBJECT)
    return parseObjectValue(json_node_get_object(node));
  else if (type == JSON_NODE_ARRAY)
    return parseArrayValue(json_node_get_array(node));
  else if (type == JSON_NODE_VALUE)
    return parseFundamentalValue(node);
  else
    throw ParseErrorException("Unexpected value type ["+
      std::string(json_node_type_name(node))+"] in JSON expression.");
}
  
}
  
/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Parser::Parser() {
}

Parser::~Parser() {  
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

Bindings Parser::parse(const std::string& json) const {
  JsonParser* parser = json_parser_new();
  GError* error = 0;

  if (!json_parser_load_from_data(parser, json.c_str(), json.size(),
      &error)) {
    std::string description(error->message);
    g_error_free(error);
  
    throw ParseErrorException(description);
  }

  try {
    JsonNode *root = json_parser_get_root(parser);
    
    if (!root)
      throw ParseErrorException("No root node found in JSON expression.");

    if (json_node_get_node_type(root) != JSON_NODE_OBJECT)
      throw ParseErrorException("Malformed JSON expression.");

    Bindings bindings;

    JsonObject* rootObject = json_node_get_object(root);
    GList* keys = json_object_get_members(rootObject);
    
    for (; keys; keys = g_list_next(keys)) {
      std::string name = reinterpret_cast<const char*>(keys->data);
      
      bindings.addValue(name, detail::parseValue(json_object_get_member(
        rootObject, reinterpret_cast<const char*>(keys->data))));
    }
    
    return bindings;
  }
  catch(const std::exception& exception) {
    g_object_unref(parser);
    
    throw ParseErrorException(exception.what());
  }
}

}

}
