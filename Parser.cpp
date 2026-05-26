#include "Parser.hpp"

void Parser::parse(){
    parseProgram();

    if(currToken.token != TokenID::ENDOFFILE){
        throw std::runtime_error("Unexpected token after end of program: " + currToken.lexeme);
    }
}

void Parser::parseProgram(){
    while(currToken.token != TokenID::ENDOFFILE){
        parseTopDecl();
    }
}

void Parser::parseTopDecl(){
    if(currToken.token == TokenID::VAR){
        parseVarDecl();
    }else if(currToken.token == TokenID::FUNC){
        parseFuncDecl();
    }else{
        throw std::runtime_error("Expected 'var' or 'func' at top level, got: " + currToken.lexeme);
    }
}

void Parser::parseVarDecl(){
    if(currToken.token != TokenID::VAR){
        throw std::runtime_error("Expected 'var' at start of variable declaration, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::IDENTIFIER){
        throw std::runtime_error("Expected identifier in variable declaration, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    parseType();

    if (currToken.token == TokenID::OP_ASSIGN){
        currToken = l.nextToken();
        parseExpr();
    }

    if(currToken.token != TokenID::SEMICOLON){
        throw std::runtime_error("Expected ';' at end of variable declaration, got: " + currToken.lexeme);
    }

    currToken = l.nextToken();
}

void Parser::parseType(){
    if(currToken.token == TokenID::INT_TYPE || currToken.token == TokenID::BOOL_TYPE){
        currToken = l.nextToken();
    }else{
        throw std::runtime_error("Expected type 'int' or 'bool', got: " + currToken.lexeme);
    }
}

void Parser::parseFuncDecl(){
    if(currToken.token != TokenID::FUNC){
        throw std::runtime_error("Expected 'func' at start of function declaration, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::IDENTIFIER){
        throw std::runtime_error("Expected identifier in function declaration, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::LPAREN){
        throw std::runtime_error("Expected '(' after function name, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::RPAREN){
        parseParamList();
        if(currToken.token != TokenID::RPAREN){
            throw std::runtime_error("Expected ')' after parameter list, got: " + currToken.lexeme);
        }
    }
    currToken = l.nextToken();

    if(currToken.token == TokenID::INT_TYPE || currToken.token == TokenID::BOOL_TYPE){
        parseType();
    }

    parseBlock();
}

void Parser::parseParamList(){
    parseParam();
    while(currToken.token == TokenID::COMMA){
        currToken = l.nextToken();
        parseParam();
    }
}

void Parser::parseParam(){
    if(currToken.token == TokenID::REF){
        currToken = l.nextToken();
    }
    if(currToken.token != TokenID::IDENTIFIER){
        throw std::runtime_error("Expected identifier in parameter declaration, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    parseType();
}

void Parser::parseBlock(){
    if(currToken.token != TokenID::LBRACE){
        throw std::runtime_error("Expected '{' at start of block, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    while(currToken.token != TokenID::RBRACE){
        parseStmt();
    }

    currToken = l.nextToken();
}

void Parser::parseStmt(){
    if(currToken.token == TokenID::VAR){
        parseVarDecl();
    }else if(currToken.token == TokenID::IF){
        parseIfStmt();
    }else if(currToken.token == TokenID::FOR){
        parseForStmt();
    }else if(currToken.token == TokenID::RETURN){
        parseReturnStmt();
    }else if(currToken.token == TokenID::PRINT || currToken.token == TokenID::PRINTLN){
        parsePrintStmt();
    }else if(currToken.token == TokenID::IDENTIFIER){
        currToken = l.nextToken();
        parseStmtTail();
    }else{
        throw std::runtime_error("Unexpected token in statement: " + currToken.lexeme);
    }
}

void Parser::parseStmtTail(){
    if(currToken.token == TokenID::OP_ASSIGN || currToken.token == TokenID::ASSIGN_SHORT){
        currToken = l.nextToken();
        parseExpr();
        if(currToken.token != TokenID::SEMICOLON){
            throw std::runtime_error("Expected ';' at end of assignment statement, got: " + currToken.lexeme);
        }
        currToken = l.nextToken();
    }else if(currToken.token == TokenID::LPAREN){
        currToken = l.nextToken();
        if(currToken.token != TokenID::RPAREN){
            parseArgList();
            if(currToken.token != TokenID::RPAREN){
                throw std::runtime_error("Expected ')' after argument list in call statement, got: " + currToken.lexeme);
            }
        }
        currToken = l.nextToken();
        if(currToken.token != TokenID::SEMICOLON){
            throw std::runtime_error("Expected ';' at end of call statement, got: " + currToken.lexeme);
        }
        currToken = l.nextToken();
    }else{
        throw std::runtime_error("Expected ':=', '=' or '(' after identifier in statement, got: " + currToken.lexeme);
    }
}

void Parser::parseIfStmt(){
    if(currToken.token != TokenID::IF){
        throw std::runtime_error("Expected 'if' at start of if statement, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    parseExpr();
    parseBlock();

    while(currToken.token == TokenID::ELSE){
        currToken = l.nextToken();
        if(currToken.token == TokenID::IF){
            currToken = l.nextToken();
            parseExpr();
            parseBlock();
        }else{
            parseBlock();
            break;
        }
    }

    if(currToken.token == TokenID::ELSE){
        currToken = l.nextToken();
        parseBlock();
    }
}

void Parser::parseForStmt(){
    if(currToken.token != TokenID::FOR){
        throw std::runtime_error("Expected 'for' at start of for statement, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    parseExpr();
    parseBlock();
}

void Parser::parseReturnStmt(){
    if(currToken.token != TokenID::RETURN){
        throw std::runtime_error("Expected 'return' at start of return statement, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::SEMICOLON){
        parseExpr();
    }

    if(currToken.token != TokenID::SEMICOLON){
        throw std::runtime_error("Expected ';' at end of return statement, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();
}

void Parser::parsePrintStmt(){
    if(currToken.token != TokenID::PRINT && currToken.token != TokenID::PRINTLN){
        throw std::runtime_error("Expected 'print' or 'println' at start of print statement, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::LPAREN){
        throw std::runtime_error("Expected '(' after 'print' or 'println', got: " + currToken.lexeme);
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::RPAREN){
        parsePrintArg();
        while(currToken.token == TokenID::COMMA){
            currToken = l.nextToken();
            parsePrintArg();
        }
        if(currToken.token != TokenID::RPAREN){
            throw std::runtime_error("Expected ')' after print arguments, got: " + currToken.lexeme);
        }
    }
    currToken = l.nextToken();

    if(currToken.token != TokenID::SEMICOLON){
        throw std::runtime_error("Expected ';' at end of print statement, got: " + currToken.lexeme);
    }
    currToken = l.nextToken();
}

void Parser::parsePrintArg(){
    if(currToken.token == TokenID::STRING_LIT){
        currToken = l.nextToken();
    }else{
        parseExpr();
    }
}

void Parser::parseArgList(){
    parseArg();
    while(currToken.token == TokenID::COMMA){
        currToken = l.nextToken();
        parseArg();
    }
}

void Parser::parseArg(){
    if(currToken.token == TokenID::AMPERSAND){
        currToken = l.nextToken();
    }

    parseExpr();
}

void Parser::parseExpr(){
    parseOrExpr();
}

void Parser::parseOrExpr(){
    parseAndExpr();
    while(currToken.token == TokenID::OP_OR){
        currToken = l.nextToken();
        parseAndExpr();
    }
}

void Parser::parseAndExpr(){
    parseNotExpr();
    while(currToken.token == TokenID::OP_AND){
        currToken = l.nextToken();
        parseNotExpr();
    }
}

void Parser::parseNotExpr(){
    if(currToken.token == TokenID::OP_NOT){
        currToken = l.nextToken();
        parseNotExpr();
    }else{
        parseRelExpr();
    }
}

void Parser::parseRelExpr(){
    parseAddExpr();
    if(
        currToken.token == TokenID::OP_EQ || 
        currToken.token == TokenID::OP_NEQ || 
        currToken.token == TokenID::OP_LT || 
        currToken.token == TokenID::OP_GT || 
        currToken.token == TokenID::OP_LEQ || 
        currToken.token == TokenID::OP_GEQ
    ){
        parseRelOp();
        parseAddExpr();
    }
}

void Parser::parseRelOp(){
    if(
        currToken.token == TokenID::OP_EQ || 
        currToken.token == TokenID::OP_NEQ || 
        currToken.token == TokenID::OP_LT || 
        currToken.token == TokenID::OP_GT || 
        currToken.token == TokenID::OP_LEQ || 
        currToken.token == TokenID::OP_GEQ
    ){
        currToken = l.nextToken();
    }else{
        throw std::runtime_error("Expected relational operator, got: " + currToken.lexeme);
    }
}

void Parser::parseAddExpr(){
    parseMulExpr();
    while(currToken.token == TokenID::PLUS || currToken.token == TokenID::MINUS){
        currToken = l.nextToken();
        parseMulExpr();
    }
}

void Parser::parseMulExpr(){
    parseUnaryExpr();
    while(
        currToken.token == TokenID::MULT || 
        currToken.token == TokenID::DIV || 
        currToken.token == TokenID::MOD
    ){
        currToken = l.nextToken();
        parseUnaryExpr();
    }
}

void Parser::parseUnaryExpr(){
    if(currToken.token == TokenID::MINUS){
        currToken = l.nextToken();
        parseUnaryExpr();
    }else{
        parsePrimary();
    }
}

void Parser::parsePrimary(){
    /*
    INT_LIT
    | "true"
    | "false"
    | IDENTIFIER
    | callExpr
    | "(" expr ")" ;
    */
    
    if(currToken.token == TokenID::INT_LIT || currToken.token == TokenID::TRUE || currToken.token == TokenID::FALSE){
        currToken = l.nextToken();
    }else if(currToken.token == TokenID::IDENTIFIER){
        currToken = l.nextToken();
        parsePrimaryTail();
    } else if(currToken.token == TokenID::LPAREN){
        currToken = l.nextToken();
        parseExpr();
        if(currToken.token != TokenID::RPAREN){
            throw std::runtime_error("Expected ')' after expression, got: " + currToken.lexeme);
        }
        currToken = l.nextToken();
    }else{
        throw std::runtime_error("Unexpected token in expression: " + currToken.lexeme);
    }
}

void Parser::parsePrimaryTail(){
    if(currToken.token == TokenID::LPAREN){
        currToken = l.nextToken();
        if(currToken.token != TokenID::RPAREN){
            parseArgList();
            if(currToken.token != TokenID::RPAREN){
                throw std::runtime_error("Expected ')' after argument list in call expression, got: " + currToken.lexeme);
            }
        }
        currToken = l.nextToken();
    }
}
