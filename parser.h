#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "simdjson.h"

enum class Kind {
  Primitive,
  ArrayClose,
  ArrayOpen,
  ObjectOpen,
  ObjectClose
};

using Key = std::string_view;
using Value = std::string_view;

struct Node {
  Key key;
  Value value;
  Kind kind;
  int depth;
};

class Parser {
private:
  // Pre-allocated buffer for string conversions
  std::string conversion_buffer;
  void parseTree(const Key& key, const simdjson::dom::element& value, int depth);
  
  std::string_view numberToStringView(double num);
  std::string_view numberToStringView(int64_t num);
  
public:
  Parser(size_t jsonSize);
  ~Parser();
  
  std::vector<Node> nodes;
  void parse(const simdjson::dom::element& node, int depth = 0);
};