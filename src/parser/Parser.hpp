#pragma once

#include "../lexer/Lexer.hpp"

class Parser{
    private:
        Lexer& lexer;
        Token currToken;

        void consume(){
            currToken = lexer.nextToken();
        }

        void parseProgram();
        void parseTopDecl();
        void parseVarDecl();
        void parseType();
        void parseFuncDecl();
        void parseParamList();
        void parseParam();
        void parseBlock();
        void parseStmt();
        void parseForStmt();
        void parseReturnStmt();
        void parseBreakStmt();
        void parseContinueStmt();
        void parsePrintStmt();
        void parseIfStmt();
        void parseIfTail();
        void parseStmtTail();
        void parsePrintArg();
        void parseArgList();
        void parseArg();
        void parseExpr();
        void parseOrExpr();
        void parseAndExpr();
        void parseNotExpr();
        void parseRelExpr();
        void parseRelOp();
        void parseAddExpr();
        void parseMulExpr();
        void parseUnaryExpr();
        void parsePrimary();
        void parsePrimaryTail();

    public:
        void parse();

        Parser(Lexer &l): lexer(l){
            currToken = l.nextToken();
        }
};