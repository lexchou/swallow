PROJECT(swallow)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_BUILD_TYPE Release)

#FIND_PATH(GTEST_DIR libgtest.a
#    ../gtest-1.7.0
#    /usr/lib
#    /usr/lib/x86_64-linux-gnu/
#)
SET(GTEST_LIBS gtest gtest_main pthread)
#LINK_DIRECTORIES(${GTEST_DIR})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
    ../gtest-1.7.0/include/
)


SET( CMAKE_BUILD_TYPE Debug )
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
cmake_policy(SET CMP0015 OLD)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wreturn-type -Wsign-compare -Wunused-variable -Wunused-const-variable -Wparentheses ")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
endif()



SET(SWALLOW_SRC
    common/CompilerResults.cpp
    common/Errors.cpp

    tokenizer/Tokenizer.cpp

    parser/Parser.cpp
    parser/Parser_Expression.cpp
    parser/Parser_Statement.cpp
    parser/Parser_Declaration.cpp
    parser/Parser_Pattern.cpp
    parser/Parser_Type.cpp
    parser/Parser_Attribute.cpp

    semantics/SymbolRegistry.cpp
    semantics/SymbolScope.cpp
    semantics/Symbol.cpp
    semantics/ScopeOwner.cpp
    semantics/ScopedNodes.cpp
    semantics/ScopedNodeFactory.cpp
    semantics/Type.cpp
    semantics/FunctionSymbol.cpp
    semantics/FunctionOverloadedSymbol.cpp
    semantics/ScopeGuard.cpp
    semantics/GlobalScope.cpp
    semantics/GenericDefinition.cpp
    semantics/GenericArgument.cpp
    semantics/TypeSpecialization.cpp
    semantics/TypeBuilder.cpp
    semantics/FunctionIterator.cpp
    semantics/SemanticAnalyzer.cpp
    semantics/SemanticAnalyzer_TypeCreation.cpp
    semantics/SemanticAnalyzer_Function.cpp
    semantics/SemanticAnalyzer_Variables.cpp
    semantics/SemanticAnalyzer_Expression.cpp
    semantics/SemanticAnalyzer_Condition.cpp

    ast/Node.cpp
    ast/Program.cpp
    ast/NodeVisitor.cpp
    ast/NodeSerializer.cpp
    ast/NodeVisitorDecorator.cpp
    ast/Expression.cpp
    ast/Pattern.cpp
    ast/Comment.cpp
    ast/NodeFactory.cpp
    ast/BinaryOperator.cpp
    ast/TypedPattern.cpp
    ast/UnaryOperator.cpp
    ast/InOutParameter.cpp
    ast/MemberAccess.cpp
    ast/SubscriptAccess.cpp
    ast/TypeCheck.cpp
    ast/TypeCast.cpp
    ast/Assignment.cpp
    ast/ConditionalOperator.cpp
    ast/TypeNode.cpp
    ast/Operator.cpp
    ast/ParenthesizedExpression.cpp
    ast/FunctionCall.cpp
    ast/InitializerReference.cpp
    ast/SelfExpression.cpp
    ast/DynamicType.cpp
    ast/ForcedValue.cpp
    ast/OptionalChaining.cpp
    ast/IntegerLiteral.cpp
    ast/FloatLiteral.cpp
    ast/StringInterpolation.cpp
    ast/StringLiteral.cpp
    ast/NilLiteral.cpp
    ast/BooleanLiteral.cpp
    ast/CompileConstant.cpp

    ast/BreakStatement.cpp
    ast/ContinueStatement.cpp
    ast/DoLoop.cpp
    ast/FallthroughStatement.cpp
    ast/ForLoop.cpp
    ast/ForInLoop.cpp
    ast/IfStatement.cpp
    ast/LabeledStatement.cpp
    ast/ReturnStatement.cpp
    ast/Statement.cpp
    ast/SwitchCase.cpp
    ast/CaseStatement.cpp
    ast/WhileLoop.cpp
    ast/CodeBlock.cpp
    ast/ValueBindingPattern.cpp
    ast/Tuple.cpp
    ast/Closure.cpp
    ast/EnumCasePattern.cpp

    ast/TypeNode.cpp
    ast/TypeIdentifier.cpp
    ast/FunctionType.cpp
    ast/ArrayType.cpp
    ast/OptionalType.cpp
    ast/ImplicitlyUnwrappedOptional.cpp
    ast/TupleType.cpp
    ast/ProtocolComposition.cpp

    ast/DictionaryLiteral.cpp
    ast/ArrayLiteral.cpp
    ast/Attribute.cpp

    ast/ClassDef.cpp
    ast/Declaration.cpp
    ast/DeinitializerDef.cpp
    ast/InitializerDef.cpp
    ast/EnumDef.cpp
    ast/ExtensionDef.cpp
    ast/FunctionDef.cpp
    ast/Import.cpp
    ast/OperatorDef.cpp
    ast/ProtocolDef.cpp
    ast/StructDef.cpp
    ast/SubscriptDef.cpp
    ast/TypeAlias.cpp
    ast/ValueBinding.cpp
    ast/ValueBindings.cpp
    ast/ComputedProperty.cpp
    ast/Identifier.cpp
    ast/GenericArgumentDef.cpp
    ast/Parameters.cpp
    ast/Parameter.cpp
    ast/TypeDeclaration.cpp
    ast/GenericParametersDef.cpp
    ast/GenericConstraintDef.cpp
    )

add_definitions(-DTRACE_NODE)

add_library(swallow SHARED ${SWALLOW_SRC})


#enable_testing()
#ADD_EXECUTABLE(TestParser
#   	tests/parser/TestParser.cpp 
#   	tests/parser/TestOperatorExpression.cpp
#   	tests/parser/TestLiteralExpression.cpp
#   	tests/parser/TestPostfixExpression.cpp
#   	tests/parser/TestType.cpp
#   	tests/parser/TestGeneric.cpp
#   	tests/parser/TestDeclaration.cpp
#   	tests/parser/TestFunc.cpp
#   	tests/parser/TestStatement.cpp
#   	tests/parser/TestEnum.cpp
#   	tests/parser/TestClass.cpp
#   	tests/parser/TestClosure.cpp
#       tests/parser/TestExtension.cpp
#       tests/parser/TestProtocol.cpp
#       tests/utils.cpp
#   	${SWIFT_TOKENIZER_SRC} ${SWIFT_PARSER_SRC})
#target_link_libraries(TestParser ${GTEST_LIBS})

#ADD_EXECUTABLE(TestTokenizer tests/tokenizer/TestTokenizer.cpp ${SWIFT_TOKENIZER_SRC})
#target_link_libraries(TestTokenizer ${GTEST_LIBS})

#ADD_EXECUTABLE(TestSemantics
    #    tests/semantics/TestSemantics.cpp
    #    tests/semantics/TestConstantFolding.cpp
    #    tests/semantics/TestTypeInference.cpp
    #    tests/semantics/TestSymbolResolve.cpp
    #    tests/semantics/TestFunctionOverloads.cpp
    #    tests/semantics/TestType.cpp
    #    tests/utils.cpp
    #    ${SWIFT_TOKENIZER_SRC} ${SWIFT_PARSER_SRC})
#target_link_libraries(TestSemantics ${GTEST_LIBS})


#ADD_TEST(NAME TestTokenizer COMMAND TestTokenizer)
#ADD_TEST(NAME TestSemantics COMMAND TestSemantics)
#ADD_TEST(NAME TestParser COMMAND TestParser)

