#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

class TestProtocol : public SwiftTestCase
{
    
    CPPUNIT_TEST_SUITE(TestProtocol);
    CPPUNIT_TEST(testEmptyProtocol);
    CPPUNIT_TEST(testStruct);
    CPPUNIT_TEST(testClass);
    CPPUNIT_TEST(testPropertyRequirements);
    CPPUNIT_TEST(testPropertyRequirements2);
    CPPUNIT_TEST(testPropertyRequirements3);
    CPPUNIT_TEST(testMethodRequirements);
    CPPUNIT_TEST(testMethodRequirements2);
    CPPUNIT_TEST(testMutatingMethod);
    CPPUNIT_TEST(testInheritance);
    CPPUNIT_TEST(testOptional);
    CPPUNIT_TEST_SUITE_END();
public:
    void testEmptyProtocol()
    {
        PARSE_STATEMENT(L"protocol SomeProtocol {\n"
                        L"// protocol definition goes here\n"
                        L"}");
        ProtocolDefPtr p;
        TypeIdentifierPtr id;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<TypeIdentifier>(p->getIdentifier()));
        ASSERT_EQUALS(L"SomeProtocol", id->getName());
    }
    
    void testStruct()
    {
        PARSE_STATEMENT(L"struct SomeStructure: FirstProtocol, AnotherProtocol {\n"
                        L"// structure definition goes here\n"
                        L"}");
        StructDefPtr c;
        
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, c->numParents());
    }
    
    void testClass()
    {
        PARSE_STATEMENT(L"class SomeClass: SomeSuperclass, FirstProtocol, AnotherProtocol {\n"
                        L"// class definition goes here\n"
                        L"}");
        ClassDefPtr c;
        
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(3, c->numParents());
    }
    
    void testPropertyRequirements()
    {
        PARSE_STATEMENT(L"protocol SomeProtocol {\n"
                        L"var mustBeSettable: Int { get set }\n"
                        L"var doesNotNeedToBeSettable: Int { get }\n"
                        L"}");
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, p->numDeclarations());
        VariablesPtr vars;
        VariablePtr var;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"mustBeSettable", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        CPPUNIT_ASSERT_EQUAL(0, vars->getSpecifiers());
        CPPUNIT_ASSERT(NULL != var->getGetter());
        CPPUNIT_ASSERT(NULL != var->getSetter());
        
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(p->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"doesNotNeedToBeSettable", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        CPPUNIT_ASSERT(NULL != var->getGetter());
        CPPUNIT_ASSERT(NULL == var->getSetter());
        
    }
    
    void testPropertyRequirements2()
    {
        PARSE_STATEMENT(L"protocol AnotherProtocol {\n"
                        L"class var someTypeProperty: Int { get set }\n"
                        L"}");
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, p->numDeclarations());
        VariablesPtr vars;
        VariablePtr var;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Class, vars->getSpecifiers());
        ASSERT_EQUALS(L"someTypeProperty", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        CPPUNIT_ASSERT(NULL != var->getGetter());
        CPPUNIT_ASSERT(NULL != var->getSetter());
        
    }
    void testPropertyRequirements3()
    {
        PARSE_STATEMENT(L"protocol FullyNamed {\n"
                        L"var fullName: String { get }\n"
                        L"}");
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, p->numDeclarations());
        VariablesPtr vars;
        VariablePtr var;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        CPPUNIT_ASSERT_EQUAL(0, vars->getSpecifiers());
        ASSERT_EQUALS(L"fullName", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        CPPUNIT_ASSERT(NULL != var->getGetter());
        CPPUNIT_ASSERT(NULL == var->getSetter());
        
    }
    
    void testMethodRequirements()
    {
        PARSE_STATEMENT(L"protocol SomeProtocol {\n"
                        L"class func someTypeMethod()\n"
                        L"}");
        
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, p->numDeclarations());
        FunctionDefPtr f;
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Class, f->getSpecifiers());
        ASSERT_EQUALS(L"someTypeMethod", f->getName());
        CPPUNIT_ASSERT(NULL == f->getBody());
    }
    
    void testMethodRequirements2()
    {
        PARSE_STATEMENT(L"protocol RandomNumberGenerator {\n"
                        L"func random() -> Double\n"
                        L"}");
        
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, p->numDeclarations());
        FunctionDefPtr f;
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(0, f->getSpecifiers());
        ASSERT_EQUALS(L"random", f->getName());
        CPPUNIT_ASSERT(NULL == f->getBody());
    }
    
    
    
    void testMutatingMethod()
    {
        PARSE_STATEMENT(L"protocol Togglable {\n"
                        L"mutating func toggle()\n"
                        L"}");
        
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, p->numDeclarations());
        FunctionDefPtr f;
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Mutating, f->getSpecifiers());
        ASSERT_EQUALS(L"toggle", f->getName());
        CPPUNIT_ASSERT(NULL == f->getBody());
    }
    
    void testInheritance()
    {
        PARSE_STATEMENT(L"protocol PrettyTextRepresentable: TextRepresentable {\n"
                        L"func asPrettyText() -> String\n"
                        L"}");
        
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, p->numDeclarations());
        CPPUNIT_ASSERT_EQUAL(1, p->numParents());
        ASSERT_EQUALS(L"TextRepresentable", p->getParent(0)->getName());
        
        
        FunctionDefPtr f;
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(0, f->getSpecifiers());
        ASSERT_EQUALS(L"asPrettyText", f->getName());
        CPPUNIT_ASSERT(NULL == f->getBody());
        
        
    }
    void testOptional()
    {
        PARSE_STATEMENT(L"@objc protocol CounterDataSource {\n"
                        L"@optional func incrementForCount(count: Int) -> Int\n"
                        L"@optional var fixedIncrement: Int { get }\n"
                        L"}");
        ProtocolDefPtr p;
        CPPUNIT_ASSERT(p = std::dynamic_pointer_cast<ProtocolDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, p->numDeclarations());
        CPPUNIT_ASSERT(p->getAttribute(L"objc"));
        
        
        
        FunctionDefPtr f;
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(0, f->getSpecifiers());
        ASSERT_EQUALS(L"incrementForCount", f->getName());
        CPPUNIT_ASSERT(NULL == f->getBody());
        CPPUNIT_ASSERT(f->getAttribute(L"optional"));
        
        VariablesPtr vars;
        VariablePtr var;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(p->getDeclaration(1)));
        CPPUNIT_ASSERT(vars->getAttribute(L"optional"));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        CPPUNIT_ASSERT_EQUAL(0, vars->getSpecifiers());
        ASSERT_EQUALS(L"fixedIncrement", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        CPPUNIT_ASSERT(NULL != var->getGetter());
        CPPUNIT_ASSERT(NULL == var->getSetter());
        
    }
    
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestProtocol, "alltest");



#endif
