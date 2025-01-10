#include <iostream>
#include <string>
#include "simdjson.h"
#include <chrono>
#include "parser.h"

using namespace simdjson;
using namespace std::chrono;

std::string kind_to_string(Kind kind) {
    switch (kind) {
        case Kind::Primitive:
            return "Primitive";
        case Kind::ArrayClose:
            return "ArrayClose";
        case Kind::ArrayOpen:
            return "ArrayOpen";
        case Kind::ObjectOpen:
            return "ObjectOpen";
        case Kind::ObjectClose:
            return "ObjectClose";
        default:
            return "Unknown";
    }
}

void print_node(const Node& node) {
    std::cout << "Key: " << node.key << " Value: " << node.value << " Kind: " << kind_to_string(node.kind) << " Depth: " << node.depth << std::endl;
}

int main() {
    auto startLoading = high_resolution_clock::now();
    padded_string json = padded_string::load("./large.json");
    auto stopLoading = high_resolution_clock::now();

    std::cout << "Loading time: " << duration_cast<milliseconds>(stopLoading - startLoading).count() << "ms" << std::endl;

    auto startParsing = high_resolution_clock::now();
    dom::parser domParser;
    dom::element element = domParser.parse(json);  // Usando o dom::parser
    auto stopParsing = high_resolution_clock::now();

    std::cout << "Parsing time: " << duration_cast<milliseconds>(stopParsing - startParsing).count() << "ms" << std::endl;

    auto startNodeTreeParsing = high_resolution_clock::now();
    auto tParser = new Parser(json.length());
    tParser->parse(element);  // Agora passando dom::element
    auto stopNodeTreeParsing = high_resolution_clock::now();

    std::cout << "Node Tree Parsing time: " << duration_cast<milliseconds>(stopNodeTreeParsing - startNodeTreeParsing).count() << "ms" << std::endl;


    std::cout << "Total time: " << duration_cast<milliseconds>(stopNodeTreeParsing - startLoading).count() << "ms" << std::endl;

    std::cout << "Nodes: " << tParser->nodes.size() << std::endl;

    // for (const auto& node : tParser->nodes) {
    //     print_node(node);
    // }
    delete tParser;
    return 0;
}
