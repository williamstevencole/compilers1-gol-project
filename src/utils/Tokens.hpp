#pragma once

#include "string"
#include <format>

enum class TokenID{
    //"EDGE CASES" 
    UNK,
    ENDOFFILE,

    //LITERAL VALUES
    IDENTIFIER,
    INT_LIT,
    STRING_LIT,

    // SINGLE OPS
    OP_ADD,
    OP_NOT,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    SEMICOLON,
    COMMA,
    OPEN_PAR,
    CLOSE_PAR,
    OPEN_BRACE,
    CLOSE_BRACE,
    OP_LT,
    OP_GT,
    OP_ASSIGN,
    OP_REF,

    // DOUBLE CHAR OP
    OP_ASSIGN_SHORT,
    OP_EQ,
    OP_NEQ,
    OP_LEQ,
    OP_GEQ,
    OP_OR,
    OP_AND,

    // KEYWORDS
    KW_VAR,
    KW_FUNC,
    KW_RETURN,
    KW_IF,
    KW_ELSE,
    KW_FOR,
    KW_TRUE,
    KW_FALSE,
    KW_INT,
    KW_BOOL,
    KW_REF,
    KW_PRINT,
    KW_PRINTLN
};

struct Token {
    TokenID id;
    std::string lexeme;

    std::string toString() {
        std::string result;
        
        switch (id) {
            // EDGE CASES
            case TokenID::UNK:              result = "UNK"; break;
            case TokenID::ENDOFFILE:        result = "ENDOFFILE"; break;

            // LITERAL VALUES
            case TokenID::IDENTIFIER:       result = "IDENTIFIER"; break;
            case TokenID::INT_LIT:          result = "INT_LIT"; break;
            case TokenID::STRING_LIT:       result = "STRING_LIT"; break;

            // SINGLE OPS
            case TokenID::OP_ADD:           result = "OP_ADD"; break;
            case TokenID::OP_NOT:           result = "OP_NOT"; break;
            case TokenID::OP_MINUS:         result = "OP_MINUS"; break;
            case TokenID::OP_MUL:           result = "OP_MUL"; break;
            case TokenID::OP_DIV:           result = "OP_DIV"; break;
            case TokenID::OP_MOD:           result = "OP_MOD"; break;
            case TokenID::SEMICOLON:        result = "SEMICOLON"; break;
            case TokenID::COMMA:            result = "COMMA"; break;
            case TokenID::OPEN_PAR:         result = "OPEN_PAR"; break;
            case TokenID::CLOSE_PAR:        result = "CLOSE_PAR"; break;
            case TokenID::OPEN_BRACE:       result = "OPEN_BRACE"; break;
            case TokenID::CLOSE_BRACE:      result = "CLOSE_BRACE"; break;
            case TokenID::OP_LT:            result = "OP_LT"; break;
            case TokenID::OP_GT:            result = "OP_GT"; break;
            case TokenID::OP_ASSIGN:        result = "OP_ASSIGN"; break;
            case TokenID::OP_REF:           result = "OP_REF"; break;

            // DOUBLE CHAR OP
            case TokenID::OP_ASSIGN_SHORT:  result = "OP_ASSIGN_SHORT"; break;
            case TokenID::OP_EQ:            result = "OP_EQ"; break;
            case TokenID::OP_NEQ:           result = "OP_NEQ"; break;
            case TokenID::OP_LEQ:           result = "OP_LEQ"; break;
            case TokenID::OP_GEQ:           result = "OP_GEQ"; break;
            case TokenID::OP_OR:            result = "OP_OR"; break;
            case TokenID::OP_AND:           result = "OP_AND"; break;

            // KEYWORDS
            case TokenID::KW_VAR:           result = "KW_VAR"; break;
            case TokenID::KW_FUNC:          result = "KW_FUNC"; break;
            case TokenID::KW_RETURN:        result = "KW_RETURN"; break;
            case TokenID::KW_IF:            result = "KW_IF"; break;
            case TokenID::KW_ELSE:          result = "KW_ELSE"; break;
            case TokenID::KW_FOR:           result = "KW_FOR"; break;
            case TokenID::KW_TRUE:          result = "KW_TRUE"; break;
            case TokenID::KW_FALSE:         result = "KW_FALSE"; break;
            case TokenID::KW_INT:           result = "KW_INT"; break;
            case TokenID::KW_BOOL:          result = "KW_BOOL"; break;
            case TokenID::KW_REF:           result = "KW_REF"; break;
            case TokenID::KW_PRINT:         result = "KW_PRINT"; break;
            case TokenID::KW_PRINTLN:       result = "KW_PRINTLN"; break;
        }

        return std::format("{:<16} -> \"{}\"", result, lexeme);
    }
};