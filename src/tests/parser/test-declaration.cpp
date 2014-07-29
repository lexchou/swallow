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
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::_, import->getKind());
        
    }
    void testImportSubModule()
    {
        PARSE_STATEMENT(L"import Foundation.SubModule");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.SubModule", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::_, import->getKind());
    }
    
    void testImportKind_Class()
    {
        PARSE_STATEMENT(L"import class Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Class, import->getKind());
    }
    
    void testImportKind_TypeAlias()
    {
        PARSE_STATEMENT(L"import typealias Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Typealias, import->getKind());
    }
    
    void testImportKind_Struct()
    {
        PARSE_STATEMENT(L"import struct Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Struct, import->getKind());
    }
    
    void testImportKind_Enum()
    {
        PARSE_STATEMENT(L"import enum Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Enum, import->getKind());
    }
    
    void testImportKind_Protocol()
    {
        PARSE_STATEMENT(L"import protocol Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Protocol, import->getKind());
    }
    
    void testImportKind_Var()
    {
        PARSE_STATEMENT(L"import var Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Var, import->getKind());
    }
    
    void testImportKind_Func()
    {
        PARSE_STATEMENT(L"import func Foundation.NSFileManager");
        ImportPtr import;
        CPPUNIT_ASSERT(import = std::dynamic_pointer_cast<Import>(root));
        ASSERT_EQUALS(L"Foundation.NSFileManager", import->getPath().c_str());
        CPPUNIT_ASSERT_EQUAL(Import::Func, import->getKind());
    }
    
    void testLet()
    {
        PARSE_STATEMENT(L"let a : Int[] = [1, 2, 3]");
        ConstantsPtr c;
        IdentifierPtr id;
        ArrayLiteralPtr value;
        ArrayTypePtr type;
        TypeIdentifierPtr Int;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Constants>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numConstants());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(c->getConstant(0)->name));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<ArrayType>(id->getDeclaredType()));
        CPPUNIT_ASSERT(Int = std::dynamic_pointer_cast<TypeIdentifier>(type->getInnerType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
        CPPUNIT_ASSERT(value = std::dynamic_pointer_cast<ArrayLiteral>(c->getConstant(0)->initializer));
        CPPUNIT_ASSERT_EQUAL(3, value->numElements());
        ASSERT_EQUALS(L"1", std::dynamic_pointer_cast<IntegerLiteral>(value->getElement(0))->valueAsString);
        ASSERT_EQUALS(L"2", std::dynamic_pointer_cast<IntegerLiteral>(value->getElement(1))->valueAsString);
        ASSERT_EQUALS(L"3", std::dynamic_pointer_cast<IntegerLiteral>(value->getElement(2))->valueAsString);

    }
    
    void testLet_Multiple()
    {
        PARSE_STATEMENT(L"let a=[k1 : 1, k2 : 2], b : Int = 2");
        ConstantsPtr c;
        IdentifierPtr id;
        TypeIdentifierPtr Int;
        DictionaryLiteralPtr dict;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Constants>(root));
        CPPUNIT_ASSERT_EQUAL(2, c->numConstants());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(c->getConstant(0)->name));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(dict = std::dynamic_pointer_cast<DictionaryLiteral>(c->getConstant(0)->initializer));
        CPPUNIT_ASSERT_EQUAL(2, dict->numElements());
        
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(c->getConstant(1)->name));
        ASSERT_EQUALS(L"b", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(Int = std::dynamic_pointer_cast<TypeIdentifier>(id->getDeclaredType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
    }
    
    void testLet_Tuple()
    {
        PARSE_STATEMENT(L"let (a, b) : Int = (1, 2)");
        ConstantsPtr c;
        TuplePtr tuple;
        TypeIdentifierPtr type;
        ParenthesizedExpressionPtr p;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Constants>(root));
        CPPUNIT_ASSERT(tuple = std::dynamic_pointer_cast<Tuple>(c->getConstant(0)->name));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getType()));
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ParenthesizedExpression>(c->getConstant(0)->initializer));
        CPPUNIT_ASSERT_EQUAL(2, p->numExpressions());
        
    }
    
    void testVar()
    {
        PARSE_STATEMENT(L"var currentLoginAttempt = 0");
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        IntegerLiteralPtr i;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(root));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = std::dynamic_pointer_cast<Variable>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"currentLoginAttempt", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
        ASSERT_EQUALS(L"0", i->valueAsString);
        
        
    }
    void testVar_Multiple()
    {
        PARSE_STATEMENT(L"var x = 0.0, y = 0.0, z = 0.0");
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        FloatLiteralPtr f;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(root));
        CPPUNIT_ASSERT_EQUAL(3, vars->numVariables());
        
        CPPUNIT_ASSERT(var = std::dynamic_pointer_cast<Variable>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"x", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FloatLiteral>(var->getInitializer()));
        ASSERT_EQUALS(L"0.0", f->valueAsString);
        
        
        CPPUNIT_ASSERT(var = std::dynamic_pointer_cast<Variable>(vars->getVariable(1)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"y", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FloatLiteral>(var->getInitializer()));
        ASSERT_EQUALS(L"0.0", f->valueAsString);
        
        
        CPPUNIT_ASSERT(var = std::dynamic_pointer_cast<Variable>(vars->getVariable(2)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"z", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FloatLiteral>(var->getInitializer()));
        ASSERT_EQUALS(L"0.0", f->valueAsString);
        
        
    }
    
    void testVar_Typed()
    {
        PARSE_STATEMENT(L"var welcomeMessage: String");
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        TypeIdentifierPtr t;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(root));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = std::dynamic_pointer_cast<Variable>(vars->getVariable(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"welcomeMessage", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(var->getDeclaredType()));
        ASSERT_EQUALS(L"String", t->getName().c_str());
        
        
    }

    void testOperator()
    {
        PARSE_STATEMENT(L"operator infix +- { associativity left precedence 140 }");
        OperatorDefPtr o;
        CPPUNIT_ASSERT(o = std::dynamic_pointer_cast<OperatorDef>(root));
        ASSERT_EQUALS(L"+-", o->getName());
        CPPUNIT_ASSERT_EQUAL(Associativity::Left, o->getAssociativity());
        CPPUNIT_ASSERT_EQUAL(140, o->getPrecedence());


    }

    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestDeclaration, "alltest");



#endif
