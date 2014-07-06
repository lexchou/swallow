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
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testStruct()
    {
        Node* root = parse(L"struct Resolution {"
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
        Node* root = parse(L"class VideoMode { "
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
        Node* root = parse(L"struct Rect {"
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
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numChildren());
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
        Node* root = parse(L"struct Rect {"
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
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numChildren());
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
        Node* root = parse(L"struct Rect {"
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
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numChildren());
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
        Node* root = parse(L"struct Rect {"
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
        CPPUNIT_ASSERT_EQUAL(0, call->getArguments()->numChildren());
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
        Node* root = parse(L"struct Cuboid { "
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
        Node* root = parse(L"class StepCounter { "
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
        Node* root = parse(L"class StepCounter { "
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
        Node* root = parse(L"class StepCounter { "
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
        Node* root = parse(L"struct SomeStructure {"
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
        Node* root = parse(L"class SomeStructure {"
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
        Node* root = parse(L"enum SomeStructure {"
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
        Node* root = parse(L"struct Point { "
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
        Node* root = parse(L"enum TriStateSwitch { "
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
        Node* root = parse(L"struct SomeClass {\n"
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
        Node* root = parse(L"enum SomeClass {\n"
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
        Node* root = parse(L"class SomeClass {\n"
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
        Node* root = parse(L"struct TimesTable {\n"
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
        Node* root = parse(
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
        Node* root = parse(L"class Bicycle: Vehicle {"
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
        InitializerDef* init = NULL;
        CPPUNIT_ASSERT_EQUAL(1, c->numDeclarations());
        CPPUNIT_ASSERT(init = dynamic_cast<InitializerDef*>(c->getDeclaration(0)));
        
        
        
        DESTROY(root);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestClass, "alltest");



#endif