#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

TEST(TestFunc, testFunc)
{
    PARSE_STATEMENT(L"func stepForward(input: Int) -> Int {"
                       L"return input + 1"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;
    CodeBlockPtr cb;
    ReturnStatementPtr ret;
    BinaryOperatorPtr add;
    IdentifierPtr id;
    IntegerLiteralPtr i;
    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"stepForward", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"input", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", type->getName());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(func->getReturnType()));
    ASSERT_EQ(L"Int", type->getName());

    ASSERT_NOT_NULL(cb = func->getBody());
    ASSERT_EQ(1, cb->numStatements());

    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
    ASSERT_NOT_NULL(add = std::dynamic_pointer_cast<BinaryOperator>(ret->getExpression()));
    ASSERT_EQ(L"+", add->getOperator());

    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(add->getLHS()));
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(add->getRHS()));

    ASSERT_EQ(L"input", id->getIdentifier());
    ASSERT_EQ(L"1", i->valueAsString);


}
TEST(TestFunc, testFunc_ReturnFunc)
{
    PARSE_STATEMENT(L"func chooseStepFunction(backwards: Bool) -> (Int) -> Int {"
                       L"return backwards ? stepBackward : stepForward"
                       L"}");

    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;
    FunctionTypePtr rettype;
    TupleTypePtr tupleType;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"chooseStepFunction", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"backwards", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Bool", type->getName());

    ASSERT_NOT_NULL(rettype = std::dynamic_pointer_cast<FunctionType>(func->getReturnType()));
    ASSERT_NOT_NULL(tupleType = std::dynamic_pointer_cast<TupleType>(rettype->getArgumentsType()));
    ASSERT_FALSE(tupleType->getVariadicParameters());
    ASSERT_EQ(1, tupleType->numElements());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(tupleType->getElementType(0)));
    ASSERT_EQ(L"Int", type->getName());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(rettype->getReturnType()));
    ASSERT_EQ(L"Int", type->getName());

}
TEST(TestFunc, testFunc_MultipleParameters)
{
    PARSE_STATEMENT(L"func halfOpenRangeLength(start: Int, end: Int) -> Int {"
                      L"return end - start"
                      L"}");

    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"halfOpenRangeLength", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(2, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"start", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", type->getName());


    ASSERT_NOT_NULL(param = params->getParameter(1));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"end", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", type->getName());


}

TEST(TestFunc, testFunc_NoParameters)
{
    PARSE_STATEMENT(L"func sayHelloWorld() -> String {"
                       L"return \"hello, world\""
                       L"}");

    FunctionDefPtr func;
    ParametersPtr params;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"sayHelloWorld", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(0, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

}
TEST(TestFunc, testFunc_NoReturn)
{
    PARSE_STATEMENT(L"func sayGoodbye(personName: String) {"
                       L"println(\"Goodbye, \(personName)!\")"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"sayGoodbye", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());
    ASSERT_NULL(func->getReturnType());

}
TEST(TestFunc, testFunc_ReturnTuple)
{
    PARSE_STATEMENT(L"func count(string: String) -> (vowels: Int, consonants: Int, others: Int) {"
                       L"var vowels = 0, consonants = 0, others = 0"
                       L"return (vowels, consonants, others)"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;
    TupleTypePtr rettype;
    TypeIdentifierPtr id;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"count", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());
    ASSERT_NOT_NULL(rettype = std::dynamic_pointer_cast<TupleType>(func->getReturnType()));

    ASSERT_EQ(3, rettype->numElements());
    ASSERT_FALSE(rettype->getElement(0).inout);
    ASSERT_EQ(L"vowels", rettype->getElement(0).name);
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<TypeIdentifier>(rettype->getElementType(0)));
    ASSERT_EQ(L"Int", id->getName());


    ASSERT_FALSE(rettype->getElement(1).inout);
    ASSERT_EQ(L"consonants", rettype->getElement(1).name);
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<TypeIdentifier>(rettype->getElementType(1)));
    ASSERT_EQ(L"Int", id->getName());

    ASSERT_FALSE(rettype->getElement(2).inout);
    ASSERT_EQ(L"others", rettype->getElement(2).name);
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<TypeIdentifier>(rettype->getElementType(2)));
    ASSERT_EQ(L"Int", id->getName());

}
TEST(TestFunc, testFunc_ExtParamNam)
{
    PARSE_STATEMENT(L"func someFunction(externalParameterName localParameterName: Int) {"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"someFunction", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"externalParameterName", param->getExternalName());
    ASSERT_EQ(L"localParameterName", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", type->getName());

}
TEST(TestFunc, testFunc_ShorthandExtParamName)
{
    PARSE_STATEMENT(L"func containsCharacter(#string: String, #characterToFind: Character) -> Bool {"
                       L"return false"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"containsCharacter", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(2, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_TRUE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"string", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"String", type->getName());

    ASSERT_NOT_NULL(param = params->getParameter(1));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_TRUE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_NULL(param->getDefaultValue());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"characterToFind", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Character", type->getName());

}
TEST(TestFunc, testFunc_DefParam)
{
    PARSE_STATEMENT(L"func join(string s1: String, toString s2: String,"
                       L"withJoiner joiner: String = \" \") -> String {"
                       L"return s1 + joiner + s2"
                       L"}");

    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;
    StringLiteralPtr str;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"join", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(3, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(2));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_EQ(L"withJoiner", param->getExternalName());
    ASSERT_EQ(L"joiner", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"String", type->getName());
    ASSERT_NOT_NULL(str = std::dynamic_pointer_cast<StringLiteral>(param->getDefaultValue()));
    ASSERT_EQ(L" ", str->toString());

}
TEST(TestFunc, testFunc_VariadicParams)
{
    PARSE_STATEMENT(L"func arithmeticMean(numbers: Double...) -> Double {"
                       L"}");

    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"arithmeticMean", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_TRUE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"numbers", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Double", type->getName());
    ASSERT_NULL(param->getDefaultValue());

}
TEST(TestFunc, testFunc_VariableParam)
{
    PARSE_STATEMENT(L"func alignRight(var string: String, count: Int, pad: Character) -> String {"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"alignRight", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(3, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::Variable, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"string", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"String", type->getName());
    ASSERT_NULL(param->getDefaultValue());
}
TEST(TestFunc, testFunc_Inout)
{
    PARSE_STATEMENT(L"func swapTwoInts(inout a: Int, inout b: Int) {"
                       L"}");

    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"swapTwoInts", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(2, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_TRUE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"a", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", type->getName());
    ASSERT_NULL(param->getDefaultValue());


    ASSERT_NOT_NULL(param = params->getParameter(1));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_TRUE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"b", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", type->getName());
    ASSERT_NULL(param->getDefaultValue());

}
TEST(TestFunc, testFunc_FuncType)
{
    PARSE_STATEMENT(L"func printMathResult(mathFunction: (Int, Int) -> Int, a: Int, b: Int) {"
                       L"}");

    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    FunctionTypePtr type;
    TupleTypePtr args;
    TypeIdentifierPtr t;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"printMathResult", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(3, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"mathFunction", param->getLocalName());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<FunctionType>(param->getDeclaredType()));
    ASSERT_NOT_NULL(args = std::dynamic_pointer_cast<TupleType>(type->getArgumentsType()));
    ASSERT_EQ(2, args->numElements());

    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(args->getElementType(0)));
    ASSERT_EQ(L"Int", t->getName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(args->getElementType(1)));
    ASSERT_EQ(L"Int", t->getName());


    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(type->getReturnType()));
    ASSERT_EQ(L"Int", t->getName());

}
TEST(TestFunc, testFunc_NestedFunc)
{
    PARSE_STATEMENT(L"func chooseStepFunction(backwards: Bool) -> (Int) -> Int {"
                       L"func stepForward(input: Int) -> Int { return input + 1 }"
                       L"func stepBackward(input: Int) -> Int { return input - 1 }"
                       L"return backwards ? stepBackward : stepForward"
                       L"}");
    FunctionDefPtr func;
    CodeBlockPtr cb;
    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"chooseStepFunction", func->getName());
    ASSERT_EQ(1, func->numParameters());

    ASSERT_NOT_NULL(cb = std::dynamic_pointer_cast<CodeBlock>(func->getBody()));

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(cb->getStatement(0)));
    ASSERT_EQ(L"stepForward", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_EQ(1, func->getParameters(0)->numParameters());

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(cb->getStatement(1)));
    ASSERT_EQ(L"stepBackward", func->getName());
    ASSERT_EQ(1, func->numParameters());
    ASSERT_EQ(1, func->getParameters(0)->numParameters());

}
TEST(TestFunc, testFunc_CurriedFunc)
{
    PARSE_STATEMENT(L"func addTwoNumbers(a: Int)(b: Int) -> Int {"
                       L"return a + b"
                       L"}");
    FunctionDefPtr func;
    ParametersPtr params;
    ParameterPtr param;
    TypeIdentifierPtr t;

    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_EQ(L"addTwoNumbers", func->getName());
    ASSERT_EQ(2, func->numParameters());

    ASSERT_NOT_NULL(params = func->getParameters(0));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"a", param->getLocalName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", t->getName());


    ASSERT_NOT_NULL(params = func->getParameters(1));
    ASSERT_EQ(1, params->numParameters());
    ASSERT_FALSE(params->isVariadicParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_EQ(Parameter::None, param->getAccessibility());
    ASSERT_FALSE(param->isShorthandExternalName());
    ASSERT_FALSE(param->isInout());
    ASSERT_EQ(L"", param->getExternalName());
    ASSERT_EQ(L"b", param->getLocalName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"Int", t->getName());

}


#endif
