cmake_minimum_required(VERSION 3.24)
project(SearchEngineTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SearchEngineTests
    InverterIndexTest.cpp
    SearchServerTest.cpp
    ${MY_SRC_DIR}/InvertedIndex.cpp
    ${MY_SRC_DIR}/SearchServer.cpp
)

enable_testing()
target_link_libraries(SearchEngineTests PRIVATE gtest_main nlohmann_json::nlohmann_json)
target_include_directories(SearchEngineTests PUBLIC ${MY_INCLUDE_DIR})

include(GoogleTest)
gtest_discover_tests(SearchEngineTests)
