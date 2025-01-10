#include "parser.h"
#include <charconv>

Parser::Parser(size_t jsonSize): conversion_buffer(32, '\0') {
  size_t possibleSize = 0.01 * jsonSize;
  std::cout << "Preallocating nodes with " << possibleSize << " nodes.\n";
  nodes.reserve(possibleSize);
};
Parser::~Parser() = default;

std::string_view Parser::numberToStringView(double num) {
    // Não é necessário clear() porque sobrescreveremos o conteúdo
    auto [ptr, ec] = std::to_chars(conversion_buffer.data(), 
                                   conversion_buffer.data() + conversion_buffer.size(), 
                                   num);
    if (ec == std::errc()) {
        return std::string_view(conversion_buffer.data(), ptr - conversion_buffer.data());
    }
    return "0";
}

std::string_view Parser::numberToStringView(int64_t num) {
    auto [ptr, ec] = std::to_chars(conversion_buffer.data(), 
                                   conversion_buffer.data() + conversion_buffer.size(), 
                                   num);
    if (ec == std::errc()) {
        return std::string_view(conversion_buffer.data(), ptr - conversion_buffer.data());
    }
    return "0";
}

void Parser::parseTree(const Key& key, const simdjson::dom::element& value, int depth) {
  if(value.is_null() || (!value.is_object() && !value.is_array())) {
    Value nodeValue;
    switch (value.type()) {
      case simdjson::dom::element_type::STRING:
        nodeValue = value.get<std::string_view>().value();
        break;
      case simdjson::dom::element_type::INT64:
        nodeValue = numberToStringView(value.get<int64_t>().value());
        break;
      case simdjson::dom::element_type::DOUBLE:
        nodeValue = numberToStringView(value.get<double>().value());
        break;
      case simdjson::dom::element_type::BOOL:
        nodeValue = value.get<bool>().value() ? "true" : "false";
        break;
      default:
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
        char index_str[20];  // Buffer para conversão
        for (auto element : node.get_array()) {
            auto [ptr, ec] = std::to_chars(index_str, index_str + 20, index++);
            if (ec == std::errc()) {
                parseTree(std::string_view(index_str, ptr - index_str), element, depth);
            }
        }
    } else if (node.is_object()) {
        for (const auto& [field_key, field_value] : node.get_object()) {
            parseTree(field_key, field_value, depth);
        }
    }
}