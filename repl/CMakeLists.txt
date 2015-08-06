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
cmake_policy(SET CMP0054 OLD)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g -std=c++11")
    SET(GTEST_LIBS gtest pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g -std=c++11")
    SET(GTEST_LIBS gtest pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0 /EHsc /Zi")
    SET(GTEST_LIBS gtest)
endif()

add_definitions(-DTRACE_NODE)

SET(REPL_SRC main.cpp ConsoleWriter.cpp REPL.cpp)

ADD_EXECUTABLE(repl main.cpp ${REPL_SRC})
target_link_libraries(repl swallow)


