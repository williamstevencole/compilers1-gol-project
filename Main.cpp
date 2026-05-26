#include "Lexer.hpp"
#include "Parser.hpp"

int main(){
    std::ifstream input("input.txt");
    if(!input.is_open()){
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    Lexer lexer(input);
    Parser parser(lexer);

    try {
        parser.parse();
        std::cout << "Parsing completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
    }

    return 0;
}