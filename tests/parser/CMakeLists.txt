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
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")

add_definitions(-DTRACE_NODE)

#enable_testing()
ADD_EXECUTABLE(TestParser
		TestParser.cpp 
		TestOperatorExpression.cpp
		TestLiteralExpression.cpp
		TestPostfixExpression.cpp
		TestType.cpp
		TestGeneric.cpp
		TestDeclaration.cpp
		TestFunc.cpp
		TestStatement.cpp
		TestEnum.cpp
		TestClass.cpp
		TestClosure.cpp
        TestExtension.cpp
        TestProtocol.cpp
        ../utils.cpp
		)
add_dependencies(TestParser swallow) 
target_link_libraries(TestParser swallow ${GTEST_LIBS})
