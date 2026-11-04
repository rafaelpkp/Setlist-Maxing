cmake_minimum_required(VERSION 3.16)
project(SetlistMaxing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(setlist_core
    src/SetlistGenerator.cpp
    src/SongLoader.cpp
)
target_include_directories(setlist_core PUBLIC include)

add_executable(setlist src/main.cpp)
target_link_libraries(setlist PRIVATE setlist_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(setlist_tests tests/SetListGeneratorTests.cpp)
    target_link_libraries(setlist_tests PRIVATE setlist_core)
    add_test(NAME setlist_tests COMMAND setlist_tests)

    add_executable(song_loader_tests tests/SongLoaderTests.cpp)
    target_link_libraries(song_loader_tests PRIVATE setlist_core)
    add_test(NAME song_loader_tests COMMAND song_loader_tests)
    set_tests_properties(song_loader_tests PROPERTIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()
