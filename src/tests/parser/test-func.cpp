#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

class TestFunc : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestFunc);
    
    CPPUNIT_TEST(testFunc);
    CPPUNIT_TEST(testFunc_ReturnFunc);
    CPPUNIT_TEST(testFunc_MultipleParameters);
    CPPUNIT_TEST(testFunc_NoParameters);
    CPPUNIT_TEST(testFunc_NoReturn);
    CPPUNIT_TEST(testFunc_ReturnTuple);
    CPPUNIT_TEST(testFunc_ExtParamNam);
    CPPUNIT_TEST(testFunc_ShorthandExtParamName);
    CPPUNIT_TEST(testFunc_DefParam);
    CPPUNIT_TEST(testFunc_VariadicParams);
    CPPUNIT_TEST(testFunc_VariableParam);
    CPPUNIT_TEST(testFunc_Inout);
    CPPUNIT_TEST(testFunc_FuncType);
    CPPUNIT_TEST(testFunc_NestedFunc);
    CPPUNIT_TEST(testFunc_CurriedFunc);
    CPPUNIT_TEST_SUITE_END();
public:
    void testFunc()
    {
        PARSE_STATEMENT(L"func stepForward(input: Int) -> Int {"
                           L"return input + 1"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        CodeBlock* cb = NULL;
        ReturnStatement* ret = NULL;
        BinaryOperator* add = NULL;
        Identifier* id = NULL;
        IntegerLiteral* i = NULL;
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"stepForward", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"input", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(func->getReturnType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
        CPPUNIT_ASSERT(cb = func->getBody());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());

        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        CPPUNIT_ASSERT(add = dynamic_cast<BinaryOperator*>(ret->getExpression()));
        ASSERT_EQUALS(L"+", add->getOperator().c_str());
        
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(add->getLHS()));
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(add->getRHS()));
        
        ASSERT_EQUALS(L"input", id->getIdentifier().c_str());
        ASSERT_EQUALS(L"1", i->toString().c_str());
        
        
    }
    void testFunc_ReturnFunc()
    {
        PARSE_STATEMENT(L"func chooseStepFunction(backwards: Bool) -> (Int) -> Int {"
                           L"return backwards ? stepBackward : stepForward"
                           L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        FunctionType* rettype = NULL;
        TupleType* tupleType = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"chooseStepFunction", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"backwards", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Bool", type->getName().c_str());
        
        CPPUNIT_ASSERT(rettype = dynamic_cast<FunctionType*>(func->getReturnType()));
        CPPUNIT_ASSERT(tupleType = dynamic_cast<TupleType*>(rettype->getArgumentsType()));
        CPPUNIT_ASSERT(!tupleType->getVariadicParameters());
        CPPUNIT_ASSERT_EQUAL(1, tupleType->numElements());
        
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(tupleType->getElementType(0)));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(rettype->getReturnType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
    }
    void testFunc_MultipleParameters()
    {
        PARSE_STATEMENT(L"func halfOpenRangeLength(start: Int, end: Int) -> Int {"
                          L"return end - start"
                          L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"halfOpenRangeLength", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(2, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"start", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
        
        CPPUNIT_ASSERT(param = params->getParameter(1));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"end", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
        
    }
    
    void testFunc_NoParameters()
    {
        PARSE_STATEMENT(L"func sayHelloWorld() -> String {"
                           L"return \"hello, world\""
                           L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"sayHelloWorld", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(0, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
    }
    void testFunc_NoReturn()
    {
        PARSE_STATEMENT(L"func sayGoodbye(personName: String) {"
                           L"println(\"Goodbye, \(personName)!\")"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;

        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"sayGoodbye", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        CPPUNIT_ASSERT(NULL == func->getReturnType());
        
    }
    void testFunc_ReturnTuple()
    {
        PARSE_STATEMENT(L"func count(string: String) -> (vowels: Int, consonants: Int, others: Int) {"
                           L"var vowels = 0, consonants = 0, others = 0"
                           L"return (vowels, consonants, others)"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        TupleType* rettype = NULL;
        TypeIdentifier* id = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"count", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        CPPUNIT_ASSERT(rettype = dynamic_cast<TupleType*>(func->getReturnType()));
        
        CPPUNIT_ASSERT_EQUAL(3, rettype->numElements());
        CPPUNIT_ASSERT(!rettype->getElement(0).inout);
        ASSERT_EQUALS(L"vowels", rettype->getElement(0).name.c_str());
        CPPUNIT_ASSERT(id = dynamic_cast<TypeIdentifier*>(rettype->getElementType(0)));
        ASSERT_EQUALS(L"Int", id->getName().c_str());
        
        
        CPPUNIT_ASSERT(!rettype->getElement(1).inout);
        ASSERT_EQUALS(L"consonants", rettype->getElement(1).name.c_str());
        CPPUNIT_ASSERT(id = dynamic_cast<TypeIdentifier*>(rettype->getElementType(1)));
        ASSERT_EQUALS(L"Int", id->getName().c_str());
        
        CPPUNIT_ASSERT(!rettype->getElement(2).inout);
        ASSERT_EQUALS(L"others", rettype->getElement(2).name.c_str());
        CPPUNIT_ASSERT(id = dynamic_cast<TypeIdentifier*>(rettype->getElementType(2)));
        ASSERT_EQUALS(L"Int", id->getName().c_str());
        
    }
    void testFunc_ExtParamNam()
    {
        PARSE_STATEMENT(L"func someFunction(externalParameterName localParameterName: Int) {"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"someFunction", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"externalParameterName", param->getExternalName().c_str());
        ASSERT_EQUALS(L"localParameterName", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        
    }
    void testFunc_ShorthandExtParamName()
    {
        PARSE_STATEMENT(L"func containsCharacter(#string: String, #characterToFind: Character) -> Bool {"
                           L"return false"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"containsCharacter", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(2, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"string", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName().c_str());
        
        CPPUNIT_ASSERT(param = params->getParameter(1));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"characterToFind", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Character", type->getName().c_str());
        
    }
    void testFunc_DefParam()
    {
        PARSE_STATEMENT(L"func join(string s1: String, toString s2: String,"
                           L"withJoiner joiner: String = \" \") -> String {"
                           L"return s1 + joiner + s2"
                           L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        StringLiteral* str = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"join", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(3, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(2));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        ASSERT_EQUALS(L"withJoiner", param->getExternalName().c_str());
        ASSERT_EQUALS(L"joiner", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName().c_str());
        CPPUNIT_ASSERT(str = dynamic_cast<StringLiteral*>(param->getDefaultValue()));
        ASSERT_EQUALS(L" ", str->toString().c_str());
        
    }
    void testFunc_VariadicParams()
    {
        PARSE_STATEMENT(L"func arithmeticMean(numbers: Double...) -> Double {"
                           L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"arithmeticMean", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"numbers", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Double", type->getName().c_str());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        
    }
    void testFunc_VariableParam()
    {
        PARSE_STATEMENT(L"func alignRight(var string: String, count: Int, pad: Character) -> String {"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"alignRight", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(3, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::Variable, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"string", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName().c_str());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
    }
    void testFunc_Inout()
    {
        PARSE_STATEMENT(L"func swapTwoInts(inout a: Int, inout b: Int) {"
                           L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"swapTwoInts", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(2, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"a", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        
        
        CPPUNIT_ASSERT(param = params->getParameter(1));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"b", param->getLocalName().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", type->getName().c_str());
        CPPUNIT_ASSERT(NULL == param->getDefaultValue());
        
    }
    void testFunc_FuncType()
    {
        PARSE_STATEMENT(L"func printMathResult(mathFunction: (Int, Int) -> Int, a: Int, b: Int) {"
                           L"}");
        
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        FunctionType* type = NULL;
        TupleType* args = NULL;
        TypeIdentifier* t = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"printMathResult", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(3, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"mathFunction", param->getLocalName().c_str());
        
        CPPUNIT_ASSERT(type = dynamic_cast<FunctionType*>(param->getType()));
        CPPUNIT_ASSERT(args = dynamic_cast<TupleType*>(type->getArgumentsType()));
        CPPUNIT_ASSERT_EQUAL(2, args->numElements());
        
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(args->getElementType(0)));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(args->getElementType(1)));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        
        
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(type->getReturnType()));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        
    }
    void testFunc_NestedFunc()
    {
        PARSE_STATEMENT(L"func chooseStepFunction(backwards: Bool) -> (Int) -> Int {"
                           L"func stepForward(input: Int) -> Int { return input + 1 }"
                           L"func stepBackward(input: Int) -> Int { return input - 1 }"
                           L"return backwards ? stepBackward : stepForward"
                           L"}");
        FunctionDef* func = NULL;
        CodeBlock* cb = NULL;
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"chooseStepFunction", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        
        CPPUNIT_ASSERT(cb = dynamic_cast<CodeBlock*>(func->getBody()));
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(cb->getStatement(0)));
        ASSERT_EQUALS(L"stepForward", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT_EQUAL(1, func->getParameters(0)->numParameters());

        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(cb->getStatement(1)));
        ASSERT_EQUALS(L"stepBackward", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(1, func->numParameters());
        CPPUNIT_ASSERT_EQUAL(1, func->getParameters(0)->numParameters());
        
    }
    void testFunc_CurriedFunc()
    {
        PARSE_STATEMENT(L"func addTwoNumbers(a: Int)(b: Int) -> Int {"
                           L"return a + b"
                           L"}");
        FunctionDef* func = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        TypeIdentifier* t = NULL;
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(root));
        ASSERT_EQUALS(L"addTwoNumbers", func->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(2, func->numParameters());
        
        CPPUNIT_ASSERT(params = func->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"a", param->getLocalName().c_str());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        
        
        CPPUNIT_ASSERT(params = func->getParameters(1));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
        CPPUNIT_ASSERT(!params->isVariadicParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT_EQUAL(Parameter::None, param->getAccessibility());
        CPPUNIT_ASSERT(!param->isShorthandExternalName());
        CPPUNIT_ASSERT(!param->isInout());
        ASSERT_EQUALS(L"", param->getExternalName().c_str());
        ASSERT_EQUALS(L"b", param->getLocalName().c_str());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestFunc, "alltest");



#endif
