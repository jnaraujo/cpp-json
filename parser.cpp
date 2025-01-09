#include "parser.h"

void Parser::parseTree(Key key, const simdjson::dom::element& value, int depth) {
  if((!value.is_object() && !value.is_array()) || value.is_null()) {
    Value nodeValue;
    try {
      switch (value.type()) {
        case simdjson::dom::element_type::STRING:
          nodeValue = value.get<std::string_view>().value();
          break;
        case simdjson::dom::element_type::INT64:
          nodeValue = std::to_string(value.get<int64_t>().value());
          break;
        case simdjson::dom::element_type::DOUBLE:
          nodeValue = std::to_string(value.get<double>().value());
          break;
        case simdjson::dom::element_type::BOOL:
          nodeValue = value.get<bool>().value() ? "true" : "false";
          break;
        default:
          nodeValue = "null";
      }
    } catch (const simdjson::simdjson_error& e) {
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

void Parser::parse(const simdjson::dom::element& node, int depth) {
    if (node.is_array()) {
        size_t index = 0;
        std::string index_str;
        index_str.reserve(20);  // Pre-allocate for index conversion
        
        for (auto element : node.get_array()) {
            index_str.clear();
            index_str = std::to_string(index++);
            parseTree(index_str, element, depth);
        }
    } else if (node.is_object()) {
        for (auto [field_key, field_value] : node.get_object()) {
            parseTree(field_key, field_value, depth);
        }
    }
}

Parser::Parser(/* args */) = default;

Parser::~Parser() {
  std::cout << "Cleaning parser" << std::endl;
  nodes.clear();
}