#pragma once

#include "../utils/Tokens.hpp"
#include "../utils/States.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>


class Lexer{
    private:
        void consume(){
            ch = input.get();
        }

        int ch;
        std::istream& input;

        std::unordered_map<std::string, TokenID> keywords = {
            {"var",     TokenID::KW_VAR},
            {"func",    TokenID::KW_FUNC},
            {"return",  TokenID::KW_RETURN},
            {"if",      TokenID::KW_IF},
            {"else",    TokenID::KW_ELSE},
            {"for",     TokenID::KW_FOR},
            {"true",    TokenID::KW_TRUE},
            {"false",   TokenID::KW_FALSE},
            {"int",     TokenID::KW_INT},
            {"bool",    TokenID::KW_BOOL},
            {"ref",     TokenID::KW_REF},
            {"print",   TokenID::KW_PRINT},
            {"println", TokenID::KW_PRINTLN}
        };

        std::unordered_map<std::string, TokenID> single_ops = {
            {"+", TokenID::OP_ADD},
            {"-", TokenID::OP_MINUS},
            {"*", TokenID::OP_MUL},
            {"%", TokenID::OP_MOD},
            {";", TokenID::SEMICOLON},
            {",", TokenID::COMMA},
            {"(", TokenID::OPEN_PAR},
            {")", TokenID::CLOSE_PAR},
            {"{", TokenID::OPEN_BRACE},
            {"}", TokenID::CLOSE_BRACE}
        };

    public: 
        Lexer(std::istream& in): input(in) {
            consume();
        }

        Token nextToken();
};