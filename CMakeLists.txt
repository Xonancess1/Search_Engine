cmake_minimum_required(VERSION 3.24)
project(search_engine)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(search_engine Resources.qrc)

add_subdirectory(src)
add_subdirectory(mainWindow)
add_subdirectory(helpWindow)
add_subdirectory(Tests)

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/helpWindow)

target_link_libraries(search_engine PRIVATE Qt6::Widgets)
target_link_libraries(search_engine PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# GOOGLE_TEST --------------------------------------------------------------- {
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
FetchContent_MakeAvailable(googletest)
# GOOGLE_TEST --------------------------------------------------------------- }


# NLOHMANN::JSON --------------------------------------------------------------- {
include(FetchContent)
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
# NLOHMANN::JSON --------------------------------------------------------------- }


include(GNUInstallDirs)
install(TARGETS search_engine
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
