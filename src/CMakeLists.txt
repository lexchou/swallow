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
    parser/parser_expression.cpp
    parser/parser_statement.cpp
    parser/parser_declaration.cpp
	parser/parser_pattern.cpp
	parser/parser_type.cpp

    ast/node.cpp
    ast/expression-node.cpp
    ast/comment-node.cpp
    ast/node-factory.cpp
    ast/binary-operator.cpp
    ast/unary-operator.cpp
    ast/in-out-parameter.cpp
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

	ast/break-statement.cpp
	ast/continue-statement.cpp
	ast/do-loop.cpp
	ast/fallthrough-statement.cpp
	ast/for-loop.cpp
	ast/if-statement.cpp
	ast/labeled-statement.cpp
	ast/return-statement.cpp
	ast/statement.cpp
	ast/switch-case.cpp
	ast/case-statement.cpp
	ast/while-loop.cpp
	ast/code-block.cpp
	ast/value-binding.cpp
    ast/tuple.cpp

    ast/type-node.cpp
    ast/type-identifier.cpp
    ast/function-type.cpp
    ast/array-type.cpp
    ast/optional-type.cpp
    ast/implicitly-unwrapped-optional.cpp
    ast/tuple-type.cpp
    ast/protocol-composition.cpp

    ast/literal-nodes.cpp
    ast/dictionary-literal.cpp
    ast/array-literal.cpp

    ast/identifier.cpp
    parser/symbol-registry.cpp
    )
SET(SWIFT_SRC main.cpp tokenizer/tokenizer.cpp)


add_definitions(-DTRACE_NODE)

enable_testing()
ADD_EXECUTABLE(TestParser
		tests/test-parser.cpp 
		tests/test-operator-expression.h
		tests/test-literal-expression.h
		tests/test-postfix-expression.h
		tests/test-type.h
		tests/test-statement.h
		${SWIFT_TOKENIZER_SRC} ${SWIFT_PARSER_SRC})
target_link_libraries(TestParser cppunit)

ADD_EXECUTABLE(TestTokenizer tests/test-tokenizer.cpp ${SWIFT_TOKENIZER_SRC})
target_link_libraries(TestTokenizer cppunit)
ADD_TEST(NAME TestTokenizer COMMAND TestTokenizer)
ADD_TEST(NAME TestParser COMMAND TestParser)

