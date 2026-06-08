#include "Lexer.hpp"
#include "../utils/States.hpp"

Token Lexer::nextToken(){
    States state = States::INITIAL;
    std::ostringstream  ss;

    do{
        switch(state){
            case States::INITIAL:
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
                    state = States::UNK;
                    consume();
                } else if(ch == '\n'){
                    state = States::WHITESPACE;
                    consume();
                } else if ( ch == '\t' || ch == '\x20'){
                    state = States::WHITESPACE_LEAD;
                    consume();
                } else if(ch == '!'){
                    state = States::OP_NOT;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '"'){
                    state = States::STR_START;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(
                    ( ch == '%') ||
                    (ch >= '(' && ch <= '-') ||
                    (ch == ';' ) ||
                    (ch == '{') ||
                    ( ch == '}')
                ){
                    state = States::SINGLE_OP;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '&'){
                    state = States::AMPERSAND;
                    ss << static_cast<char>(ch);
                    consume();
                } else if( ch == '/'){
                    state = States::SLASH;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch >= '0' && ch <= '9'){
                    state = States::INT_FIRST;
                    ss << static_cast<char>(ch);
                    consume();
                } else if (ch == ':'){
                    state = States::COLON;
                    ss << static_cast<char>(ch);
                    consume();
                } else if (ch == '<'){
                    state = States::OP_LT;
                    ss << static_cast<char>(ch);
                    consume();
                } else if (ch == '='){
                    state = States::OP_ASSIGN;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '>'){
                    state = States::OP_GT;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch == '_') ||
                    (ch >= 'a' && ch <= 'z')
                ){
                    state = States::IDENT_FIRST;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '|'){
                    state = States::PIPE;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    ss << static_cast<char>(ch);
                    consume();
                    return {TokenID::UNK, ss.str()};
                }
                break;

            case States::UNK:
                return {TokenID::UNK, ss.str()};
                break;
            case States::WHITESPACE_LEAD:
                if( ch == '\x20' || (ch >= '\t' && ch <= '\n')){
                    state = States::WHITESPACE;
                    consume();
                } else {
                    state = States::INITIAL;
                }
                break;
            case States::OP_NOT:
                if(ch == '='){
                    state = States::OP_NEQ;
                    ss << static_cast<char>(ch);
                    consume();
                } else{
                    return {TokenID::OP_NOT, ss.str()};
                }
                break;

            case States::STR_START:
                if(ch == '"'){
                    state = States::STR_END;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(
                    (ch >= '#' && ch <= '[') ||
                    (ch >= ']' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= '!')
                ){
                    state = States::STR_REST;
                    ss << static_cast<char>(ch);
                    consume();
                } else if(ch == '\\'){
                    state = States::STR_ESCAPE;
                    ss << static_cast<char>(ch);
                    consume();
                }
                break;

            case States::SINGLE_OP: {
                auto it = single_ops.find(ss.str());
                if(it != single_ops.end()) return { it->second, ss.str()};
                
                return { TokenID::UNK, ss.str()};
                break;
            }

            case States::AMPERSAND:
                if(ch == '&'){
                    state = States::OP_AND;
                    ss << static_cast<char>(ch);
                    consume();
                }else {
                    return {TokenID::OP_REF, ss.str()};
                }
                break;
            case States::SLASH:
                if(ch == '*'){
                    ss.str("");
                    ss.clear();

                    state = States::BLOCK_COMMENT;
                    consume();
                } else if( ch == '/'){
                    ss.str("");
                    ss.clear();

                    state = States::LINE_COMMENT;
                    consume();
                } else {
                    return {TokenID::OP_DIV, ss.str()};
                }
                break;
            case States::INT_FIRST:
                if( ch >= '0' && ch <= '9'){
                    state = States::INT_REST;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return { TokenID::INT_LIT, ss.str()};
                }
                break;
            case States::COLON:
                if(ch == '='){
                    state = States::ASSIGN_SHORT;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return { TokenID::UNK, ss.str()};
                }
                break;
            case States::OP_LT:
                if(ch == '='){
                    state = States::OP_LEQ;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return {TokenID::OP_LT, ss.str()};
                }
                break;
            case States::OP_ASSIGN:
                if(ch == '='){
                    state = States::OP_EQ;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return {TokenID::OP_ASSIGN, ss.str()};
                }
                break;
            case States::OP_GT:
                if(ch == '='){
                    state = States::OP_GEQ;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return {TokenID::OP_GT, ss.str()};
                }
                break;
            case States::IDENT_FIRST: {
                if(
                    (ch >= '0' && ch <= '9') ||
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch >= 'a' && ch <= 'z') ||
                    ch == '_'
                ){
                    state = States::IDENT_REST;
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
            case States::PIPE:
                if(ch == '|'){
                    state = States::OP_OR;
                    ss << static_cast<char>(ch);
                    consume();
                }else {
                    return {TokenID::UNK, ss.str()};
                }
                break;
            case States::WHITESPACE:
                while(ch == '\x20' || ch >= '\t' && ch <= '\n'){
                    consume();
                }
                state = States::INITIAL;
                break;
            case States::OP_NEQ:
                return {TokenID::OP_NEQ, ss.str()};
            case States::STR_REST:
                while(
                    (ch >= ']' && ch <= 0xff) ||
                    (ch >= '#' && ch <= '[') ||
                    (ch >= '\0' && ch <= '!')
                ){
                    ss << static_cast<char>(ch);
                    consume();
                }

                if(ch == '"'){
                    state = States::STR_END;
                    ss << static_cast<char>(ch);
                    consume();
                }

                else if(ch == '\\'){
                    state = States::STR_ESCAPE;
                    ss << static_cast<char>(ch);
                    consume();
                }

                else if(ch == EOF){
                    return { TokenID::UNK, ss.str() };
                }
                break;

            case States::OP_AND:
                return {TokenID::OP_AND, ss.str()};

            case States::BLOCK_COMMENT:
                while(
                    (ch >= '+' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= ')')
                ){
                    consume();
                }

                if(ch == '*'){
                    state = States::BLOCK_COMMENT_STAR;
                    consume();
                } else if(ch==EOF) {
                    return {TokenID::UNK, ss.str()};
                } else {
                    state = States::BLOCK_COMMENT;
                    consume();
                }
                break;

            case States::LINE_COMMENT:
                while(
                    (ch >= '\v' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= '\t')
                ){
                    consume();
                }
                ss.str("");
                ss.clear();
                consume();

                state = States::INITIAL;
                break;

            case States::INT_REST:
                while(ch >= '0' && ch <= '9'){
                    ss << static_cast<char>(ch);
                    consume();
                }
                return {TokenID::INT_LIT, ss.str()};

            case States::ASSIGN_SHORT:
                return {TokenID::OP_ASSIGN_SHORT, ss.str()};

            case States::OP_LEQ:
                return {TokenID::OP_LEQ, ss.str()};

            case States::OP_EQ:
                return {TokenID::OP_EQ, ss.str()};

            case States::OP_GEQ:
                return {TokenID::OP_GEQ, ss.str()};

            case States::IDENT_REST: {
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

            case States::OP_OR:
                return {TokenID::OP_OR, ss.str()};

            case States::STR_END:
                return {TokenID::STRING_LIT, ss.str()};

            case States::STR_ESCAPE:
                if(
                    (ch >= '\0' && ch <= '\t') ||
                    (ch >= '\v' && ch <= 0xff)
                ){
                    state = States::STR_REST;
                    ss << static_cast<char>(ch);
                    consume();
                } else {
                    return { TokenID::UNK, ss.str()};
                }
                break;

            case States::BLOCK_COMMENT_STAR:
                if(ch == '/'){
                    state = States::BLOCK_COMMENT_END;
                    consume();
                } else if(ch == '*'){
                    state = States::BLOCK_COMMENT_STARS;
                    consume();
                } else {
                    state = States::BLOCK_COMMENT;
                    consume();
                }
                break;

            case States::BLOCK_COMMENT_END:
                state = States::INITIAL;
                ss.clear();
                ss.str("");
                break;

            case States::BLOCK_COMMENT_STARS:
                if (ch == '*') {
                    state = States::BLOCK_COMMENT_STARS;
                    consume();
                } else if (ch == '/') {
                    state = States::BLOCK_COMMENT_END;
                    consume();
                } else if (ch == EOF) {
                    return {TokenID::UNK, ss.str()};
                } else if(
                    (ch >= '0' && ch <= 0xff) ||
                    (ch >= '\0' && ch <= ')') ||
                    (ch >= '+' && ch <= '.')
                ){
                    state = States::BLOCK_COMMENT;
                    consume();
                }
                break;
        }
    }while(true);
}
