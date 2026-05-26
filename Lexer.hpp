#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

enum class State {
    INITIAL, 
    UNK, 
    WHITESPACE, 
    OP_NOT, 
    STR_START,
    STR_REST,
    STR_ESCAPE,
    AMPERSAND, 
    INT_FIRST,
    INT_REST,
    COLON,
    OP_LT,
    OP_ASSIGN,
    OP_GT,
    IDENT_FIRST,
    IDENT_REST,
    PIPE
};

enum class TokenID {
    ASSIGN_SHORT,
    OP_EQ,
    OP_NEQ,
    OP_LEQ,
    OP_GEQ,
    OP_OR,
    OP_AND,
    
    SEMICOLON,
    COMMA,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    OP_ASSIGN,
    OP_NOT,
    OP_LT,
    OP_GT,
    AMPERSAND,

    INT_LIT,
    STRING_LIT,
    IDENTIFIER,

    VAR, 
    FUNC, 
    INT_TYPE, 
    BOOL_TYPE, 
    IF, 
    ELSE, 
    FOR, 
    RETURN, 
    PRINT, 
    PRINTLN, 
    TRUE, 
    FALSE, 
    REF,

    UNK,
    ENDOFFILE
};

struct Token {
    TokenID token;
    std::string lexeme;
};

class Lexer {
    private:
        std::istream& input;
        char ch;

        const std::unordered_map<char, TokenID> singleOps = {
            {';', TokenID::SEMICOLON},
            {',', TokenID::COMMA},
            {'+', TokenID::PLUS},
            {'-', TokenID::MINUS},
            {'*', TokenID::MULT},
            {'/', TokenID::DIV},
            {'%', TokenID::MOD},
            {'(', TokenID::LPAREN},
            {')', TokenID::RPAREN},
            {'{', TokenID::LBRACE},
            {'}', TokenID::RBRACE}
        };

        const std::unordered_map<std::string, TokenID> keywords = {
            {"var",     TokenID::VAR},
            {"func",    TokenID::FUNC},
            {"int",     TokenID::INT_TYPE},
            {"bool",    TokenID::BOOL_TYPE},
            {"if",      TokenID::IF},
            {"else",    TokenID::ELSE},
            {"for",     TokenID::FOR},
            {"return",  TokenID::RETURN},
            {"print",   TokenID::PRINT},
            {"println", TokenID::PRINTLN},
            {"true",    TokenID::TRUE},
            {"false",   TokenID::FALSE},
            {"ref",     TokenID::REF}
        };

        void consume() {
            ch = input.get();
        }

    public:
        Lexer(std::istream& input_) : input(input_) {
            consume();
        }

        Token nextToken();

        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;
};