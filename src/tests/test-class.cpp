#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"

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
        StructDef* s = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        ASSERT_EQUALS(L"Resolution", static_cast<TypeIdentifier*>(s->getIdentifier())->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        DESTROY(root);
    }
    void testClass()
    {
        PARSE_STATEMENT(L"class VideoMode { "
                           L"var resolution = Resolution() "
                           L"var interlaced = false "
                           L"var frameRate = 0.0 "
                           L"var name: String? "
                           L"}");
        ClassDef* c = NULL;
        Variables* vars = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        ASSERT_EQUALS(L"VideoMode", static_cast<TypeIdentifier*>(c->getIdentifier())->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(4, c->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(0)));
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(1)));
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(2)));
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(3)));
        
        
        
        DESTROY(root);
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
        StructDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        FunctionCall* call = NULL;
        CodeBlock* cb = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(cb = var->getSetter());
        ASSERT_EQUALS(L"newCenter", var->getSetterName());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<Assignment*>(cb->getStatement(0)));
        
        
        DESTROY(root);
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
        StructDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        FunctionCall* call = NULL;
        CodeBlock* cb = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(cb = var->getSetter());
        ASSERT_EQUALS(L"", var->getSetterName());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<Assignment*>(cb->getStatement(0)));
        
        
        DESTROY(root);
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
        StructDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        FunctionCall* call = NULL;
        CodeBlock* cb = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(cb = var->getSetter());
        ASSERT_EQUALS(L"", var->getSetterName());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<Assignment*>(cb->getStatement(0)));
        
        
        DESTROY(root);
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
        StructDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        Identifier* id = NULL;
        FunctionCall* call = NULL;
        CodeBlock* cb = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"origin", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(var->getInitializer()));
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numExpressions());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"Point", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"center", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(NULL == var->getSetter());
        
        DESTROY(root);
    }
    
    void testReadonlyComputedProperty()
    {
        PARSE_STATEMENT(L"struct Cuboid { "
                           L"var width = 0.0, height = 0.0, depth = 0.0 "
                           L"var volume: Double { "
                           L"    return width * height * depth "
                           L"} "
                           L"}");
        StructDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        CodeBlock* cb = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());

        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"volume", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        CPPUNIT_ASSERT(NULL == var->getWillSet());
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        
        DESTROY(root);
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
        
        ClassDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        CodeBlock* cb = NULL;
        IntegerLiteral* i = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"totalSteps", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(var->getInitializer()));
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(NULL == var->getGetter());
        
        
        CPPUNIT_ASSERT(cb = var->getWillSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(cb->getStatement(0)));
        
        CPPUNIT_ASSERT(cb = var->getDidSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<IfStatement*>(cb->getStatement(0)));
        
        
        DESTROY(root);
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
        
        ClassDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        CodeBlock* cb = NULL;
        IntegerLiteral* i = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"totalSteps", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(var->getInitializer()));
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(NULL == var->getGetter());
        
        
        CPPUNIT_ASSERT(cb = var->getWillSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(cb->getStatement(0)));
        
        CPPUNIT_ASSERT(cb = var->getDidSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<IfStatement*>(cb->getStatement(0)));
        
        
        DESTROY(root);
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
        
        ClassDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        CodeBlock* cb = NULL;
        IntegerLiteral* i = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"totalSteps", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(var->getInitializer()));
        CPPUNIT_ASSERT(NULL == var->getSetter());
        CPPUNIT_ASSERT(NULL == var->getGetter());
        
        
        CPPUNIT_ASSERT(cb = var->getWillSet());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(cb->getStatement(0)));
        
        CPPUNIT_ASSERT(NULL == var->getDidSet());
        
        DESTROY(root);
    }
    
    void testTypeProperty_Struct()
    {
        PARSE_STATEMENT(L"struct SomeStructure {"
                           L"static var storedTypeProperty = \"Some value.\""
                           L"static var computedTypeProperty: Int {"
                           L"    // return an Int value here\n"
                           L"}"
                           L"}");
        StructDef* s = NULL;
        Variables* vars = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        DESTROY(root);
    }
    
    void testTypeProperty_Class()
    {
        PARSE_STATEMENT(L"class SomeStructure {"
                           L"static var storedTypeProperty = \"Some value.\""
                           L"static var computedTypeProperty: Int {"
                           L"    // return an Int value here\n"
                           L"}"
                           L"}");
        ClassDef* s = NULL;
        Variables* vars = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        DESTROY(root);
    }
    
    void testTypeProperty_Enum()
    {
        PARSE_STATEMENT(L"enum SomeStructure {"
                           L"static var storedTypeProperty = \"Some value.\""
                           L"static var computedTypeProperty: Int {"
                           L"    // return an Int value here\n"
                           L"}"
                           L"}");
        EnumDef* s = NULL;
        Variables* vars = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<EnumDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, vars->getSpecifiers());
        
        DESTROY(root);
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
        
        StructDef* s = NULL;
        Variables* vars = NULL;
        FunctionDef* f = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(0, vars->getSpecifiers());
        
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Mutating, f->getSpecifiers());
        
        DESTROY(root);
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
        
        EnumDef* s = NULL;
        FunctionDef* f = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<EnumDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Mutating, f->getSpecifiers());
        
        
        DESTROY(root);
    }
    
    void testTypeMethod_Struct()
    {
        PARSE_STATEMENT(L"struct SomeClass {\n"
                           L"static func someTypeMethod() {\n"
                           L"    // type method implementation goes here\n"
                           L"}\n"
                           L"}");
        
        StructDef* s = NULL;
        FunctionDef* f = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, f->getSpecifiers());
        
        DESTROY(root);
    }
    
    void testTypeMethod_Enum()
    {
        PARSE_STATEMENT(L"enum SomeClass {\n"
                           L"static func someTypeMethod() {\n"
                           L"    // type method implementation goes here\n"
                           L"}\n"
                           L"}");
        
        EnumDef* s = NULL;
        FunctionDef* f = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<EnumDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Static, f->getSpecifiers());
        
        DESTROY(root);
    }
    
    void testTypeMethod_Class()
    {
        PARSE_STATEMENT(L"class SomeClass {\n"
                           L"class func someTypeMethod() {\n"
                           L"    // type method implementation goes here\n"
                           L"}\n"
                           L"}");
        
        ClassDef* s = NULL;
        FunctionDef* f = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Class, f->getSpecifiers());
        
        DESTROY(root);
    }
    
    void testSubscript_Struct()
    {
        PARSE_STATEMENT(L"struct TimesTable {\n"
                           L"let multiplier: Int\n"
                           L"subscript(index: Int) -> Int {\n"
                           L"    return multiplier * index\n"
                           L"}\n"
                           L"}");
        
        StructDef* s = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        SubscriptDef* sub = NULL;
        
        CPPUNIT_ASSERT(sub = dynamic_cast<SubscriptDef*>(s->getDeclaration(1)));
        
        DESTROY(root);
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
        
        StructDef* s = NULL;
        Parameter* p = NULL;
        Parameters* args = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        SubscriptDef* sub = NULL;
        
        CPPUNIT_ASSERT(sub = dynamic_cast<SubscriptDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT(args = sub->getParameters());
        CPPUNIT_ASSERT_EQUAL(2, args->numParameters());
        CPPUNIT_ASSERT(p =  args->getParameter(0));
        ASSERT_EQUALS(L"row", p->getLocalName());
        CPPUNIT_ASSERT(p =  args->getParameter(1));
        ASSERT_EQUALS(L"column", p->getLocalName());
        
        DESTROY(root);
    }
    
    void testSubclass()
    {
        PARSE_STATEMENT(L"class Bicycle: Vehicle {"
                           L"init() {"
                           L"    super.init()"
                           L"    numberOfWheels = 2"
                           L"}"
                           L"}");
        ClassDef* c = NULL;
        TypeIdentifier* t = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(c->getParent(0)));
        ASSERT_EQUALS(L"Vehicle", t->getName().c_str());
        
        
        InitializerDef* init = NULL;
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(init = dynamic_cast<InitializerDef*>(c->getDeclaration(0)));
        
        
        
        DESTROY(root);
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
        
        ClassDef* c = NULL;
        TypeIdentifier* t = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(c->getParent(0)));
        ASSERT_EQUALS(L"Vehicle", t->getName().c_str());
        
        
        FunctionDef* f = NULL;
        CPPUNIT_ASSERT_EQUAL(3, c->numDeclarations());
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(c->getDeclaration(2)));
        ASSERT_EQUALS(L"description", f->getName().c_str());
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Override, f->getSpecifiers());
        
        
    
        DESTROY(root);
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
        
        
        ClassDef* c = NULL;
        TypeIdentifier* t = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(c->getParent(0)));
        ASSERT_EQUALS(L"Car", t->getName().c_str());
        
        
        Variables* vars = NULL;
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        Variable* var = NULL;
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        Identifier* id = NULL;
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"speed", id->getIdentifier().c_str());
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Override, var->getSpecifiers());
        
        
        DESTROY(root);
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
        
        
        ClassDef* c = NULL;
        TypeIdentifier* t = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(c->getParent(0)));
        ASSERT_EQUALS(L"Car", t->getName().c_str());
        
        
        Variables* vars = NULL;
        CPPUNIT_ASSERT_EQUAL(3, c->numDeclarations());
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        Variable* var = NULL;
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        Identifier* id = NULL;
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(var->getName()));
        ASSERT_EQUALS(L"speed", id->getIdentifier().c_str());
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Override, var->getSpecifiers());
        
        
        
        DESTROY(root);
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
        
        
        ClassDef* c = NULL;
        TypeIdentifier* t = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        
        CPPUNIT_ASSERT_EQUAL(1, (int)c->getAttributes().size());
        ASSERT_EQUALS(L"final", c->getAttributes().front()->getName());
        
        
        CPPUNIT_ASSERT_EQUAL(1, c->numParents());
        CPPUNIT_ASSERT(t = dynamic_cast<TypeIdentifier*>(c->getParent(0)));
        ASSERT_EQUALS(L"Car", t->getName().c_str());
        
        CPPUNIT_ASSERT_EQUAL(3, c->numDeclarations());
        Variables* vars = NULL;
        FunctionDef* func = NULL;
        SubscriptDef* sub = NULL;
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(c->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, (int)vars->getAttributes().size());
        ASSERT_EQUALS(L"final", vars->getAttributes().front()->getName());
        
        
        CPPUNIT_ASSERT(func = dynamic_cast<FunctionDef*>(c->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, (int)func->getAttributes().size());
        ASSERT_EQUALS(L"final", func->getAttributes().front()->getName());
        
        
        CPPUNIT_ASSERT(sub = dynamic_cast<SubscriptDef*>(c->getDeclaration(2)));
        CPPUNIT_ASSERT_EQUAL(1, (int)sub->getAttributes().size());
        ASSERT_EQUALS(L"final", sub->getAttributes().front()->getName());
        
        
        DESTROY(root);
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
        StructDef* s = NULL;
        Constant* let = NULL;
        InitializerDef* init = NULL;
        Parameters* params = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(let = dynamic_cast<Constant*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT(init = dynamic_cast<InitializerDef*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT(!init->isConvenience());
        CPPUNIT_ASSERT(params = dynamic_cast<Parameters*>(init->getParameters()));
        CPPUNIT_ASSERT_EQUAL(3, params->numParameters());
        
        
        DESTROY(root);
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
        
        ClassDef* s = NULL;

        InitializerDef* init = NULL;
        Parameters* params = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(3, s->numDeclarations());
        
        CPPUNIT_ASSERT(init = dynamic_cast<InitializerDef*>(s->getDeclaration(2)));
        CPPUNIT_ASSERT(init->isConvenience());
        CPPUNIT_ASSERT(params = dynamic_cast<Parameters*>(init->getParameters()));
        CPPUNIT_ASSERT_EQUAL(0, params->numParameters());
        DESTROY(root);
    }
    
    
    void testDefaultValueWithClosure()
    {
        
        PARSE_STATEMENT(L"class SomeClass {\n"
                        L"let someProperty: SomeType = {\n"
                        L"    return someValue\n"
                        L"}()\n"
                        L"}");
        
        DESTROY(root);
         
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
        
        ClassDef* s = NULL;
        DeinitializerDef* deinit = NULL;
        CodeBlock* body = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(4, s->numDeclarations());
        
        CPPUNIT_ASSERT(deinit = dynamic_cast<DeinitializerDef*>(s->getDeclaration(3)));
        CPPUNIT_ASSERT(body = deinit->getBody());
        CPPUNIT_ASSERT_EQUAL(1, body->numStatements());
        
        
        DESTROY(root);
    }
    
    
    void testWeakReference()
    {
        PARSE_STATEMENT(L"class Apartment {\n"
                        L"let number: Int\n"
                        L"init(number: Int) { self.number = number }\n"
                        L"weak var tenant: Person?\n"
                        L"deinit { println(\"Apartment #\(number) is being deinitialized\") }\n"
                        L"}");
        
        ClassDef* s = NULL;
        Variables* vars = NULL;
        
        
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(4, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(2)));
        CPPUNIT_ASSERT(vars->getSpecifiers() && TypeSpecifier::Weak);

        DESTROY(root);
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
        
        ClassDef* s = NULL;
        Constant* let = NULL;
        
        
        CPPUNIT_ASSERT(s = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(4, s->numDeclarations());
        
        CPPUNIT_ASSERT(let = dynamic_cast<Constant*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT(let->getSpecifiers() && TypeSpecifier::Unowned);

        DESTROY(root);
    }
    
    void testNestedType_Struct()
    {
        PARSE_STATEMENT(L"class Foo\n"
                        L"{\n"
                        L"  struct Bar\n"
                        L"  {\n"
                        L"  }\n"
                        L"}");
        ClassDef* c = NULL;
        StructDef* s = NULL;
        
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(c->getDeclaration(0)));
        
        DESTROY(root);
    }
    
    void testNestedType_Class()
    {
        PARSE_STATEMENT(L"struct Foo\n"
                        L"{\n"
                        L"  class Bar\n"
                        L"  {\n"
                        L"  }\n"
                        L"}");
        ClassDef* c = NULL;
        StructDef* s = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        CPPUNIT_ASSERT(c = dynamic_cast<ClassDef*>(s->getDeclaration(0)));
        
        DESTROY(root);
    }
    
    
    void testNestedType_Enum()
    {
        PARSE_STATEMENT(L"struct Foo\n"
                        L"{\n"
                        L"  enum Bar\n"
                        L"  {\n"
                        L"  }\n"
                        L"}");
        EnumDef* c = NULL;
        StructDef* s = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        CPPUNIT_ASSERT(c = dynamic_cast<EnumDef*>(s->getDeclaration(0)));
        
        DESTROY(root);
    }
    
    
    
    
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestClass, "alltest");



#endif