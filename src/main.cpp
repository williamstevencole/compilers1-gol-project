#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(){
    std::ifstream file("input.txt");
    if(!file){
        throw std::runtime_error("El archivo no existe");
    }

    Lexer l(file);
    Parser p(l);
    Token currToken;

    /*
    do{
        currToken = l.nextToken();
        std::cout << currToken.toString() << "\n";
    }while(currToken.id != TokenID::ENDOFFILE);
    */
    
    try{
        p.parse();
        std::cout << "code is valid :D";
    }catch(const std::runtime_error& error){
        std::cerr << error.what() << "\n";
    }
}