cmake_minimum_required(VERSION 3.10.0)
project(Jono VERSION 0.1.0 LANGUAGES C)

add_executable(Jono alphabeta.c bitboards.c debug.c main.c movgen.c timemgmt.c)

#add_executable(perft Perft/perft.c bitboards.c movgen.c)

#add_executable(compare "Search test/compare.c")

#add_executable(searchtest alphabeta.c bitboards.c movgen.c "Search test/searchtest.c")

#add_executable(searchtest bitboards.c movgen.c "Search test/searchtest.c")

# Compile alphabeta.c separately for searchtest with the SEARCHTEST macro
add_library(alphabeta_searchtest OBJECT alphabeta.c)
target_compile_definitions(alphabeta_searchtest PRIVATE SEARCHTEST)

# Link the specially compiled alphabeta.o to searchtest
#target_link_libraries(searchtest PRIVATE alphabeta_searchtest)