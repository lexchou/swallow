PROJECT(swallow)
cmake_minimum_required(VERSION 2.6)
SUBDIRS(tests)
set(CMAKE_BUILD_TYPE Release)

#FIND_PATH(GTEST_DIR libgtest.a
#    ../gtest-1.7.0
#    /usr/lib
#    /usr/lib/x86_64-linux-gnu/
#)
SET(GTEST_LIBS gtest gtest_main pthread)
#LINK_DIRECTORIES(${GTEST_DIR})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/includes
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
    src/common/CompilerResults.cpp
    src/common/Errors.cpp
    src/common/SwallowUtils.cpp

    src/tokenizer/Tokenizer.cpp

    src/parser/Parser.cpp
    src/parser/Parser_Expression.cpp
    src/parser/Parser_Statement.cpp
    src/parser/Parser_Declaration.cpp
    src/parser/Parser_Pattern.cpp
    src/parser/Parser_Type.cpp
    src/parser/Parser_Attribute.cpp

    src/semantics/SymbolRegistry.cpp
    src/semantics/SymbolScope.cpp
    src/semantics/Symbol.cpp
    src/semantics/ScopeOwner.cpp
    src/semantics/ScopedNodes.cpp
    src/semantics/ScopedNodeFactory.cpp
    src/semantics/Type.cpp
    src/semantics/FunctionSymbol.cpp
    src/semantics/FunctionOverloadedSymbol.cpp
    src/semantics/ScopeGuard.cpp
    src/semantics/GlobalScope.cpp
    src/semantics/GenericDefinition.cpp
    src/semantics/GenericArgument.cpp
    src/semantics/TypeSpecialization.cpp
    src/semantics/TypeBuilder.cpp
    src/semantics/CollectionTypeAnalyzer.cpp
    src/semantics/SemanticAnalyzer.cpp
    src/semantics/SemanticAnalyzer_TypeCreation.cpp
    src/semantics/SemanticAnalyzer_Function.cpp
    src/semantics/SemanticAnalyzer_Variables.cpp
    src/semantics/SemanticAnalyzer_Expression.cpp
    src/semantics/SemanticAnalyzer_Condition.cpp
    src/semantics/SemanticAnalyzer_Loop.cpp
    src/semantics/SemanticAnalyzer_Modifiers.cpp
    src/semantics/SemanticAnalyzer_Literal.cpp
    src/semantics/SemanticAnalyzer_FunctionCall.cpp
    src/semantics/SemanticAnalyzer_Operators.cpp
    src/semantics/OperatorResolver.cpp
    src/semantics/CompilerResultEmitter.cpp
    src/semantics/SemanticPass.cpp

    src/ast/Node.cpp
    src/ast/Program.cpp
    src/ast/NodeVisitor.cpp
    src/ast/Expression.cpp
    src/ast/Pattern.cpp
    src/ast/Comment.cpp
    src/ast/NodeFactory.cpp
    src/ast/BinaryOperator.cpp
    src/ast/TypedPattern.cpp
    src/ast/UnaryOperator.cpp
    src/ast/InOutParameter.cpp
    src/ast/MemberAccess.cpp
    src/ast/SubscriptAccess.cpp
    src/ast/TypeCheck.cpp
    src/ast/TypeCast.cpp
    src/ast/Assignment.cpp
    src/ast/ConditionalOperator.cpp
    src/ast/TypeNode.cpp
    src/ast/Operator.cpp
    src/ast/ParenthesizedExpression.cpp
    src/ast/FunctionCall.cpp
    src/ast/InitializerReference.cpp
    src/ast/SelfExpression.cpp
    src/ast/ForcedValue.cpp
    src/ast/OptionalChaining.cpp
    src/ast/IntegerLiteral.cpp
    src/ast/FloatLiteral.cpp
    src/ast/StringInterpolation.cpp
    src/ast/StringLiteral.cpp
    src/ast/NilLiteral.cpp
    src/ast/BooleanLiteral.cpp
    src/ast/CompileConstant.cpp

    src/ast/BreakStatement.cpp
    src/ast/ContinueStatement.cpp
    src/ast/DoLoop.cpp
    src/ast/FallthroughStatement.cpp
    src/ast/ForLoop.cpp
    src/ast/ForInLoop.cpp
    src/ast/IfStatement.cpp
    src/ast/LabeledStatement.cpp
    src/ast/ReturnStatement.cpp
    src/ast/Statement.cpp
    src/ast/SwitchCase.cpp
    src/ast/CaseStatement.cpp
    src/ast/WhileLoop.cpp
    src/ast/CodeBlock.cpp
    src/ast/ValueBindingPattern.cpp
    src/ast/Tuple.cpp
    src/ast/Closure.cpp
    src/ast/EnumCasePattern.cpp
    src/ast/TypeNode.cpp
    src/ast/TypeIdentifier.cpp
    src/ast/ImplicitlyUnwrappedOptional.cpp
    src/ast/ProtocolComposition.cpp
    src/ast/FunctionType.cpp
    src/ast/ArrayType.cpp
    src/ast/OptionalType.cpp
    src/ast/TupleType.cpp
    src/ast/DictionaryType.cpp
    src/ast/DynamicType.cpp
    src/ast/DictionaryLiteral.cpp
    src/ast/ArrayLiteral.cpp
    src/ast/Attribute.cpp
    src/ast/ClassDef.cpp
    src/ast/Declaration.cpp
    src/ast/DeinitializerDef.cpp
    src/ast/InitializerDef.cpp
    src/ast/EnumDef.cpp
    src/ast/ExtensionDef.cpp
    src/ast/FunctionDef.cpp
    src/ast/Import.cpp
    src/ast/OperatorDef.cpp
    src/ast/ProtocolDef.cpp
    src/ast/StructDef.cpp
    src/ast/SubscriptDef.cpp
    src/ast/TypeAlias.cpp
    src/ast/ValueBinding.cpp
    src/ast/ValueBindings.cpp
    src/ast/ComputedProperty.cpp
    src/ast/Identifier.cpp
    src/ast/GenericArgumentDef.cpp
    src/ast/Parameters.cpp
    src/ast/Parameter.cpp
    src/ast/TypeDeclaration.cpp
    src/ast/GenericParametersDef.cpp
    src/ast/GenericConstraintDef.cpp
    src/ast/utils/ASTHierachyDumper.cpp
    src/ast/utils/NodeSerializer.cpp
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

