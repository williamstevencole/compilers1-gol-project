#include "Lexer.hpp"

Token Lexer::nextToken(){
    std::ostringstream ss;
    State state = State::INITIAL;
    
    while(true){
        switch(state){
            case State::INITIAL:
                if(ch == EOF){
                    return { TokenID::ENDOFFILE, "<<EOF>>"};
                }else if(
                    (ch >= '\0' && ch <= '\b') ||
                    (ch >= '\v' && ch <= '\x1f') || 
                    (ch >= '#' && ch <= '$') ||
                    (ch == '\'' || ch == '.') ||
                    (ch >= '?' && ch <= '@') ||
                    (ch >= '[' && ch <= '^') ||
                    (ch >= '`') ||
                    (ch >= '~' && ch <= '\xff')
                ){
                    state = State::UNK;
                    ss << ch;
                    consume();
                }else if(ch == '\t' || ch == '\n' || ch == '\x20' || ch == '\r'){
                    state = State::WHITESPACE;
                    consume();
                }else if(ch == '!'){
                    state = State::OP_NOT;
                    ss << ch;
                    consume();
                }else if(ch == '"'){
                    state = State::STR_START;
                    ss << ch;
                    consume();
                }else if(ch == '&'){
                    state = State::AMPERSAND;
                    ss << ch;
                    consume();
                }else if(ch >= '0' && ch <= '9'){
                    state = State::INT_FIRST;
                    ss << ch;
                    consume();
                }else if(ch == ':'){
                    state = State::COLON;
                    ss << ch;
                    consume();
                }else if(ch == '<'){
                    state = State::OP_LT;
                    ss << ch;
                    consume();
                }else if(ch == '='){
                    state = State::OP_ASSIGN;
                    ss << ch;
                    consume();
                }else if(ch == '>'){
                    state = State::OP_GT;
                    ss << ch;
                    consume();
                }else if(
                    ch == '_' ||
                    (ch >= 'a' && ch <= 'z') ||
                    (ch >= 'A' && ch <= 'Z')
                ){
                    state = State::IDENT_FIRST;
                    ss << ch;
                    consume();
                }else if(ch == '|'){
                    state = State::PIPE;
                    ss << ch;
                    consume();
                }else {
                    auto it = singleOps.find(ch);
                    if(it != singleOps.end()){
                        ss << ch;
                        consume();
                        return { it->second, ss.str() };
                    }else{
                        state = State::UNK;
                        ss << ch;
                        consume();
                    }
                }
                break;

            case State::UNK:
                return {TokenID::UNK, ss.str()};

            case State::WHITESPACE:
                state = State::INITIAL;
                break;

            case State::OP_NOT:
                if(ch != '='){
                    return {TokenID::OP_NOT, ss.str()};
                }else{
                    ss << ch;
                    consume();
                    return {TokenID::OP_NEQ, ss.str()};
                }
                break;

            case State::STR_START:
            case State::STR_REST:
                if(ch == '"'){
                    ss << ch;
                    consume();
                    return {TokenID::STRING_LIT, ss.str()};
                }else if(ch == '\\'){
                    state = State::STR_ESCAPE;
                    ss << ch;
                    consume();
                }else if(ch == EOF){
                    return {TokenID::UNK, ss.str()};
                }else{
                    state = State::STR_REST;
                    ss << ch;
                    consume();
                }
                break;

            case State::STR_ESCAPE:
                if(ch == EOF){
                    return {TokenID::UNK, ss.str()};
                }
                state = State::STR_REST;
                ss << ch;
                consume();
                break;

            case State::AMPERSAND:
                if(ch != '&'){
                    return {TokenID::AMPERSAND, ss.str()};
                }else{
                    ss << ch;
                    consume();
                    return {TokenID::OP_AND, ss.str()};
                }
                break;

            case State::INT_FIRST:
            case State::INT_REST:
                if(ch >= '0' && ch <= '9'){
                    state = State::INT_REST;
                    ss << ch;
                    consume();
                }else{
                    return {TokenID::INT_LIT, ss.str()};
                }
                break;

            case State::COLON:
                if(ch == '='){
                    ss << ch;
                    consume();
                    return {TokenID::ASSIGN_SHORT, ss.str()};
                }else{
                    return {TokenID::UNK, ss.str()};
                }
                break;

            case State::OP_LT:
                if(ch == '='){
                    ss << ch;
                    consume();
                    return {TokenID::OP_LEQ, ss.str()};
                }else{
                    return {TokenID::OP_LT, ss.str()};
                }
                break;

            case State::OP_ASSIGN:
                if(ch == '='){
                    ss << ch;
                    consume();
                    return {TokenID::OP_EQ, ss.str()};
                }else{
                    return {TokenID::OP_ASSIGN, ss.str()};
                }
                break;

            case State::OP_GT:
                if(ch == '='){
                    ss << ch;
                    consume();
                    return {TokenID::OP_GEQ, ss.str()};
                }else{
                    return {TokenID::OP_GT, ss.str()};
                }
                break;

            case State::IDENT_FIRST:
            case State::IDENT_REST:
                if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_'){
                    state = State::IDENT_REST;
                    ss << ch;
                    consume();
                }else{
                    std::string lexeme = ss.str();
                    auto it = keywords.find(lexeme);
                    if(it != keywords.end()){
                        return {it->second, lexeme};
                    }else{
                        return {TokenID::IDENTIFIER, lexeme};
                    }
                }
                break;

            case State::PIPE:
                if(ch == '|'){
                    ss << ch;
                    consume();
                    return {TokenID::OP_OR, ss.str()};
                }else{
                    return {TokenID::UNK, ss.str()};
                }
                break;
        }
    }
}