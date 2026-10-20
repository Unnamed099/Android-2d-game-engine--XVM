#pragma once

#include "common/Token.h"

#include <cstddef>
#include <string>
#include <vector>

class Lexer {
private:
    std::string source;
    size_t pos;
    int line;

public:
    Lexer(const std::string& src) : source(src), pos(0), line(1) {}

    std::vector<Token> tokenize();
};
