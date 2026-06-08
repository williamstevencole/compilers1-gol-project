#include "Parser.hpp"
#include <stdexcept>

// program = { topDecl } EOF
void Parser::parse() {
    parseProgram();
    if (currToken.id != TokenID::ENDOFFILE) {
        throw std::runtime_error("Syntax error: Exp: " + currToken.lexeme);
    }
}

// program = { topDecl } EOF
void Parser::parseProgram() {
    while (currToken.id != TokenID::ENDOFFILE) {
        parseTopDecl();
    }
}

// topDecl = varDecl | funcDecl
void Parser::parseTopDecl() {
    if (currToken.id == TokenID::KW_VAR) {
        parseVarDecl();
    } else if (currToken.id == TokenID::KW_FUNC) {
        parseFuncDecl();
    } else {
        throw std::runtime_error("Syntax error: Expected {var , func} but got: " + currToken.lexeme);
    }
}

// varDecl = "var" IDENTIFIER type [ "=" expr ] ";"
void Parser::parseVarDecl() {
    if (currToken.id != TokenID::KW_VAR) {
        throw std::runtime_error("Syntax error: Expected {var} but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::IDENTIFIER) {
        throw std::runtime_error("Syntax error: Expected {IDENTIFIER} but got: " + currToken.lexeme);
    }
    consume();

    parseType();

    if (currToken.id == TokenID::OP_ASSIGN) {
        consume();
        parseExpr();
    }

    if (currToken.id != TokenID::SEMICOLON) {
        throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
    }
    consume();
}

// type = "int" | "bool"
void Parser::parseType() {
    if (currToken.id == TokenID::KW_INT || currToken.id == TokenID::KW_BOOL) {
        consume();
    } else {
        throw std::runtime_error("Syntax error: Expected { int, bool} but got: " + currToken.lexeme);
    }
}

// funcDecl = "func" IDENTIFIER "(" [ paramList ] ")" [ type ] block
void Parser::parseFuncDecl() {
    if (currToken.id != TokenID::KW_FUNC) {
        throw std::runtime_error("Syntax error: Expected { func } but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::IDENTIFIER) {
        throw std::runtime_error("Syntax error: Expected { IDENTIFIER } but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::OPEN_PAR) {
        throw std::runtime_error("Syntax error: Expected { ( } but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::CLOSE_PAR) {
        parseParamList();
        if (currToken.id != TokenID::CLOSE_PAR) {
            throw std::runtime_error("Syntax error: Expected { ) } but got: " + currToken.lexeme);
        }
    }
    consume();

    if (currToken.id == TokenID::KW_INT || currToken.id == TokenID::KW_BOOL) {
        parseType();
    }

    parseBlock();
}

// paramList = param { ","  param }
void Parser::parseParamList() {
    parseParam();
    while (currToken.id == TokenID::COMMA) {
        consume();
        parseParam();
    }
}

// param = [ "ref" ] IDENTIFIER type
void Parser::parseParam() {
    if (currToken.id == TokenID::KW_REF) {
        consume();
    }

    if (currToken.id != TokenID::IDENTIFIER) {
        throw std::runtime_error("Syntax error: Expected { IDENTIFIER } but got: " + currToken.lexeme);
    }
    consume();

    parseType();
}

// block = "{" { stmt } "}"
void Parser::parseBlock() {
    if (currToken.id != TokenID::OPEN_BRACE) {
        throw std::runtime_error("Syntax error: Expected { { } but got: " + currToken.lexeme);
    }
    consume();

    while (currToken.id != TokenID::CLOSE_BRACE && currToken.id != TokenID::ENDOFFILE) {
        parseStmt();
    }

    if (currToken.id != TokenID::CLOSE_BRACE) {
        throw std::runtime_error("Syntax error: Expected { } } but got: " + currToken.lexeme);
    }
    consume();
}

// stmt = IDENTIFIER stmtTail | varDecl | ifStmt | forStmt | returnStmt | printStmt | breakStmt | continueStmt
void Parser::parseStmt() {
    if (currToken.id == TokenID::IDENTIFIER) {
        consume();
        parseStmtTail();
    } else if (currToken.id == TokenID::KW_VAR) {
        parseVarDecl();
    } else if (currToken.id == TokenID::KW_IF) {
        parseIfStmt();
    } else if (currToken.id == TokenID::KW_FOR) {
        parseForStmt();
    } else if (currToken.id == TokenID::KW_RETURN) {
        parseReturnStmt();
    } else if (currToken.id == TokenID::KW_PRINT || currToken.id == TokenID::KW_PRINTLN) {
        parsePrintStmt();
    } else if(currToken.id == TokenID::KW_BREAK){
        parseBreakStmt();
    } else if(currToken.id == TokenID::KW_CONTINUE){
        parseContinueStmt();
    } else {
        throw std::runtime_error("Syntax error: Expected { Identifier, var, if, for, return, print, println, break, continue } but got: " + currToken.lexeme);
    }
}

void Parser::parseBreakStmt(){
    consume();
    if(currToken.id != TokenID::SEMICOLON){
        throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
    }
    consume();
}

void Parser::parseContinueStmt(){
    consume();
    if(currToken.id != TokenID::SEMICOLON){
        throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
    }
    consume();
}

// ifStmt = "if" expr block [ "else" ifTail ]
void Parser::parseIfStmt() {
    if (currToken.id != TokenID::KW_IF) {
        throw std::runtime_error("Syntax error: Expected { if } but got: " + currToken.lexeme);
    }
    consume();

    parseExpr();
    parseBlock();

    if (currToken.id == TokenID::KW_ELSE) {
        consume();
        parseIfTail();
    }
}

// ifTail = "if" expr block [ "else" ifTail ] | block
void Parser::parseIfTail() {
    if (currToken.id == TokenID::KW_IF) {
        consume();
        parseExpr();
        parseBlock();
        if (currToken.id == TokenID::KW_ELSE) {
            consume();
            parseIfTail();
        }
    } else if (currToken.id == TokenID::OPEN_BRACE) {
        parseBlock();
    } else {
        throw std::runtime_error("Syntax error: Expected { if, { } after 'else' but got: " + currToken.lexeme);
    }
}

// stmtTail = ":=" expr ";" | "=" expr ";" | "(" [ argList ] ")" ";"
void Parser::parseStmtTail() {
    if (currToken.id == TokenID::OP_ASSIGN_SHORT || currToken.id == TokenID::OP_ASSIGN) {
        consume();
        parseExpr();
        if (currToken.id != TokenID::SEMICOLON) {
            throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
        }
        consume();
    } else if (currToken.id == TokenID::OPEN_PAR) {
        consume();
        if (currToken.id != TokenID::CLOSE_PAR) {
            parseArgList();
            if (currToken.id != TokenID::CLOSE_PAR) {
                throw std::runtime_error("Syntax error: Expected { ) } but got: " + currToken.lexeme);
            }
        }
        consume();
        if (currToken.id != TokenID::SEMICOLON) {
            throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
        }
        consume();
    } else {
        throw std::runtime_error("Syntax error: Expected { := , = , ( } after identifier but got: " + currToken.lexeme);
    }
}

// forStmt = "for" expr block
void Parser::parseForStmt() {
    if (currToken.id != TokenID::KW_FOR) {
        throw std::runtime_error("Syntax error: Expected { for } but got: " + currToken.lexeme);
    }
    consume();

    parseExpr();
    parseBlock();
}

// returnStmt = "return" [ expr ] ";"
void Parser::parseReturnStmt() {
    if (currToken.id != TokenID::KW_RETURN) {
        throw std::runtime_error("Syntax error: Expected { return } but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::SEMICOLON) {
        parseExpr();
    }

    if (currToken.id != TokenID::SEMICOLON) {
        throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
    }
    consume();
}

// printStmt = ( "print" | "println" ) "(" printArg { "," printArg } ")" ";"
void Parser::parsePrintStmt() {
    if (currToken.id != TokenID::KW_PRINT && currToken.id != TokenID::KW_PRINTLN) {
        throw std::runtime_error("Syntax error: Expected { print, println } but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::OPEN_PAR) {
        throw std::runtime_error("Syntax error: Expected { ( } but got: " + currToken.lexeme);
    }
    consume();

    parsePrintArg();
    while (currToken.id == TokenID::COMMA) {
        consume();
        parsePrintArg();
    }

    if (currToken.id != TokenID::CLOSE_PAR) {
        throw std::runtime_error("Syntax error: Expected { ) } but got: " + currToken.lexeme);
    }
    consume();

    if (currToken.id != TokenID::SEMICOLON) {
        throw std::runtime_error("Syntax error: Expected { ; } but got: " + currToken.lexeme);
    }
    consume();
}

// printArg = expr | STRING_LIT
void Parser::parsePrintArg() {
    if (currToken.id == TokenID::STRING_LIT) {
        consume();
    } else {
        parseExpr();
    }
}

// argList = arg { "," arg }
void Parser::parseArgList() {
    parseArg();
    while (currToken.id == TokenID::COMMA) {
        consume();
        parseArg();
    }
}

// arg = [ "&" ] expr
void Parser::parseArg() {
    if (currToken.id == TokenID::OP_REF) {
        consume();
    }
    parseExpr();
}

// expr = orExpr
void Parser::parseExpr() {
    parseOrExpr();
}

// orExpr = andExpr { "||" andExpr }
void Parser::parseOrExpr() {
    parseAndExpr();
    while (currToken.id == TokenID::OP_OR) {
        consume();
        parseAndExpr();
    }
}

// andExpr = notExpr { "&&" notExpr }
void Parser::parseAndExpr() {
    parseNotExpr();
    while (currToken.id == TokenID::OP_AND) {
        consume();
        parseNotExpr();
    }
}

// notExpr = "!" notExpr | relExpr
void Parser::parseNotExpr() {
    if (currToken.id == TokenID::OP_NOT) {
        consume();
        parseNotExpr();
    } else {
        parseRelExpr();
    }
}

// relExpr = addExpr [ relOp addExpr ]
void Parser::parseRelExpr() {
    parseAddExpr();
    if (
        currToken.id == TokenID::OP_EQ  || currToken.id == TokenID::OP_NEQ || 
        currToken.id == TokenID::OP_LT  || currToken.id == TokenID::OP_GT  || 
        currToken.id == TokenID::OP_LEQ || currToken.id == TokenID::OP_GEQ
    ) {
        parseRelOp();
        parseAddExpr();
    }
}

// relOp = "==" | "!=" | "<" | ">" | "<=" | ">="
void Parser::parseRelOp() {
    if (
        currToken.id == TokenID::OP_EQ  || currToken.id == TokenID::OP_NEQ || 
        currToken.id == TokenID::OP_LT  || currToken.id == TokenID::OP_GT  || 
        currToken.id == TokenID::OP_LEQ || currToken.id == TokenID::OP_GEQ
    ) {
        consume();
    } else {
        throw std::runtime_error("Syntax error: Expected { ==, !=, <, >, <=, >= } but got: " + currToken.lexeme);
    }
}

// addExpr = mulExpr { ( "+" | "-" ) mulExpr }
void Parser::parseAddExpr() {
    parseMulExpr();
    while (currToken.id == TokenID::OP_ADD || currToken.id == TokenID::OP_MINUS) {
        consume();
        parseMulExpr();
    }
}

// mulExpr = unaryExpr { ( "*" | "/" | "%" ) unaryExpr }
void Parser::parseMulExpr() {
    parseUnaryExpr();
    while (
        currToken.id == TokenID::OP_MUL || 
        currToken.id == TokenID::OP_DIV || 
        currToken.id == TokenID::OP_MOD
    ) {
        consume();
        parseUnaryExpr();
    }
}

// unaryExpr = "-" unaryExpr | primary
void Parser::parseUnaryExpr() {
    if (currToken.id == TokenID::OP_MINUS) {
        consume();
        parseUnaryExpr();
    } else {
        parsePrimary();
    }
}

// primary = INT_LIT | "true" | "false" | IDENTIFIER [ primaryTail ] | "(" expr ")"
void Parser::parsePrimary() {
    if (currToken.id == TokenID::INT_LIT  || 
        currToken.id == TokenID::KW_TRUE  || 
        currToken.id == TokenID::KW_FALSE) {
        consume();
    } else if (currToken.id == TokenID::IDENTIFIER) {
        consume();
        parsePrimaryTail();
    } else if (currToken.id == TokenID::OPEN_PAR) {
        consume();
        parseExpr();
        if (currToken.id != TokenID::CLOSE_PAR) {
            throw std::runtime_error("Syntax error: Expected { ) } but got: " + currToken.lexeme);
        }
        consume();
    } else {
        throw std::runtime_error("Syntax error: Expected literal, identifier or { ( } but got: " + currToken.lexeme);
    }
}

// primaryTail = "(" [ argList ] ")"
void Parser::parsePrimaryTail() {
    if (currToken.id == TokenID::OPEN_PAR) {
        consume();
        if (currToken.id != TokenID::CLOSE_PAR) {
            parseArgList();
            if (currToken.id != TokenID::CLOSE_PAR) {
                throw std::runtime_error("Syntax error: Expected { ) } but got: " + currToken.lexeme);
            }
        }
        consume();
    }
}