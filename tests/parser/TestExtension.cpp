#include "../utils.h"

using namespace Swift;

TEST(TestExtension, testEmptyExtension)
{
    PARSE_STATEMENT(L"extension SomeType {"
                    L"}");
    ExtensionDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
}
TEST(TestExtension, testExtensionProtocols)
{
    PARSE_STATEMENT(L"extension SomeType: SomeProtocol, AnotherProtocol {"
                    L"}");
    ExtensionDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
}
TEST(TestExtension, testComputedProperty)
{
    PARSE_STATEMENT(L"extension Double {\n"
                    L"var km: Double { return self * 1_000.0 }\n"
                    L"var m: Double { return self }\n"
                    L"var cm: Double { return self / 100.0 }\n"
                    L"var mm: Double { return self / 1_000.0 }\n"
                    L"var ft: Double { return self / 3.28084 }\n"
                    L"}");
    ExtensionDefPtr s;
    ComputedPropertyPtr var;
    CodeBlockPtr cb;
    ReturnStatementPtr ret;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
    ASSERT_EQ(5, s->numDeclarations());

    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(0)));
    ASSERT_EQ(L"km", var->getName());
    ASSERT_EQ(L"Double", std::dynamic_pointer_cast<TypeIdentifier>(var->getDeclaredType())->getName());
    ASSERT_NULL(var->getInitializer());
    ASSERT_NOT_NULL(cb = var->getGetter());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));


    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
    ASSERT_EQ(L"m", var->getName());
    ASSERT_EQ(L"Double", std::dynamic_pointer_cast<TypeIdentifier>(var->getDeclaredType())->getName());
    ASSERT_NULL(var->getInitializer());
    ASSERT_NOT_NULL(cb = var->getGetter());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));


}


TEST(TestExtension, testMethod)
{
    PARSE_STATEMENT(L"extension Int {\n"
                    L"func repetitions(task: () -> ()) {\n"
                    L"    for i in 0..self {\n"
                    L"        task()\n"
                    L"    }\n"
                    L"}\n"
                    L"}");
    ExtensionDefPtr e;
    FunctionDefPtr f;
    ParameterPtr param;

    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<ExtensionDef>(root));
    ASSERT_EQ(L"Int", std::dynamic_pointer_cast<TypeIdentifier>(e->getIdentifier())->getName());
    ASSERT_EQ(1, e->numDeclarations());

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(e->getDeclaration(0)));
    ASSERT_EQ(L"repetitions", f->getName());

    ASSERT_EQ(1, f->numParameters());
    ASSERT_EQ(1, f->getParameters(0)->numParameters());
    ASSERT_NOT_NULL(param = f->getParameters(0)->getParameter(0));
    ASSERT_EQ(L"task", param->getLocalName());
    FunctionTypePtr type;
    TupleTypePtr tuple;
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<FunctionType>(param->getDeclaredType()));
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<TupleType>(type->getArgumentsType()));
    ASSERT_EQ(0, tuple->numElements());

    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<TupleType>(type->getReturnType()));
    ASSERT_EQ(0, tuple->numElements());

}


TEST(TestExtension, testInit)
{
    PARSE_STATEMENT(L"extension Rect {\n"
                    L"init(center: Point, size: Size) {\n"
                    L"    let originX = center.x - (size.width / 2)\n"
                    L"    let originY = center.y - (size.height / 2)\n"
                    L"    self.init(origin: Point(x: originX, y: originY), size: size)\n"
                    L"}\n"
                    L"}");

    ExtensionDefPtr s;
    InitializerDefPtr init;
    ParametersPtr params;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
    ASSERT_EQ(1, s->numDeclarations());

    ASSERT_NOT_NULL(init = std::dynamic_pointer_cast<InitializerDef>(s->getDeclaration(0)));
    ASSERT_FALSE(init->isConvenience());
    ASSERT_NOT_NULL(params = std::dynamic_pointer_cast<Parameters>(init->getParameters()));
    ASSERT_EQ(2, params->numParameters());


}

TEST(TestExtension, testMutatingMethod)
{
    PARSE_STATEMENT(L"extension Int {\n"
                    L"mutating func square() {\n"
                    L"    self = self * self\n"
                    L"}\n"
                    L"}");

    ExtensionDefPtr s;
    FunctionDefPtr square;
    ParametersPtr params;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
    ASSERT_EQ(1, s->numDeclarations());

    ASSERT_NOT_NULL(square = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Mutating, square->getSpecifiers());
    ASSERT_NOT_NULL(params = std::dynamic_pointer_cast<Parameters>(square->getParameters(0)));
    ASSERT_EQ(0, params->numParameters());


}

TEST(TestExtension, testSubscript)
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

    ExtensionDefPtr s;
    SubscriptDefPtr subscript;
    ParametersPtr params;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
    ASSERT_EQ(1, s->numDeclarations());

    ASSERT_NOT_NULL(subscript = std::dynamic_pointer_cast<SubscriptDef>(s->getDeclaration(0)));
    ASSERT_NOT_NULL(params = std::dynamic_pointer_cast<Parameters>(subscript->getParameters()));
    ASSERT_EQ(1, params->numParameters());
}
TEST(TestExtension, testNestedType)
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

    ExtensionDefPtr s;
    EnumDefPtr e;
    ComputedPropertyPtr kind;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ExtensionDef>(root));
    ASSERT_EQ(2, s->numDeclarations());

    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(s->getDeclaration(0)));
    ASSERT_EQ(L"Kind", std::dynamic_pointer_cast<TypeIdentifier>(e->getIdentifier())->getName());

    ASSERT_NOT_NULL(kind = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
    ASSERT_EQ(L"kind", kind->getName());


}



