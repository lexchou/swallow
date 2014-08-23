PROJECT(swift)
cmake_minimum_required(VERSION 2.6)

FIND_PATH(GTEST_DIR libgtest.a
    ../../gtest-1.7.0
)
SET(GTEST_LIBS gtest gtest_main pthread)
LINK_DIRECTORIES(${GTEST_DIR})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../../swallow
    ../../gtest-1.7.0/include/
)


SET( CMAKE_BUILD_TYPE Debug )
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
cmake_policy(SET CMP0015 OLD)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++0x -Qunused-arguments")

add_definitions(-DTRACE_NODE)


ADD_EXECUTABLE(TestSemantics
    TestSemantics.cpp
    TestConstantFolding.cpp
    TestTypeInference.cpp
    TestSymbolResolve.cpp
    TestFunctionOverloads.cpp
    TestType.cpp
    ../utils.cpp
    )
target_link_libraries(TestSemantics swallow ${GTEST_LIBS})


#ADD_TEST(NAME TestTokenizer COMMAND TestTokenizer)
#ADD_TEST(NAME TestSemantics COMMAND TestSemantics)
#ADD_TEST(NAME TestParser COMMAND TestParser)

