PROJECT(tests)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 OLD)
LINK_DIRECTORIES(../../bin ../../gtest-1.7.0)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../swallow/includes
    ../../gtest-1.7.0/include/
)
SET( CMAKE_BUILD_TYPE Debug )
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../bin)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g -std=c++11")
    SET(GTEST_LIBS gtest pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g -std=gnu++11")
    SET(GTEST_LIBS gtest pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
	cmake_policy(SET CMP0054 OLD)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0 /EHsc /Zi /DEBUG")
    SET(GTEST_LIBS gtest)
endif()

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
    semantics/TestDeclarationOrder.cpp
    semantics/TestSubscripts.cpp
    semantics/TestInheritance.cpp
    semantics/TestInitialization.cpp
    semantics/TestBasic.cpp
    semantics/TestDeinit.cpp
    semantics/TestAccessControl.cpp
    )

SET(CODEGEN_SRC
    codegen/TestNameMangling.cpp
    )
ADD_EXECUTABLE(TestCodeGen
    utils.cpp
    tests.cpp
    ${CODEGEN_SRC}
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
target_link_libraries(TestCodeGen swallow ${GTEST_LIBS})


enable_testing()
add_test(NAME test-tokenizer COMMAND TestTokenizer)
add_test(NAME test-parser COMMAND TestParser)
add_test(NAME test-semantics COMMAND TestSemantics)
add_test(NAME test-codegen COMMAND TestCodeGen)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_custom_command(TARGET TestSemantics POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_BINARY_DIR}/../../bin/Debug/swallow.dll"
            $<TARGET_FILE_DIR:TestSemantics>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_BINARY_DIR}/../../bin/Debug/swallow.pdb"
            $<TARGET_FILE_DIR:TestSemantics>
            )

endif()
