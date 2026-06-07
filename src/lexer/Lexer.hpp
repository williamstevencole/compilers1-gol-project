#include "Tokens.hpp"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>


class Lexer{
    private:
        void consume(){
            ch = input.get();
        }

        char ch;
        std::istream& input;

    public: 
        Lexer(std::istream& in): input(in) {
            consume();
        }

        Token nextToken();
};