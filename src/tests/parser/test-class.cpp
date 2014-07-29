#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

class TestClass : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestClass);
    CPPUNIT_TEST(testStruct);
    CPPUNIT_TEST(testClass);
    CPPUNIT_TEST(testComputedProperty);
    CPPUNIT_TEST(testShorthandSetter);
    CPPUNIT_TEST(testShorthandSetter2);
    CPPUNIT_TEST(testShorthandSetter3);
    CPPUNIT_TEST(testReadonlyComputedProperty);
    CPPUNIT_TEST(testPropertyObservers);
    CPPUNIT_TEST(testPropertyObservers2);
    CPPUNIT_TEST(testPropertyObservers3);
    CPPUNIT_TEST(testTypeProperty_Struct);
    CPPUNIT_TEST(testTypeProperty_Class);
    CPPUNIT_TEST(testTypeProperty_Enum);
    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testMethod_Enum);
    CPPUNIT_TEST(testTypeMethod_Struct);
    CPPUNIT_TEST(testTypeMethod_Enum);
    CPPUNIT_TEST(testTypeMethod_Class);
    CPPUNIT_TEST(testSubscript_Struct);
    CPPUNIT_TEST(testSubscriptOptions);
    CPPUNIT_TEST(testSubclass);
    CPPUNIT_TEST(testOverride_Func);
    CPPUNIT_TEST(testOverride_Var);
    CPPUNIT_TEST(testOverride_PropertyObservers);
    CPPUNIT_TEST(testFinalAttribute);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testConvenienceInitializer);
    CPPUNIT_TEST(testDefaultValueWithClosure);
    CPPUNIT_TEST(testDeinit);
    CPPUNIT_TEST(testWeakReference);
    CPPUNIT_TEST(testUnownedReference);
    CPPUNIT_TEST(testNestedType_Struct);
    CPPUNIT_TEST(testNestedType_Class);
    CPPUNIT_TEST(testNestedType_Enum);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testStruct()
    {
        PARSE_STATEMENT(L"struct Resolution {"
                           L"var width = 0"
                           L"var height = 0"
                           L"}");
        StructDefPtr s;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        ASSERT_EQUALS(L"Resolution", std::static_pointer_cast<TypeIdentifier>(s->getIdentifier())->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
    }
    void testClass()
    {
        PARSE_STATEMENT(L"class VideoMode { "
                           L"var resolution = Resolution() "
                           L"var interlaced = false "
                           L"var frameRate = 0.0 "
                           L"var name: String? "
                           L"}");
        ClassDefPtr c;
        VariablesPtr vars;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        ASSERT_EQUALS(L"VideoMode", std::static_pointer_cast<TypeIdentifier>(c->getIdentifier())->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(4, c->numDeclarations());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(0)));
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(1)));
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(2)));
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(3)));
        
        
        
    }
    void testComputedProperty()
    {
        PARSE_STATEMENT(L"struct Rect {"
                           L"var origin = Point()"
                           L"var center: Point {"
                           L"    get {"
                           L"        return origin"
                           L"    }"
                           L"    set(newCenter) {"
                           L"        origin = newCenter"
                           L"    }"
                           L"}"
                           L"}");
        StructDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        FunctionCallPtr call;
        CodeBlockPtr cb;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(cb = var->getSetter());
        ASSERT_EQUALS(L"newCenter", var->getSetterName());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Assignment>(cb->getStatement(0)));
        
        
    }
    
    
    void testShorthandSetter()
    {
        PARSE_STATEMENT(L"struct Rect {"
                           L"var origin = Point()"
                           L"var center: Point {"
                           L"    get {"
                           L"        return origin"
                           L"    }"
                           L"    set {"
                           L"        origin = newCenter"
                           L"    }"
                           L"}"
                           L"}");
        StructDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        FunctionCallPtr call;
        CodeBlockPtr cb;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(cb = var->getSetter());
        ASSERT_EQUALS(L"", var->getSetterName());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Assignment>(cb->getStatement(0)));
        
        
    }
    
    void testShorthandSetter2()
    {
        PARSE_STATEMENT(L"struct Rect {"
                           L"var origin = Point()"
                           L"var center: Point {"
                           L"    set {"
                           L"        origin = newCenter"
                           L"    }"
                           L"    get {"
                           L"        return origin"
                           L"    }"
                           L"}"
                           L"}");
        StructDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        FunctionCallPtr call;
        CodeBlockPtr cb;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(cb = var->getSetter());
        ASSERT_EQUALS(L"", var->getSetterName());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Assignment>(cb->getStatement(0)));
        
        
    }
    
    void testShorthandSetter3()
    {
        PARSE_STATEMENT(L"struct Rect {"
                           L"var origin = Point()"
                           L"var center: Point {"
                           L"    get {"
                           L"        return origin"
                           L"    }"
                           L"}"
                           L"}");
        StructDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        IdentifierPtr id;
        FunctionCallPtr call;
        CodeBlockPtr cb;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(NULL == var->getSetter());
        
    }
    
    void testReadonlyComputedProperty()
    {
        PARSE_STATEMENT(L"struct Cuboid { "
                           L"var width = 0.0, height = 0.0, depth = 0.0 "
                           L"var volume: Double { "
                           L"    return width * height * depth "
                           L"} "
                           L"}");
        StructDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        CodeBlockPtr cb;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());

        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"volume", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
        
    }
    void testPropertyObservers()
    {
        PARSE_STATEMENT(L"class StepCounter { "
                           L"var totalSteps: Int = 0 { "
                           L"    willSet(newTotalSteps) { "
                           L"        println(\"About to set totalSteps to \(newTotalSteps)\") "
                           L"    } "
                           L"    didSet { "
                           L"        if totalSteps > oldValue  { "
                           L"            println(\"Added \(totalSteps - oldValue) steps\") "
                           L"        } "
                           L"    } "
                           L"} "
                           L"}");
        
        ClassDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        CodeBlockPtr cb;
        IntegerLiteralPtr i;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"totalSteps", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(NULL == var->getGetter());
        
        
        CPPUNIT_ASSERT(cb = var->getWillSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(cb->getStatement(0)));
        
        CPPUNIT_ASSERT(cb = var->getDidSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<IfStatement>(cb->getStatement(0)));
        
        
    }
    
    void testPropertyObservers2()
    {
        PARSE_STATEMENT(L"class StepCounter { "
                           L"var totalSteps: Int = 0 { "
                           L"    didSet { "
                           L"        if totalSteps > oldValue  { "
                           L"            println(\"Added \(totalSteps - oldValue) steps\") "
                           L"        } "
                           L"    } "
                           L"    willSet(newTotalSteps) { "
                           L"        println(\"About to set totalSteps to \(newTotalSteps)\") "
                           L"    } "
                           L"} "
                           L"}");
        
        ClassDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        CodeBlockPtr cb;
        IntegerLiteralPtr i;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"totalSteps", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(NULL == var->getGetter());
        
        
        CPPUNIT_ASSERT(cb = var->getWillSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(cb->getStatement(0)));
        
        CPPUNIT_ASSERT(cb = var->getDidSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<IfStatement>(cb->getStatement(0)));
        
        
    }
    
    void testPropertyObservers3()
    {
        PARSE_STATEMENT(L"class StepCounter { "
                           L"var totalSteps: Int = 0 { "
                           L"    willSet(newTotalSteps) { "
                           L"        println(\"About to set totalSteps to \(newTotalSteps)\") "
                           L"    } "
                           L"} "
                           L"}");
        
        ClassDefPtr s;
        VariablesPtr vars;
        VariablePtr var;
        CodeBlockPtr cb;
        IntegerLiteralPtr i;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"totalSteps", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(NULL == var->getGetter());
        
        
        CPPUNIT_ASSERT(cb = var->getWillSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(cb->getStatement(0)));
        
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        
    }
    
    void testTypeProperty_Struct()
    {
        PARSE_STATEMENT(L"struct SomeStructure {"
                           L"static var storedTypeProperty = \"Some value.\""
                           L"static var computedTypeProperty: Int {"
                           L"    // return an Int value here\n"
                           L"}"
                           L"}");
        StructDefPtr s;
        VariablesPtr vars;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
    }
    
    void testTypeProperty_Class()
    {
        PARSE_STATEMENT(L"class SomeStructure {"
                           L"static var storedTypeProperty = \"Some value.\""
                           L"static var computedTypeProperty: Int {"
                           L"    // return an Int value here\n"
                           L"}"
                           L"}");
        ClassDefPtr s;
        VariablesPtr vars;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
    }
    
    void testTypeProperty_Enum()
    {
        PARSE_STATEMENT(L"enum SomeStructure {"
                           L"static var storedTypeProperty = \"Some value.\""
                           L"static var computedTypeProperty: Int {"
                           L"    // return an Int value here\n"
                           L"}"
                           L"}");
        EnumDefPtr s;
        VariablesPtr vars;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
    }
    
    void testMethod()
    {
        PARSE_STATEMENT(L"struct Point { "
                           L"var x = 0.0, y = 0.0 "
                           L"mutating func moveByX(deltaX: Double, y deltaY: Double) { "
                           L"    x += deltaX "
                           L"    y += deltaY "
                           L"} "
                           L"}");
        
        StructDefPtr s;
        VariablesPtr vars;
        FunctionDefPtr f;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(0, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Mutating, f->getSpecifiers());
        
    }
    
    void testMethod_Enum()
    {
        PARSE_STATEMENT(L"enum TriStateSwitch { "
                           L"case Off, Low, High "
                           L"mutating func next() { "
                           L"    switch self { "
                           L"    case Off: "
                           L"        self = Low "
                           L"    case Low: "
                           L"        self = High "
                           L"    case High: "
                           L"        self = Off "
                           L"    } "
                           L"} "
                           L"}");
        
        EnumDefPtr s;
        FunctionDefPtr f;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Mutating, f->getSpecifiers());
        
        
    }
    
    void testTypeMethod_Struct()
    {
        PARSE_STATEMENT(L"struct SomeClass {\n"
                           L"static func someTypeMethod() {\n"
                           L"    // type method implementation goes here\n"
                           L"}\n"
                           L"}");
        
        StructDefPtr s;
        FunctionDefPtr f;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, f->getSpecifiers());
        
    }
    
    void testTypeMethod_Enum()
    {
        PARSE_STATEMENT(L"enum SomeClass {\n"
                           L"static func someTypeMethod() {\n"
                           L"    // type method implementation goes here\n"
                           L"}\n"
                           L"}");
        
        EnumDefPtr s;
        FunctionDefPtr f;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, f->getSpecifiers());
        
    }
    
    void testTypeMethod_Class()
    {
        PARSE_STATEMENT(L"class SomeClass {\n"
                           L"class func someTypeMethod() {\n"
                           L"    // type method implementation goes here\n"
                           L"}\n"
                           L"}");
        
        ClassDefPtr s;
        FunctionDefPtr f;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Class, f->getSpecifiers());
        
    }
    
    void testSubscript_Struct()
    {
        PARSE_STATEMENT(L"struct TimesTable {\n"
                           L"let multiplier: Int\n"
                           L"subscript(index: Int) -> Int {\n"
                           L"    return multiplier * index\n"
                           L"}\n"
                           L"}");
        
        StructDefPtr s;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        SubscriptDefPtr sub;
        
        CPPUNIT_ASSERT(sub = std::dynamic_pointer_cast<SubscriptDef>(s->getDeclaration(1)));
        
    }
    void testSubscriptOptions()
    {
        PARSE_STATEMENT(
                           L"struct Matrix {\n"
                           L"    subscript(row: Int, column: Int) -> Double {\n"
                           L"        get {\n"
                           L"            return grid[(row * columns) + column]\n"
                           L"        }\n"
                           L"        set {\n"
                           L"            grid[(row * columns) + column] = newValue\n"
                           L"        }\n"
                           L"    }\n"
                           L"}\n");
        
        StructDefPtr s;
        ParameterPtr p;
        ParametersPtr args;
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        SubscriptDefPtr sub;
        
        CPPUNIT_ASSERT(sub = std::dynamic_pointer_cast<SubscriptDef>(s->getDeclaration(0)));
        CPPUNIT_ASSERT(args = sub->getParameters());
        CPPUNIT_ASSERT_EQUAL(2, args->numParameters());
        CPPUNIT_ASSERT(p =  args->getParameter(0));
        ASSERT_EQUALS(L"row", p->getLocalName());
        CPPUNIT_ASSERT(p =  args->getParameter(1));
        ASSERT_EQUALS(L"column", p->getLocalName());
        
    }
    
    void testSubclass()
    {
        PARSE_STATEMENT(L"class Bicycle: Vehicle {"
                           L"init() {"
                           L"    super.init()"
                           L"    numberOfWheels = 2"
                           L"}"
                           L"}");
        ClassDefPtr c;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
        ASSERT_EQUALS(L"Vehicle", t->getName().c_str());
        
        
        InitializerDefPtr init;
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(init = std::dynamic_pointer_cast<InitializerDef>(c->getDeclaration(0)));
        
        
        
    }
    
    void testOverride_Func()
    {
        PARSE_STATEMENT(L"class Car: Vehicle {\n"
                        L"var speed: Double = 0.0\n"
                        L"init() {\n"
                        L"    super.init()\n"
                        L"    maxPassengers = 5\n"
                        L"    numberOfWheels = 4\n"
                        L"}\n"
                        L"override func description() -> String {\n"
                        L"    return super.description() + \"; \"\n"
                        L"    + \"traveling at \(speed) mph\"\n"
                        L"}\n"
                        L"}");
        
        ClassDefPtr c;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
        ASSERT_EQUALS(L"Vehicle", t->getName().c_str());
        
        
        FunctionDefPtr f;
        CPPUNIT_ASSERT_EQUAL(3, c->numDeclarations());
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionDef>(c->getDeclaration(2)));
        ASSERT_EQUALS(L"description", f->getName().c_str());
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Override, f->getSpecifiers());
        
        
    
    }
    void testOverride_Var()
    {
        PARSE_STATEMENT(L"class SpeedLimitedCar: Car {\n"
                        L"override var speed: Double  {\n"
                        L"    get {\n"
                        L"        return super.speed\n"
                        L"    }\n"
                        L"    set {\n"
                        L"        super.speed = min(newValue, 40.0)\n"
                        L"    }\n"
                        L"}\n"
                        L"}");
        
        
        ClassDefPtr c;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
        ASSERT_EQUALS(L"Car", t->getName().c_str());
        
        
        VariablesPtr vars;
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        VariablePtr var;
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        IdentifierPtr id;
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"speed", id->getIdentifier().c_str());
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Override, var->getSpecifiers());
        
        
    }
    
    void testOverride_PropertyObservers()
    {
        PARSE_STATEMENT(L"class AutomaticCar: Car {\n"
                        L"var gear = 1\n"
                        L"override var speed: Double {\n"
                        L"    didSet {\n"
                        L"        gear = Int(speed / 10.0) + 1\n"
                        L"    }\n"
                        L"}\n"
                        L"override func description() -> String {\n"
                        L"    return super.description() + \" in gear \(gear)\"\n"
                        L"}\n"
                        L"}");
        
        
        ClassDefPtr c;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
        ASSERT_EQUALS(L"Car", t->getName().c_str());
        
        
        VariablesPtr vars;
        CPPUNIT_ASSERT_EQUAL(3, c->numDeclarations());
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        VariablePtr var;
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        IdentifierPtr id;
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
        ASSERT_EQUALS(L"speed", id->getIdentifier().c_str());
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Override, var->getSpecifiers());
        
        
        
    }
    
    
    void testFinalAttribute()
    {
        PARSE_STATEMENT(L"@final class AutomaticCar: Car {\n"
                        L"@final var gear = 1\n"
                        L"@final func description() -> String {\n"
                        L"    return super.description() + \" in gear \(gear)\"\n"
                        L"}\n"
                        L"@final subscript(index: Int) -> Int {\n"
                        L"    return multiplier * index\n"
                        L"}\n"
                        L"}");
        
        
        ClassDefPtr c;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        
        CPPUNIT_ASSERT_EQUAL(1, (int)c->getAttributes().size());
        ASSERT_EQUALS(L"final", c->getAttributes().front()->getName());
        
        
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
        ASSERT_EQUALS(L"Car", t->getName().c_str());
        
        CPPUNIT_ASSERT_EQUAL(3, c->numDeclarations());
        VariablesPtr vars;
        FunctionDefPtr func;
        SubscriptDefPtr sub;
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(c->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, (int)vars->getAttributes().size());
        ASSERT_EQUALS(L"final", vars->getAttributes().front()->getName());
        
        
        CPPUNIT_ASSERT(func = std::dynamic_pointer_cast<FunctionDef>(c->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, (int)func->getAttributes().size());
        ASSERT_EQUALS(L"final", func->getAttributes().front()->getName());
        
        
        CPPUNIT_ASSERT(sub = std::dynamic_pointer_cast<SubscriptDef>(c->getDeclaration(2)));
        CPPUNIT_ASSERT_EQUAL(1, (int)sub->getAttributes().size());
        ASSERT_EQUALS(L"final", sub->getAttributes().front()->getName());
        
        
    }
    void testInit()
    {
        PARSE_STATEMENT(L"struct Color {\n"
                        L"let red = 0.0, green = 0.0, blue = 0.0\n"
                        L"init(red: Double, green: Double, blue: Double) {\n"
                        L"  self.red   = red\n"
                        L"  self.green = green\n"
                        L"  self.blue  = blue\n"
                        L"}\n"
                        L"}\n");
        StructDefPtr s;
        ConstantsPtr let;
        InitializerDefPtr init;
        ParametersPtr params;
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(let = std::dynamic_pointer_cast<Constants>(s->getDeclaration(0)));
        CPPUNIT_ASSERT(init = std::dynamic_pointer_cast<InitializerDef>(s->getDeclaration(1)));
        CPPUNIT_ASSERT(!init->isConvenience());
        CPPUNIT_ASSERT(params = std::dynamic_pointer_cast<Parameters>(init->getParameters()));
        CPPUNIT_ASSERT_EQUAL(3, params->numParameters());
        
        
    }
    
    
    
    void testConvenienceInitializer()
    {
        PARSE_STATEMENT(L"class Food {\n"
                        L"  var name: String\n"
                        L"  init(name: String) {\n"
                        L"      self.name = name\n"
                        L"  }\n"
                        L"  convenience init() {\n"
                        L"      self.init(name: \"[Unnamed]\")\n"
                        L"  }\n"
                        L"}");
        
        ClassDefPtr s;

        InitializerDefPtr init;
        ParametersPtr params;
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(3, s->numDeclarations());
        
        CPPUNIT_ASSERT(init = std::dynamic_pointer_cast<InitializerDef>(s->getDeclaration(2)));
        CPPUNIT_ASSERT(init->isConvenience());
        CPPUNIT_ASSERT(params = std::dynamic_pointer_cast<Parameters>(init->getParameters()));
        CPPUNIT_ASSERT_EQUAL(0, params->numParameters());
    }
    
    
    void testDefaultValueWithClosure()
    {
        
        PARSE_STATEMENT(L"class SomeClass {\n"
                        L"let someProperty: SomeType = {\n"
                        L"    return someValue\n"
                        L"}()\n"
                        L"}");
        
         
    }
    
    
    
    
    
    void testDeinit()
    {
        PARSE_STATEMENT(L"class Player {\n"
                        L"var coinsInPurse: Int\n"
                        L"init(coins: Int) {\n"
                        L"    coinsInPurse = Bank.vendCoins(coins)\n"
                        L"}\n"
                        L"func winCoins(coins: Int) {\n"
                        L"    coinsInPurse += Bank.vendCoins(coins)\n"
                        L"}\n"
                        L"deinit {\n"
                        L"    Bank.receiveCoins(coinsInPurse)\n"
                        L"}\n"
                        L"}");
        
        ClassDefPtr s;
        DeinitializerDefPtr deinit;
        CodeBlockPtr body;
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(4, s->numDeclarations());
        
        CPPUNIT_ASSERT(deinit = std::dynamic_pointer_cast<DeinitializerDef>(s->getDeclaration(3)));
        CPPUNIT_ASSERT(body = deinit->getBody());
        CPPUNIT_ASSERT_EQUAL(1, body->numStatements());
        
        
    }
    
    
    void testWeakReference()
    {
        PARSE_STATEMENT(L"class Apartment {\n"
                        L"let number: Int\n"
                        L"init(number: Int) { self.number = number }\n"
                        L"weak var tenant: Person?\n"
                        L"deinit { println(\"Apartment #\(number) is being deinitialized\") }\n"
                        L"}");
        
        ClassDefPtr s;
        VariablesPtr vars;
        
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(4, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = std::dynamic_pointer_cast<Variables>(s->getDeclaration(2)));
        CPPUNIT_ASSERT(vars->getSpecifiers() && TypeSpecifier::Weak);

    }
    
    void testUnownedReference()
    {
        PARSE_STATEMENT(L"class CreditCard {\n"
                        L"let number: Int\n"
                        L"unowned let customer: Customer\n"
                        L"init(number: Int, customer: Customer) {\n"
                        L"    self.number = number\n"
                        L"    self.customer = customer\n"
                        L"}\n"
                        L"deinit { println(\"Card #\(number) is being deinitialized\") }\n"
                        L"}");
        
        ClassDefPtr s;
        ConstantsPtr let;
        
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(4, s->numDeclarations());
        
        CPPUNIT_ASSERT(let = std::dynamic_pointer_cast<Constants>(s->getDeclaration(1)));
        CPPUNIT_ASSERT(let->getSpecifiers() && TypeSpecifier::Unowned);

    }
    
    void testNestedType_Struct()
    {
        PARSE_STATEMENT(L"class Foo\n"
                        L"{\n"
                        L"  struct Bar\n"
                        L"  {\n"
                        L"  }\n"
                        L"}");
        ClassDefPtr c;
        StructDefPtr s;
        
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(c->getDeclaration(0)));
        
    }
    
    void testNestedType_Class()
    {
        PARSE_STATEMENT(L"struct Foo\n"
                        L"{\n"
                        L"  class Bar\n"
                        L"  {\n"
                        L"  }\n"
                        L"}");
        ClassDefPtr c;
        StructDefPtr s;
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<ClassDef>(s->getDeclaration(0)));
        
    }
    
    
    void testNestedType_Enum()
    {
        PARSE_STATEMENT(L"struct Foo\n"
                        L"{\n"
                        L"  enum Bar\n"
                        L"  {\n"
                        L"  }\n"
                        L"}");
        EnumDefPtr c;
        StructDefPtr s;
        
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StructDef>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<EnumDef>(s->getDeclaration(0)));
        
    }
    
    
    
    
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestClass, "alltest");



#endif
