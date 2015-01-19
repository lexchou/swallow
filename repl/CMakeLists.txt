PROJECT(swallow_repl)
cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../swallow/includes
)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/../bin)


SET( CMAKE_BUILD_TYPE Debug )
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
cmake_policy(SET CMP0015 OLD)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++0x")

add_definitions(-DTRACE_NODE)

SET(REPL_SRC main.cpp ConsoleWriter.cpp REPL.cpp)

ADD_EXECUTABLE(repl main.cpp ${REPL_SRC})
target_link_libraries(repl swallow)


