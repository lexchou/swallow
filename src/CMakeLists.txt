PROJECT(swift)
cmake_minimum_required(VERSION 2.6)

FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/TestCase.h
    /usr/local/include
    /usr/include
)
FIND_PATH(CPPUNIT_LIBRARIES libcppunit.a
    /usr/local/lib
    /usr/lib
    /usr/lib/x86_64-linux-gnu/
)
LINK_DIRECTORIES(${CPPUNIT_LIBRARIES})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
    ${CPPUNIT_INCLUDE_DIR}
)


SET( CMAKE_BUILD_TYPE Debug )
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
cmake_policy(SET CMP0015 OLD)



SET(SWIFT_TOKENIZER_SRC tokenizer/tokenizer.cpp)
SET(SWIFT_PARSER_SRC ${SWIFT_TOKENIZER_SRC}
    parser/parser.cpp
    ast/nodes.cpp
    ast/expression-node.cpp
    ast/comment-node.cpp
    ast/node-factory.cpp
    ast/binary-operator.cpp
    ast/unary-operator.cpp
	ast/member-access.cpp
	ast/subscript.cpp
	ast/type-check.cpp
	ast/type-cast.cpp
	ast/assignment.cpp
	ast/conditional-operator.cpp
	ast/type-node.cpp
	ast/operator-node.cpp
	ast/parenthesized-expression.cpp
	ast/function-call.cpp
	ast/initializer.cpp
	ast/self-expression.cpp
	ast/dynamic-type.cpp
	ast/forced-value.cpp
	ast/optional-chaining.cpp
	ast/integer-literal.cpp
	ast/float-literal.cpp
	ast/string-literal.cpp
	ast/compile-constant.cpp



    ast/literal-nodes.cpp
    ast/dictionary-literal.cpp
    ast/array-literal.cpp

    ast/identifier.cpp
    parser/symbol-registry.cpp
    )
SET(SWIFT_SRC main.cpp tokenizer/tokenizer.cpp)

enable_testing()
ADD_EXECUTABLE(TestParser tests/test-parser.cpp ${SWIFT_TOKENIZER_SRC} ${SWIFT_PARSER_SRC})
target_link_libraries(TestParser cppunit)

ADD_EXECUTABLE(TestTokenizer tests/test-tokenizer.cpp ${SWIFT_TOKENIZER_SRC})
target_link_libraries(TestTokenizer cppunit)
ADD_TEST(NAME TestTokenizer COMMAND TestTokenizer)
ADD_TEST(NAME TestParser COMMAND TestParser)

