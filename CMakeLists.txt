cmake_minimum_required(VERSION 3.16)
project(Setlist_Maxing)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(song_tree RedBlackTree.cpp)
target_include_directories(song_tree PUBLIC backend/include)

add_executable(Setlist_Maxing backend/src/main.cpp backend/src/SetlistGenerator.cpp hash_table.cpp)
target_link_libraries(Setlist_Maxing PRIVATE song_tree)

include(CTest)
if(BUILD_TESTING)
    add_executable(red_black_tree_tests tests/RedBlackTreeTests.cpp backend/src/SetlistGenerator.cpp)
    target_link_libraries(red_black_tree_tests PRIVATE song_tree)
    add_test(NAME red_black_tree_tests COMMAND red_black_tree_tests)
endif()
