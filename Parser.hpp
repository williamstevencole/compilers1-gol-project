#pragma once
#include "Lexer.hpp"

class Parser{
    private:
        Lexer &l;
        Token currToken;

        void parseProgram();
        void parseTopDecl();
        void parseVarDecl();
        void parseType();
        void parseFuncDecl();
        void parseParamList();
        void parseParam();
        void parseBlock();
        void parseStmt();
        void parseStmtTail();
        void parseIfStmt();
        void parseForStmt();
        void parseReturnStmt();
        void parsePrintStmt();
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

        Parser(Lexer &l_) : l(l_) {
            currToken = l.nextToken();
        }

};