#pragma once

enum class States {
    INITIAL, // 0
    UNK, // 1
    WHITESPACE_LEAD, // 2
    OP_NOT, // 3
    STR_START, // 4
    SINGLE_OP, // 5
    AMPERSAND, // 6
    SLASH, // 7
    INT_FIRST, // 8
    COLON, // 9
    OP_LT, // 10 
    OP_ASSIGN, // 11
    OP_GT, // 12
    IDENT_FIRST, // 13
    PIPE, // 14
    WHITESPACE, // 15
    OP_NEQ, // 16
    STR_REST, // 17
    OP_AND, // 18
    BLOCK_COMMENT,  // 19
    LINE_COMMENT, // 20
    INT_REST, // 21
    ASSIGN_SHORT, // 22
    OP_LEQ, // 23
    OP_EQ, // 24
    OP_GEQ, // 25 
    IDENT_REST, // 26
    OP_OR, // 27
    STR_END, // 28
    STR_ESCAPE, // 29
    BLOCK_COMMENT_STAR, // 30
    BLOCK_COMMENT_END, // 31
    BLOCK_COMMENT_STARS // 32
};
