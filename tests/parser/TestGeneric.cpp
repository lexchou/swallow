#include "../utils.h"

using namespace Swift;

TEST(TestGeneric, testFunc)
{
    PARSE_STATEMENT(L"func swapTwoValues<T>(inout a: T, inout b: T) {\n"
                    L"let temporaryA = a\n"
                    L"a = b\n"
                    L"b = temporaryA\n"
                    L"}");
    FunctionDefPtr f;
    GenericParametersDefPtr gp;
    TypeIdentifierPtr type;
    ParametersPtr params;
    ParameterPtr param;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_NOT_NULL(gp = std::dynamic_pointer_cast<GenericParametersDef>(f->getGenericParametersDef()));
    ASSERT_EQ(1, gp->numGenericTypes());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(gp->getGenericType(0)));
    ASSERT_EQ(L"T", type->getName());

    ASSERT_EQ(1, f->numParameters());
    ASSERT_NOT_NULL(params = f->getParameters(0));
    ASSERT_EQ(2, params->numParameters());

    ASSERT_NOT_NULL(param = params->getParameter(0));
    ASSERT_TRUE(param->isInout());
    ASSERT_EQ(L"a", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"T", type->getName());

    ASSERT_NOT_NULL(param = params->getParameter(1));
    ASSERT_TRUE(param->isInout());
    ASSERT_EQ(L"b", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"T", type->getName());

}
TEST(TestGeneric, testStruct)
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
    StructDefPtr s;
    GenericParametersDefPtr gp;
    TypeIdentifierPtr type;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_NOT_NULL(gp = std::dynamic_pointer_cast<GenericParametersDef>(s->getGenericParametersDef()));
    ASSERT_EQ(1, gp->numGenericTypes());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(gp->getGenericType(0)));
    ASSERT_EQ(L"T", type->getName());

}

TEST(TestGeneric, testVar)
{
    PARSE_STATEMENT(L"var stackOfStrings = Stack<String>()");
    ValueBindingsPtr vars;
    ValueBindingPtr var;
    FunctionCallPtr call;
    IdentifierPtr id;
    GenericArgumentPtr arg;
    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_EQ(1, vars->numBindings());
    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ValueBinding>(vars->get(0)));
    ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
    ASSERT_NOT_NULL(arg = id->getGenericArgument());
}

TEST(TestGeneric, testTypeConstraint)
{
    PARSE_STATEMENT(L"func someFunction<T: SomeClass, U: SomeProtocol>(someT: T, someU: U) {\n"
                    L"}");
    FunctionDefPtr f;
    GenericParametersDefPtr params;
    TypeIdentifierPtr type;
    GenericConstraintDefPtr constraint;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_NOT_NULL(params = f->getGenericParametersDef());
    ASSERT_EQ(2, params->numGenericTypes());
    ASSERT_EQ(2, params->numConstraints());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(params->getGenericType(0)));
    ASSERT_EQ(L"T", type->getName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(params->getGenericType(1)));
    ASSERT_EQ(L"U", type->getName());

    ASSERT_NOT_NULL(constraint = params->getConstraint(0));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::DerivedFrom, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"T", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"SomeClass", type->getName());

    ASSERT_NOT_NULL(constraint = params->getConstraint(1));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::DerivedFrom, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"U", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"SomeProtocol", type->getName());

}

TEST(TestGeneric, testTypeConstraint2)
{
    PARSE_STATEMENT(L"func findIndex<T: Equatable>(array: T[], valueToFind: T) -> Int? {\n"
                    L"for (index, value) in enumerate(array) {\n"
                    L"    if value == valueToFind {\n"
                    L"        return index\n"
                    L"    }\n"
                    L"}\n"
                    L"return nil\n"
                    L"}");


    FunctionDefPtr f;
    GenericParametersDefPtr params;
    TypeIdentifierPtr type;
    GenericConstraintDefPtr constraint;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_NOT_NULL(params = f->getGenericParametersDef());
    ASSERT_EQ(1, params->numGenericTypes());
    ASSERT_EQ(1, params->numConstraints());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(params->getGenericType(0)));
    ASSERT_EQ(L"T", type->getName());


    ASSERT_NOT_NULL(constraint = params->getConstraint(0));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::DerivedFrom, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"T", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"Equatable", type->getName());
}

TEST(TestGeneric, testWhereClause)
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

    FunctionDefPtr f;
    GenericParametersDefPtr params;
    TypeIdentifierPtr type;
    GenericConstraintDefPtr constraint;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(root));
    ASSERT_NOT_NULL(params = f->getGenericParametersDef());
    ASSERT_EQ(2, params->numGenericTypes());
    ASSERT_EQ(4, params->numConstraints());

    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(params->getGenericType(0)));
    ASSERT_EQ(L"C1", type->getName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(params->getGenericType(1)));
    ASSERT_EQ(L"C2", type->getName());

    ASSERT_NOT_NULL(constraint = params->getConstraint(0));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::DerivedFrom, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"C1", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"Container", type->getName());

    ASSERT_NOT_NULL(constraint = params->getConstraint(1));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::DerivedFrom, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"C2", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"Container", type->getName());

    ASSERT_NOT_NULL(constraint = params->getConstraint(2));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::EqualsTo, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"C1", type->getName());
    ASSERT_NOT_NULL(type = type->getNestedType());
    ASSERT_EQ(L"ItemType", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"C2", type->getName());
    ASSERT_NOT_NULL(type = type->getNestedType());
    ASSERT_EQ(L"ItemType", type->getName());

    ASSERT_NOT_NULL(constraint = params->getConstraint(3));
    ASSERT_EQ(1, constraint->numExpectedTypes());
    ASSERT_EQ(GenericConstraintDef::DerivedFrom, constraint->getConstraintType());
    ASSERT_NOT_NULL(type = constraint->getIdentifier());
    ASSERT_EQ(L"C1", type->getName());
    ASSERT_NOT_NULL(type = type->getNestedType());
    ASSERT_EQ(L"ItemType", type->getName());
    ASSERT_NOT_NULL(type = constraint->getExpectedType(0));
    ASSERT_EQ(L"Equatable", type->getName());

}
    


