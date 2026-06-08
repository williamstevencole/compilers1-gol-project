cmake_minimum_required(VERSION 3.20)
project(Gol)

set(CMAKE_CXX_STANDARD 20)
add_executable(Gol
    main.cpp 
    lexer/lexer.hpp 
    lexer/lexer.cpp
    parser/parser.hpp
    parser/parser.cpp
)