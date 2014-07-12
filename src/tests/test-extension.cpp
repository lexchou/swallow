#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"

using namespace Swift;

class TestExtension : public SwiftTestCase
{
    
    CPPUNIT_TEST_SUITE(TestExtension);
    CPPUNIT_TEST(testEmptyExtension);
    CPPUNIT_TEST(testExtensionProtocols);
    CPPUNIT_TEST(testComputedProperty);
    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testMutatingMethod);
    CPPUNIT_TEST(testSubscript);
    CPPUNIT_TEST(testNestedType);
    /*
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
     */
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testEmptyExtension()
    {
        PARSE_STATEMENT(L"extension SomeType {"
                        L"}");
        ExtensionDef* s = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
    }
    void testExtensionProtocols()
    {
        PARSE_STATEMENT(L"extension SomeType: SomeProtocol, AnotherProtocol {"
                        L"}");
        ExtensionDef* s = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numParents());
    }
    void testComputedProperty()
    {
        PARSE_STATEMENT(L"extension Double {\n"
                        L"var km: Double { return self * 1_000.0 }\n"
                        L"var m: Double { return self }\n"
                        L"var cm: Double { return self / 100.0 }\n"
                        L"var mm: Double { return self / 1_000.0 }\n"
                        L"var ft: Double { return self / 3.28084 }\n"
                        L"}");
        ExtensionDef* s = NULL;
        Variables* vars = NULL;
        Variable* var = NULL;
        CodeBlock* cb = NULL;
        ReturnStatement* ret = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
        CPPUNIT_ASSERT_EQUAL(5, s->numDeclarations());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"km", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        ASSERT_EQUALS(L"Double", dynamic_cast<TypeIdentifier*>(var->getType())->getName());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(cb->getStatement(0)));
        
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = vars->getVariable(0));
        ASSERT_EQUALS(L"m", dynamic_cast<Identifier*>(var->getName())->getIdentifier().c_str());
        ASSERT_EQUALS(L"Double", dynamic_cast<TypeIdentifier*>(var->getType())->getName());
        CPPUNIT_ASSERT(NULL == var->getInitializer());
        CPPUNIT_ASSERT(cb = var->getGetter());
        CPPUNIT_ASSERT_EQUAL(1, cb->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(cb->getStatement(0)));

        
    }
    
    
    void testMethod()
    {
        PARSE_STATEMENT(L"extension Int {\n"
                        L"func repetitions(task: () -> ()) {\n"
                        L"    for i in 0..self {\n"
                        L"        task()\n"
                        L"    }\n"
                        L"}\n"
                        L"}");
        ExtensionDef* e = NULL;
        FunctionDef* f = NULL;
        Parameter* param = NULL;
        
        CPPUNIT_ASSERT(e = dynamic_cast<ExtensionDef*>(root));
        ASSERT_EQUALS(L"Int", dynamic_cast<TypeIdentifier*>(e->getIdentifier())->getName());
        CPPUNIT_ASSERT_EQUAL(1, e->numDeclarations());
        
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(e->getDeclaration(0)));
        ASSERT_EQUALS(L"repetitions", f->getName());
        
        CPPUNIT_ASSERT_EQUAL(1, f->numParameters());
        CPPUNIT_ASSERT_EQUAL(1, f->getParameters(0)->numParameters());
        CPPUNIT_ASSERT(param = f->getParameters(0)->getParameter(0));
        ASSERT_EQUALS(L"task", param->getLocalName());
        FunctionType* type = NULL;
        TupleType* tuple = NULL;
        CPPUNIT_ASSERT(type = dynamic_cast<FunctionType*>(param->getType()));
        CPPUNIT_ASSERT(tuple = dynamic_cast<TupleType*>(type->getArgumentsType()));
        CPPUNIT_ASSERT_EQUAL(0, tuple->numElements());
        
        CPPUNIT_ASSERT(tuple = dynamic_cast<TupleType*>(type->getReturnType()));
        CPPUNIT_ASSERT_EQUAL(0, tuple->numElements());
        
    }
    
    
    void testInit()
    {
        PARSE_STATEMENT(L"extension Rect {\n"
                        L"init(center: Point, size: Size) {\n"
                        L"    let originX = center.x - (size.width / 2)\n"
                        L"    let originY = center.y - (size.height / 2)\n"
                        L"    self.init(origin: Point(x: originX, y: originY), size: size)\n"
                        L"}\n"
                        L"}");
        
        ExtensionDef* s = NULL;
        InitializerDef* init = NULL;
        Parameters* params = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        CPPUNIT_ASSERT(init = dynamic_cast<InitializerDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT(!init->isConvenience());
        CPPUNIT_ASSERT(params = dynamic_cast<Parameters*>(init->getParameters()));
        CPPUNIT_ASSERT_EQUAL(2, params->numParameters());
        
        
    }
    
    void testMutatingMethod()
    {
        PARSE_STATEMENT(L"extension Int {\n"
                        L"mutating func square() {\n"
                        L"    self = self * self\n"
                        L"}\n"
                        L"}");
        
        ExtensionDef* s = NULL;
        FunctionDef* square = NULL;
        Parameters* params = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        CPPUNIT_ASSERT(square = dynamic_cast<FunctionDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT_EQUAL((int)TypeSpecifier::Mutating, square->getSpecifiers());
        CPPUNIT_ASSERT(params = dynamic_cast<Parameters*>(square->getParameters(0)));
        CPPUNIT_ASSERT_EQUAL(0, params->numParameters());
        
        
    }
    
    void testSubscript()
    {
        PARSE_STATEMENT(L"extension Int {\n"
                        L"  subscript(digitIndex: Int) -> Int {\n"
                        L"    var decimalBase = 1\n"
                        L"    for _ in 1...digitIndex {\n"
                        L"        decimalBase *= 10\n"
                        L"    }\n"
                        L"    return (self / decimalBase) % 10\n"
                        L"  }\n"
                        L"}");
     
        ExtensionDef* s = NULL;
        SubscriptDef* subscript = NULL;
        Parameters* params = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
        CPPUNIT_ASSERT_EQUAL(1, s->numDeclarations());
        
        CPPUNIT_ASSERT(subscript = dynamic_cast<SubscriptDef*>(s->getDeclaration(0)));
        CPPUNIT_ASSERT(params = dynamic_cast<Parameters*>(subscript->getParameters()));
        CPPUNIT_ASSERT_EQUAL(1, params->numParameters());
    }
    void testNestedType()
    {
        PARSE_STATEMENT(L"extension Character {\n"
                        L"  enum Kind {\n"
                        L"      case Vowel, Consonant, Other\n"
                        L"  }\n"
                        L"  var kind: Kind {\n"
                        L"    switch String(self).lowercaseString {\n"
                        L"    case \"a\", \"e\", \"i\", \"o\", \"u\":\n"
                        L"        return .Vowel\n"
                        L"    case \"b\", \"c\", \"d\", \"f\", \"g\", \"h\", \"j\", \"k\", \"l\", \"m\",\n"
                        L"        \"n\", \"p\", \"q\", \"r\", \"s\", \"t\", \"v\", \"w\", \"x\", \"y\", \"z\":\n"
                        L"        return .Consonant\n"
                        L"    default:\n"
                        L"        return .Other\n"
                        L"    }\n"
                        L"  }\n"
                        L"}");
        
        ExtensionDef* s = NULL;
        EnumDef* e = NULL;
        Variables* vars = NULL;
        Variable* kind = NULL;
        
        CPPUNIT_ASSERT(s = dynamic_cast<ExtensionDef*>(root));
        CPPUNIT_ASSERT_EQUAL(2, s->numDeclarations());
        
        CPPUNIT_ASSERT(e = dynamic_cast<EnumDef*>(s->getDeclaration(0)));
        ASSERT_EQUALS(L"Kind", dynamic_cast<TypeIdentifier*>(e->getIdentifier())->getName());
        
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(s->getDeclaration(1)));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(kind = vars->getVariable(0));
        ASSERT_EQUALS(L"kind", dynamic_cast<Identifier*>(kind->getName())->getIdentifier());
	
        
    }
    
    
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestExtension, "alltest");



#endif
