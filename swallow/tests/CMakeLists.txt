PROJECT(tests)
cmake_minimum_required(VERSION 2.6)
SET(GTEST_LIBS gtest gtest_main pthread)
cmake_policy(SET CMP0015 OLD)
LINK_DIRECTORIES(../../bin ../../gtest-1.7.0)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../swallow/includes
    ../../gtest-1.7.0/include/
)
SET( CMAKE_BUILD_TYPE Debug )
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../bin)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++0x")

add_definitions(-DTRACE_NODE)


SET(TOKENIZER_SRC tokenizer/TestTokenizer.cpp)
SET(PARSER_SRC
	parser/TestOperatorExpression.cpp
	parser/TestLiteralExpression.cpp
	parser/TestPostfixExpression.cpp
	parser/TestType.cpp
	parser/TestGeneric.cpp
	parser/TestDeclaration.cpp
	parser/TestFunc.cpp
	parser/TestStatement.cpp
	parser/TestEnum.cpp
	parser/TestClass.cpp
	parser/TestClosure.cpp
    parser/TestExtension.cpp
    parser/TestProtocol.cpp
		)

SET(SEMANTICS_SRC
    semantics/TestConstantFolding.cpp
    semantics/TestTypeInference.cpp
    semantics/TestSymbolResolve.cpp
    semantics/TestFunctionOverloads.cpp
    semantics/TestType.cpp
    semantics/TestProtocol.cpp
    semantics/TestGeneric.cpp
    semantics/TestEnumeration.cpp
    semantics/TestCondition.cpp
    semantics/TestOptional.cpp
    semantics/TestCollection.cpp
    semantics/TestOperators.cpp
    semantics/TestExtension.cpp
    semantics/TestLoop.cpp
    semantics/TestProperties.cpp
    semantics/TestMethods.cpp
    )

ADD_EXECUTABLE(TestTokenizer
    utils.cpp
    tests.cpp
    ${TOKENIZER_SRC}
    )
ADD_EXECUTABLE(TestParser
    utils.cpp
    tests.cpp
    ${PARSER_SRC}
    )
ADD_EXECUTABLE(TestSemantics
    utils.cpp
    tests.cpp
    ${SEMANTICS_SRC}
    )
target_link_libraries(TestTokenizer swallow ${GTEST_LIBS})
target_link_libraries(TestParser swallow ${GTEST_LIBS})
target_link_libraries(TestSemantics swallow ${GTEST_LIBS})

enable_testing()
add_test(NAME test-tokenizer COMMAND TestTokenizer)
add_test(NAME test-parser COMMAND TestParser)
add_test(NAME test-semantics COMMAND TestSemantics)
