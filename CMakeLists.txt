cmake_minimum_required(VERSION 3.20)

project(Project3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project3
        main.cpp
        Parser.cpp
        Interpreter.cpp
        Scanner.cpp
        Token.cpp
        Relation.cpp
)