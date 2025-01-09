#include "parser.h"

void Parser::parseTree(Key key, const simdjson::dom::element& value, uint8_t depth) {
  if((!value.is_object() && !value.is_array()) || value.is_null()) {
    Value nodeValue;
    if(value.is_string()) {
      nodeValue = value.get<std::string_view>();
    } else if(value.is_int64()) {
      nodeValue = std::to_string(value.get<int64_t>());
    } else if(value.is_bool()) {
      nodeValue = std::to_string(value.get<bool>());
    } else if(value.is_double()) {
      nodeValue = std::to_string(value.get<double>());
    } else {
      nodeValue = "null";
    }

    nodes.emplace_back(Node{key, nodeValue, Kind::Primitive, depth});
    return;
  }

  nodes.emplace_back(Node{key, "", value.is_array() ? Kind::ArrayOpen : Kind::ObjectOpen, depth});

  parse(value, depth+1);

  if(value.is_array()) {
    nodes.emplace_back(Node{key, "", Kind::ArrayClose, depth});
  }
}

void Parser::parse(const simdjson::dom::element& node, uint8_t depth) {
  if(node.is_array()) {
    size_t index = 0;
    for (const auto& element : node.get_array()) {
      parseTree(std::to_string(index), element, depth);
      index++;
    }
  } else if(node.is_object()) {
    for (const auto& field : node.get_object()) {
      parseTree(std::string(field.key), field.value, depth);
    }
  }
}

Parser::Parser(/* args */) = default;

Parser::~Parser() {
  std::cout << "Cleaning parser" << std::endl;
  nodes.clear();
}