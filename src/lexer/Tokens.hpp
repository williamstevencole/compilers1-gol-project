#include "string"

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

struct Token{
    TokenID id;
    std::string lexeme;
};