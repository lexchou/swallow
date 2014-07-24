#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

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
    CPPUNIT_TEST(testOperator);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testImport()
    {
        PARSE_STATEMENT(L"import Foundation");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::_, import->getKind());
        
    }
    void testImportSubModule()
    {
        PARSE_STATEMENT(L"import Foundation.SubModule");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.SubModule", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::_, import->getKind());
    }
    
    void testImportKind_Class()
    {
        PARSE_STATEMENT(L"import class Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Class, import->getKind());
    }
    
    void testImportKind_TypeAlias()
    {
        PARSE_STATEMENT(L"import typealias Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Typealias, import->getKind());
    }
    
    void testImportKind_Struct()
    {
        PARSE_STATEMENT(L"import struct Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Struct, import->getKind());
    }
    
    void testImportKind_Enum()
    {
        PARSE_STATEMENT(L"import enum Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Enum, import->getKind());
    }
    
    void testImportKind_Protocol()
    {
        PARSE_STATEMENT(L"import protocol Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Protocol, import->getKind());
    }
    
    void testImportKind_Var()
    {
        PARSE_STATEMENT(L"import var Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Var, import->getKind());
    }
    
    void testImportKind_Func()
    {
        PARSE_STATEMENT(L"import func Foundation.NSFileManager");
        Import* import = NULL;
        CPPUNIT_ASSERT(import = dynamic_cast<Import*>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Func, import->getKind());
    }
    
    void testLet()
    {
        PARSE_STATEMENT(L"let a : Int[] = [1, 2, 3]");
        Constant* c = NULL;
        Identifier* id = NULL;
        ArrayLiteral* value = NULL;
        ArrayType* type = NULL;
        TypeIdentifier* Int = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numPairs());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(0).first));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<ArrayType*>(id->getDeclaredType()));
        CPPUNIT_ASSERT(Int = dynamic_cast<TypeIdentifier*>(type->getInnerType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
        CPPUNIT_ASSERT(value = dynamic_cast<ArrayLiteral*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(3, value->numElements());
        ASSERT_EQUALS(L"1", dynamic_cast<IntegerLiteral*>(value->getElement(0))->toString().c_str());
        ASSERT_EQUALS(L"2", dynamic_cast<IntegerLiteral*>(value->getElement(1))->toString().c_str());
        ASSERT_EQUALS(L"3", dynamic_cast<IntegerLiteral*>(value->getElement(2))->toString().c_str());

    }
    
    void testLet_Multiple()
    {
        PARSE_STATEMENT(L"let a=[k1 : 1, k2 : 2], b : Int = 2");
        Constant* c = NULL;
        Identifier* id = NULL;
        TypeIdentifier* Int = NULL;
        DictionaryLiteral* dict = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT_EQUAL(2, c->numPairs());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(0).first));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(dict = dynamic_cast<DictionaryLiteral*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(2, dict->numElements());
        
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(1).first));
        ASSERT_EQUALS(L"b", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(Int = dynamic_cast<TypeIdentifier*>(id->getDeclaredType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
    }
    
    void testLet_Tuple()
    {
        PARSE_STATEMENT(L"let (a, b) : Int = (1, 2)");
        Constant* c = NULL;
        Tuple* tuple = NULL;
        TypeIdentifier* type = NULL;
        ParenthesizedExpression* p = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(c->getPair(0).first));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(tuple->getType()));
        CPPUNIT_ASSERT(p = dynamic_cast<ParenthesizedExpression*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(2, p->numExpressions());
        
    }
    
    void testVar()
    {
        PARSE_STATEMENT(L"var currentLoginAttempt = 0");
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
        
        
    }
    void testVar_Multiple()
    {
        PARSE_STATEMENT(L"var x = 0.0, y = 0.0, z = 0.0");
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
        PARSE_STATEMENT(L"var welcomeMessage: String");
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        TypeIdentifier* t = NULL;
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(root));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"welcomeMessage", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(var->getDeclaredType()));
        ASSERT_EQUALS(L"String", t->getName().c_str());
        
        
    }

    void testOperator()
    {
        PARSE_STATEMENT(L"operator infix +- { associativity left precedence 140 }");
        OperatorDef* o = NULL;
        CPPUNIT_ASSERT(o = dynamic_cast<OperatorDef*>(root));
        ASSERT_EQUALS(L"+-", o->getName());
        CPPUNIT_ASSERT_EQUAL(Associativity::Left, o->getAssociativity());
        CPPUNIT_ASSERT_EQUAL(140, o->getPrecedence());


    }

    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestDeclaration, "alltest");



#endif
