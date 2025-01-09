#pragma once

#include <string>
#include <variant>
#include <vector>
#include <any>
#include <iostream>
#include "simdjson.h"

using namespace simdjson;

enum class Kind {
  Primitive,
  ArrayClose,
  ArrayOpen,
  ObjectOpen,
  ObjectClose
};

using Key = std::string;
using Value = std::string_view;

struct Node {
  Key key;
  Value value;
  Kind kind;
  uint8_t depth;
};

class Parser {
private:
  void parseTree(Key key, const simdjson::dom::element& value, uint8_t depth);

public:
  Parser();
  ~Parser();

  std::vector<Node> nodes;

  void parse(const simdjson::dom::element& node, uint8_t depth = 0);
};