cmake_minimum_required(VERSION 3.15)
project(calc)

# set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

# find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_link_options(-lm)

add_executable(calc
        src/main.c
        src/error.c
        src/error.h
        src/interface.c
        src/interface.h
        src/calculate.c
        src/calculate.h
        src/list.c
        src/list.h
        src/parser.c
        src/parser.h
        src/scanner.c
        src/scanner.h)

# target_link_libraries(sort Qt5::Widgets)
