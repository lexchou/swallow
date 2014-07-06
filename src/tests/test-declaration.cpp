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
    CPPUNIT_TEST(testVar);
    CPPUNIT_TEST(testVar_Multiple);
    CPPUNIT_TEST(testVar_Typed);
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
    
    void testVar()
    {
        Node* root = parse(L"var currentLoginAttempt = 0");
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        IntegerLiteral* i = NULL;
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(root));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"currentLoginAttempt", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(var->getInitializer()));
        ASSERT_EQUALS(L"0", i->toString().c_str());
        
        
        DESTROY(root);
    }
    void testVar_Multiple()
    {
        Node* root = parse(L"var x = 0.0, y = 0.0, z = 0.0");
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        FloatLiteral* f = NULL;
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(root));
        CPPUNIT_ASSERT_EQUAL(3, vars->numVariables());
        
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"x", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(f = dynamic_cast<FloatLiteral*>(var->getInitializer()));
        ASSERT_EQUALS(L"0.0", f->toString().c_str());
        
        
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(1)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"y", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(f = dynamic_cast<FloatLiteral*>(var->getInitializer()));
        ASSERT_EQUALS(L"0.0", f->toString().c_str());
        
        
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(2)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"z", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(f = dynamic_cast<FloatLiteral*>(var->getInitializer()));
        ASSERT_EQUALS(L"0.0", f->toString().c_str());
        
        
    }
    
    void testVar_Typed()
    {
        Node* root = parse(L"var welcomeMessage: String");
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        TypeIdentifier* t = NULL;
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(root));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"welcomeMessage", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(var->getType()));
        ASSERT_EQUALS(L"String", t->getName().c_str());
        
        
        DESTROY(root);
    }
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestDeclaration, "alltest");



#endif