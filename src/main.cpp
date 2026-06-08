#include "lexer/Lexer.hpp"


int main(){
    std::ifstream file("input.txt");
    if(!file){
        throw std::runtime_error("El archivo no existe");
    }

    Lexer l(file);
    Token currToken;

    do{
        currToken = l.nextToken();
        std::cout << currToken.toString() << "\n";
    }while(currToken.id != TokenID::ENDOFFILE);
}