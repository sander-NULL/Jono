cmake_minimum_required(VERSION 3.10.0)
project(Jono VERSION 0.1.0 LANGUAGES C)

add_executable(Jono alphabeta.c bitboards.c debug.c main.c movgen.c timemgmt.c)

add_executable(perft Perft/perft.c bitboards.c movgen.c)

add_executable(compare "Search test/compare.c")

add_executable(searchtest alphabeta.c bitboards.c movgen.c "Search test/searchtest.c")

set_source_files_properties(alphabeta.c PROPERTIES COMPILE_DEFINITIONS SEARCHTEST)