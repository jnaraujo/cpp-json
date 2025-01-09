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
  void parseTree(Key key, const simdjson::dom::element& value, int depth);
public:
  Parser();
  ~Parser();

  std::vector<Node> nodes;

  void parse(const simdjson::dom::element& node, int depth = 0);
};