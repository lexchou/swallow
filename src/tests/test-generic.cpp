#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"

using namespace Swift;

class TestGeneric : public SwiftTestCase
{
    
    CPPUNIT_TEST_SUITE(TestGeneric);
    CPPUNIT_TEST(testFunc);
    CPPUNIT_TEST(testStruct);
    CPPUNIT_TEST(testVar);
    CPPUNIT_TEST(testTypeConstraint);
    CPPUNIT_TEST(testTypeConstraint2);
    CPPUNIT_TEST(testWhereClause);
    CPPUNIT_TEST_SUITE_END();
public:
    void testFunc()
    {
        PARSE_STATEMENT(L"func swapTwoValues<T>(inout a: T, inout b: T) {\n"
                        L"let temporaryA = a\n"
                        L"a = b\n"
                        L"b = temporaryA\n"
                        L"}");
        FunctionDef* f = NULL;
        GenericParameters* gp = NULL;
        TypeIdentifier* type = NULL;
        Parameters* params = NULL;
        Parameter* param = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(root));
        CPPUNIT_ASSERT(gp = dynamic_cast<GenericParameters*>(f->getGenericParameters()));
        CPPUNIT_ASSERT_EQUAL(1, gp->numGenericTypes());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(gp->getGenericType(0)));
        ASSERT_EQUALS(L"T", type->getName());
        
        CPPUNIT_ASSERT_EQUAL(1, f->numParameters());
        CPPUNIT_ASSERT(params = f->getParameters(0));
        CPPUNIT_ASSERT_EQUAL(2, params->numParameters());
        
        CPPUNIT_ASSERT(param = params->getParameter(0));
        CPPUNIT_ASSERT(param->isInout());
        ASSERT_EQUALS(L"a", param->getLocalName());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"T", type->getName());
        
        CPPUNIT_ASSERT(param = params->getParameter(1));
        CPPUNIT_ASSERT(param->isInout());
        ASSERT_EQUALS(L"b", param->getLocalName());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"T", type->getName());
        
    }
    void testStruct()
    {
        PARSE_STATEMENT(L"struct Stack<T> {\n"
                        L"var items = T[]()\n"
                        L"mutating func push(item: T) {\n"
                        L"    items.append(item)\n"
                        L"}\n"
                        L"mutating func pop() -> T {\n"
                        L"    return items.removeLast()\n"
                        L"}\n"
                        L"}");
        StructDef* s = NULL;
        GenericParameters* gp = NULL;
        TypeIdentifier* type = NULL;
        CPPUNIT_ASSERT(s = dynamic_cast<StructDef*>(root));
        CPPUNIT_ASSERT(gp = dynamic_cast<GenericParameters*>(s->getGenericParameters()));
        CPPUNIT_ASSERT_EQUAL(1, gp->numGenericTypes());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(gp->getGenericType(0)));
        ASSERT_EQUALS(L"T", type->getName());
        
    }
    
    void testVar()
    {
        PARSE_STATEMENT(L"var stackOfStrings = Stack<String>()");
        Variables* vars = NULL;
        Variable* var;
        FunctionCall* call = NULL;
        Identifier* id = NULL;
        GenericArgument* arg = NULL;
        CPPUNIT_ASSERT(vars = dynamic_cast<Variables*>(root));
        CPPUNIT_ASSERT_EQUAL(1, vars->numVariables());
        CPPUNIT_ASSERT(var = dynamic_cast<Variable*>(vars->getVariable(0)));
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(var->getInitializer()));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(call->getFunction()));
        CPPUNIT_ASSERT(arg = id->getGenericArgument());
    }
    
    void testTypeConstraint()
    {
        PARSE_STATEMENT(L"func someFunction<T: SomeClass, U: SomeProtocol>(someT: T, someU: U) {\n"
                        L"}");
        FunctionDef* f = NULL;
        GenericParameters* params = NULL;
        TypeIdentifier* type = NULL;
        GenericConstraint* constraint = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(root));
        CPPUNIT_ASSERT(params = f->getGenericParameters());
        CPPUNIT_ASSERT_EQUAL(2, params->numGenericTypes());
        CPPUNIT_ASSERT_EQUAL(2, params->numConstraints());

        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(params->getGenericType(0)));
        ASSERT_EQUALS(L"T", type->getName());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(params->getGenericType(1)));
        ASSERT_EQUALS(L"U", type->getName());
        
        CPPUNIT_ASSERT(constraint = params->getConstraint(0));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::DerivedFrom, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"T", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"SomeClass", type->getName());
        
        CPPUNIT_ASSERT(constraint = params->getConstraint(1));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::DerivedFrom, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"U", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"SomeProtocol", type->getName());
        
    }
    
    void testTypeConstraint2()
    {
        PARSE_STATEMENT(L"func findIndex<T: Equatable>(array: T[], valueToFind: T) -> Int? {\n"
                        L"for (index, value) in enumerate(array) {\n"
                        L"    if value == valueToFind {\n"
                        L"        return index\n"
                        L"    }\n"
                        L"}\n"
                        L"return nil\n"
                        L"}");
        
        
        FunctionDef* f = NULL;
        GenericParameters* params = NULL;
        TypeIdentifier* type = NULL;
        GenericConstraint* constraint = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(root));
        CPPUNIT_ASSERT(params = f->getGenericParameters());
        CPPUNIT_ASSERT_EQUAL(1, params->numGenericTypes());
        CPPUNIT_ASSERT_EQUAL(1, params->numConstraints());
        
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(params->getGenericType(0)));
        ASSERT_EQUALS(L"T", type->getName());

        
        CPPUNIT_ASSERT(constraint = params->getConstraint(0));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::DerivedFrom, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"T", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"Equatable", type->getName());
    }
    
    void testWhereClause()
    {
        PARSE_STATEMENT(L"func allItemsMatch<C1: Container, C2: Container where C1.ItemType == C2.ItemType, C1.ItemType: Equatable>\n"
                        L"(someContainer: C1, anotherContainer: C2) -> Bool {\n"
                        L"  if someContainer.count != anotherContainer.count {\n"
                        L"      return false\n"
                        L"  }\n"
                        L"  for i in 0..someContainer.count {\n"
                        L"      if someContainer[i] != anotherContainer[i] {\n"
                        L"          return false\n"
                        L"      }\n"
                        L"  }\n"
                        L"  return true\n"
                        L"}");
        
        FunctionDef* f = NULL;
        GenericParameters* params = NULL;
        TypeIdentifier* type = NULL;
        GenericConstraint* constraint = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionDef*>(root));
        CPPUNIT_ASSERT(params = f->getGenericParameters());
        CPPUNIT_ASSERT_EQUAL(2, params->numGenericTypes());
        CPPUNIT_ASSERT_EQUAL(4, params->numConstraints());
        
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(params->getGenericType(0)));
        ASSERT_EQUALS(L"C1", type->getName());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(params->getGenericType(1)));
        ASSERT_EQUALS(L"C2", type->getName());
        
        CPPUNIT_ASSERT(constraint = params->getConstraint(0));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::DerivedFrom, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"C1", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"Container", type->getName());
        
        CPPUNIT_ASSERT(constraint = params->getConstraint(1));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::DerivedFrom, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"C2", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"Container", type->getName());
        
        CPPUNIT_ASSERT(constraint = params->getConstraint(2));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::EqualsTo, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"C1", type->getName());
        CPPUNIT_ASSERT(type = type->getNestedType());
        ASSERT_EQUALS(L"ItemType", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"C2", type->getName());
        CPPUNIT_ASSERT(type = type->getNestedType());
        ASSERT_EQUALS(L"ItemType", type->getName());
        
        CPPUNIT_ASSERT(constraint = params->getConstraint(3));
        CPPUNIT_ASSERT_EQUAL(1, constraint->numExpectedTypes());
        CPPUNIT_ASSERT_EQUAL(GenericConstraint::DerivedFrom, constraint->getConstraintType());
        CPPUNIT_ASSERT(type = constraint->getIdentifier());
        ASSERT_EQUALS(L"C1", type->getName());
        CPPUNIT_ASSERT(type = type->getNestedType());
        ASSERT_EQUALS(L"ItemType", type->getName());
        CPPUNIT_ASSERT(type = constraint->getExpectedType(0));
        ASSERT_EQUALS(L"Equatable", type->getName());
        
    }
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestGeneric, "alltest");



#endif