#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"

using namespace Swift;

class TestDeclaration : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestDeclaration);
    CPPUNIT_TEST(testImport);
    CPPUNIT_TEST(testImportKind_Class);
    CPPUNIT_TEST(testImportKind_TypeAlias);
    CPPUNIT_TEST(testImportKind_Struct);
    CPPUNIT_TEST(testImportKind_Enum);
    CPPUNIT_TEST(testImportKind_Protocol);
    CPPUNIT_TEST(testImportKind_Var);
    CPPUNIT_TEST(testImportKind_Func);
    CPPUNIT_TEST(testImportSubModule);
    CPPUNIT_TEST(testLet);
    CPPUNIT_TEST(testLet_Multiple);
    CPPUNIT_TEST(testLet_Tuple);
    
    CPPUNIT_TEST(testFunc);
    CPPUNIT_TEST(testFunc_ReturnFunc);
    CPPUNIT_TEST(testFunc_MultipleParameters);
    CPPUNIT_TEST(testFunc_NoParameters);
    CPPUNIT_TEST(testFunc_NoReturn);
    CPPUNIT_TEST(testFunc_ReturnTuple);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testImport()
    {
        Node* root = parse(L"import Foundation");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::_, import->getKind());
        DESTROY(root);
        
    }
    void testImportSubModule()
    {
        Node* root = parse(L"import Foundation.SubModule");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.SubModule", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::_, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_Class()
    {
        Node* root = parse(L"import class Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Class, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_TypeAlias()
    {
        Node* root = parse(L"import typealias Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Typealias, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_Struct()
    {
        Node* root = parse(L"import struct Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Struct, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_Enum()
    {
        Node* root = parse(L"import enum Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Enum, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_Protocol()
    {
        Node* root = parse(L"import protocol Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Protocol, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_Var()
    {
        Node* root = parse(L"import var Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Var, import->getKind());
        DESTROY(root);
    }
    
    void testImportKind_Func()
    {
        Node* root = parse(L"import func Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Func, import->getKind());
        DESTROY(root);
    }
    
    void testLet()
    {
        Node* root = parse(L"let a : Int[] = [1, 2, 3]");
        Constant* c = NULL;
        Identifier* id = NULL;
        ArrayLiteral* value = NULL;
        ArrayType* type = NULL;
        TypeIdentifier* Int = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numPairs());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(0).first));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<ArrayType*>(id->getType()));
        CPPUNIT_ASSERT(Int = dynamic_cast<TypeIdentifier*>(type->getInnerType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
        CPPUNIT_ASSERT(value = dynamic_cast<ArrayLiteral*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(3, value->numChildren());
        ASSERT_EQUALS(L"1", dynamic_cast<IntegerLiteral*>(value->get(0))->toString().c_str());
        ASSERT_EQUALS(L"2", dynamic_cast<IntegerLiteral*>(value->get(1))->toString().c_str());
        ASSERT_EQUALS(L"3", dynamic_cast<IntegerLiteral*>(value->get(2))->toString().c_str());

        DESTROY(root);
    }
    
    void testLet_Multiple()
    {
        Node* root = parse(L"let a=[k1 : 1, k2 : 2], b : Int = 2");
        Constant* c = NULL;
        Identifier* id = NULL;
        TypeIdentifier* Int = NULL;
        DictionaryLiteral* dict = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT_EQUAL(2, c->numPairs());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(0).first));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(dict = dynamic_cast<DictionaryLiteral*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(4, dict->numChildren());
        
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(1).first));
        ASSERT_EQUALS(L"b", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(Int = dynamic_cast<TypeIdentifier*>(id->getType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
        DESTROY(root);
    }
    
    void testLet_Tuple()
    {
        Node* root = parse(L"let (a, b) : Int = (1, 2)");
        Constant* c = NULL;
        Tuple* tuple = NULL;
        TypeIdentifier* type = NULL;
        ParenthesizedExpression* p = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(c->getPair(0).first));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(tuple->getType()));
        CPPUNIT_ASSERT(p = dynamic_cast<ParenthesizedExpression*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(2, p->numChildren());
        
        DESTROY(root);
    }
    
    void testFunc()
    {
        Node* root = parse(L"func stepForward(input: Int) -> Int {"
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
        
        
        DESTROY(root);
    }
    void testFunc_ReturnFunc()
    {
        Node* root = parse(L"func chooseStepFunction(backwards: Bool) -> (Int) -> Int {"
                           L"return backwards ? stepBackward : stepForward"
                           L"}");
        
        DESTROY(root);
    }
    void testFunc_MultipleParameters()
    {
        Node* root = parse(L"func halfOpenRangeLength(start: Int, end: Int) -> Int {"
                          L"return end - start"
                          L"}");
        DESTROY(root);
    }
    
    void testFunc_NoParameters()
    {
        Node* root = parse(L"func sayHelloWorld() -> String {"
                           L"return \"hello, world\""
                           L"}");
        DESTROY(root);
    }
    void testFunc_NoReturn()
    {
        Node* root = parse(L"func sayGoodbye(personName: String) {"
                           L"println(\"Goodbye, \(personName)!\")"
                           L"}");
        DESTROY(root);
    }
    void testFunc_ReturnTuple()
    {
        Node* root = parse(L"func count(string: String) -> (vowels: Int, consonants: Int, others: Int) {"
                           L"var vowels = 0, consonants = 0, others = 0"
                           L"return (vowels, consonants, others)"
                           L"}");
        DESTROY(root);
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestDeclaration, "alltest");



#endif