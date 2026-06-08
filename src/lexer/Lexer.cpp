#include "Lexer.hpp"

Token Lexer::nextToken(){
    int state = 0;
    std::ostringstream  ss;

    do{
        switch(state){
            case 0:
                if(ch == EOF){
                    return { TokenID::ENDOFFILE, "<<EOF>>"};
                } else if(
                    (ch >= '[' && ch <= '^') || 
                    ch == '`' ||
                    (ch >= '~' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= '\b') ||
                    (ch >= '\v' && ch <= '\x1f') ||
                    (ch >= '#' && ch <= '$') ||
                    ch == '\'' ||
                    ch == '.' ||
                    (ch >= '?' && ch <= '@')
                ){
                    state = 1;
                    consume();
                } else if(ch == '\n'){
                    state = 15;
                    consume();
                } else if ( ch == '\t' || ch == '\x20'){
                    state = 2;
                    consume();
                } else if(ch == '!'){
                    state = 3;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '"'){
                    state = 4;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(
                    ( ch == '%') ||
                    (ch >= '(' && ch <= '-') || 
                    (ch == ';' ) ||
                    (ch == '{') ||
                    ( ch == '}')
                ){
                    state = 5;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '&'){
                    state= 6;
                    ss << static_cast<char>(ch);
                    consume();
                } else if( ch == '/'){
                    state = 7;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch >= '0' && ch <= '9'){
                    state = 8;
                    ss << static_cast<char>(ch);
                    consume();
                } else if (ch == ':'){
                    state = 9;
                    ss << static_cast<char>(ch);
                    consume();
                } else if (ch == '<'){
                    state = 10;
                    ss << static_cast<char>(ch);
                    consume();
                } else if (ch == '='){
                    state = 11;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '>'){
                    state = 12;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch == '_') ||
                    (ch >= 'a' && ch <= 'z')
                ){
                    state = 13;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '|'){
                    state = 14;
                    ss << static_cast<char>(ch); 
                    consume();
                } else {
                    ss << static_cast<char>(ch);
                    consume();
                    return {TokenID::UNK, ss.str()};
                }
                break;

            case 1:
                return {TokenID::UNK, ss.str()};
                break;
            case 2:
                if( ch == '\x20' || (ch >= '\t' && ch <= '\n')){
                    state = 15;
                    consume();
                } else {
                    state = 0;
                }
                break;
            case 3:
                if(ch == '='){
                    state = 16;
                    ss << static_cast<char>(ch);
                    consume();
                } else{
                    return {TokenID::OP_NOT, ss.str()};
                }
                break;

            case 4:
                if(ch == '"'){
                    state = 28;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(
                    (ch >= '#' && ch <= '[') ||
                    (ch <= ']' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= '!')
                ){
                    state = 17;
                    ss << static_cast<char>(ch);
                    consume();
                }
                break;

            case 5: {
                std::string lexeme = ss.str();
                if (lexeme == "+") return { TokenID::OP_ADD, lexeme };
                if (lexeme == "-") return { TokenID::OP_MINUS, lexeme };
                if (lexeme == "*") return { TokenID::OP_MUL, lexeme };
                if (lexeme == "%") return { TokenID::OP_MOD, lexeme };
                if (lexeme == ";") return { TokenID::SEMICOLON, lexeme };
                if (lexeme == ",") return { TokenID::COMMA, lexeme };
                if (lexeme == "(") return { TokenID::OPEN_PAR, lexeme };
                if (lexeme == ")") return { TokenID::CLOSE_PAR, lexeme };
                if (lexeme == "{") return { TokenID::OPEN_BRACE, lexeme };
                if (lexeme == "}") return { TokenID::CLOSE_BRACE, lexeme };
                
                return { TokenID::UNK, lexeme };
                break;
            }
            case 6:
                if(ch == '&'){
                    state = 18;
                    ss << static_cast<char>(ch);
                    consume();
                }else {
                    return {TokenID::OP_REF, ss.str()};
                }
                break;
            case 7:
                if(ch == '*'){
                    ss.str(""); 
                    ss.clear();

                    state = 19;
                    consume();
                } else if( ch == '/'){
                    ss.str(""); 
                    ss.clear();
                    
                    state = 20;
                    consume();
                } else {
                    return {TokenID::OP_DIV, ss.str()};
                }
                break;
            case 8:
                if( ch >= '0' && ch <= '9'){
                    state = 21;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return { TokenID::INT_LIT, ss.str()};
                }
                break;
            case 9:
                if(ch == '='){
                    state = 22;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return { TokenID::UNK, ss.str()};
                }
                break;
            case 10:
                if(ch == '='){
                    state = 23;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return {TokenID::OP_LT, ss.str()};
                }
                break;
            case 11:
                if(ch == '='){
                    state = 24;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return {TokenID::OP_ASSIGN, ss.str()};
                }
                break;
            case 12:
                if(ch == '='){
                    state = 25;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return {TokenID::OP_GT, ss.str()};
                }
                break;
            case 13: {
                if(
                    (ch >= '0' && ch <= '9') ||
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch >= 'a' && ch <= 'z') ||
                    ch == '_'
                ){
                    state = 26;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    auto it = keywords.find(ss.str());
                    if(it != keywords.end()){
                        return {it->second, ss.str()};
                    }

                    return {TokenID::IDENTIFIER, ss.str()};
                }
                break;
            }
            case 14:
                if(ch == '|'){
                    state = 27;
                    ss << static_cast<char>(ch);
                    consume();
                }else {
                    return {TokenID::UNK, ss.str()};
                }
                break;
            case 15:
                while(ch == '\x20' || ch >= '\t' && ch <= '\n'){
                    consume();
                }
                state = 0;
                break;
            case 16:
                return {TokenID::OP_NOT, ss.str()};
            case 17:
                while(
                    (ch >= ']' && ch <= 0xff) ||
                    (ch >= '#' && ch <= '[') ||
                    (ch >= '\0' && ch <= '!')
                ){
                    consume();
                }

                if(ch >= '\"'){
                    state = 28;
                    ss << static_cast<char>(ch);
                    consume();
                }

                else if(ch == '\\'){
                    state = 29;
                    consume();
                }
                break;
            
            case 18:
                return {TokenID::OP_AND, ss.str()};

            case 19:
                while(
                    (ch >= '+' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= ')')
                ){
                    consume();
                }

                if(ch == '*'){
                    state = 30;
                    consume();
                } else if(ch==EOF) {
                    return {TokenID::UNK, ss.str()}; 
                } else {
                    state = 19; 
                    consume();
                }
                break;

            case 20:
                while(
                    (ch >= '\v' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= '\t')
                ){
                    consume();
                }
                ss.str("");
                ss.clear();
                consume();

                state = 0;
                break;
            
            case 21:
                while(ch >= '0' && ch <= '9'){
                    ss << static_cast<char>(ch);
                    consume();
                }
                return {TokenID::INT_LIT, ss.str()};
            
            case 22:
                return {TokenID::OP_ASSIGN_SHORT, ss.str()};
                
            case 23:
                return {TokenID::OP_LEQ, ss.str()};
            
            case 24:
                return {TokenID::OP_EQ, ss.str()};
            
            case 25:
                return {TokenID::OP_GEQ, ss.str()};
            
            case 26: {
                while(
                    (ch >= '0' && ch <= '9') ||
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch >= 'a' && ch <= 'z') ||
                    ch == '_'
                ){
                    ss << static_cast<char>(ch);
                    consume();
                }

                auto it = keywords.find(ss.str());
                if(it != keywords.end()){
                    return {it->second, ss.str()};
                }

                return {TokenID::IDENTIFIER, ss.str()};
                break;
            }

            case 27:
                return {TokenID::OP_OR, ss.str()};
            
            case 28:
                return {TokenID::STRING_LIT, ss.str()};

            case 29:
                if(
                    (ch >= '\0' && ch <= '\t') ||
                    (ch >= '\v' && ch <= 0xff)
                ){
                    state = 17;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return { TokenID::UNK, ss.str()};
                }
                break;
            
            case 30:
                if(ch == '/'){
                    state = 31;
                    consume();
                } else if(ch == '*'){
                    state = 32;
                    consume();
                } else {
                    state = 19;
                    consume();
                }
                break;

            case 31:
                state = 0;
                ss.clear();
                ss.str("");
                break;

            case 32:
                if (ch == '*') {
                    state = 32;
                    consume();
                } else if (ch == '/') {
                    state = 31; 
                    consume();
                } else if (ch == EOF) {
                    return {TokenID::UNK, ss.str()};
                } else if(
                    (ch >= '0' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= ')') ||
                    (ch >= '+' && ch <= '.')
                ){
                    state = 19;
                    consume();
                }
                break;
        }   
    }while(true);
}