#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(int argc, char* argv[]){

    if(argc < 2){
        std::cerr << "Usage: "<< argv[0] << " <filename>";
    }
    std::ifstream file(argv[1]);

    if(!file){
        std::cerr << "Error opening file: " << argv[1];
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