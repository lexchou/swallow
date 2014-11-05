/* TestDeclaration.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "../utils.h"

using namespace Swallow;

TEST(TestDeclaration, testImport)
{
    PARSE_STATEMENT(L"import Foundation");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation", import->getPath());
    ASSERT_EQ(Import::_, import->getKind());

}
TEST(TestDeclaration, testImportSubModule)
{
    PARSE_STATEMENT(L"import Foundation.SubModule");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.SubModule", import->getPath());
    ASSERT_EQ(Import::_, import->getKind());
}

TEST(TestDeclaration, testImportKind_Class)
{
    PARSE_STATEMENT(L"import class Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Class, import->getKind());
}

TEST(TestDeclaration, testImportKind_TypeAlias)
{
    PARSE_STATEMENT(L"import typealias Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Typealias, import->getKind());
}

TEST(TestDeclaration, testImportKind_Struct)
{
    PARSE_STATEMENT(L"import struct Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Struct, import->getKind());
}

TEST(TestDeclaration, testImportKind_Enum)
{
    PARSE_STATEMENT(L"import enum Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Enum, import->getKind());
}

TEST(TestDeclaration, testImportKind_Protocol)
{
    PARSE_STATEMENT(L"import protocol Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Protocol, import->getKind());
}

TEST(TestDeclaration, testImportKind_Var)
{
    PARSE_STATEMENT(L"import var Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Var, import->getKind());
}

TEST(TestDeclaration, testImportKind_Func)
{
    PARSE_STATEMENT(L"import func Foundation.NSFileManager");
    ImportPtr import;
    ASSERT_NOT_NULL(import = std::dynamic_pointer_cast<Import>(root));
    ASSERT_EQ(L"Foundation.NSFileManager", import->getPath());
    ASSERT_EQ(Import::Func, import->getKind());
}

TEST(TestDeclaration, testLet)
{
    PARSE_STATEMENT(L"let a : Int[] = [1, 2, 3]");
    ValueBindingsPtr c;
    IdentifierPtr id;
    ValueBindingPtr a;
    ArrayLiteralPtr value;
    ArrayTypePtr type;
    TypeIdentifierPtr Int;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_TRUE(c->isReadOnly());
    ASSERT_EQ(1, c->numBindings());
    ASSERT_NOT_NULL(a = c->get(0));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(a->getName()));
    ASSERT_EQ(L"a", id->getIdentifier());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<ArrayType>(a->getDeclaredType()));
    ASSERT_NOT_NULL(Int = std::dynamic_pointer_cast<TypeIdentifier>(type->getInnerType()));
    ASSERT_EQ(L"Int", Int->getName());

    ASSERT_NOT_NULL(value = std::dynamic_pointer_cast<ArrayLiteral>(c->get(0)->getInitializer()));
    ASSERT_EQ(3, value->numElements());
    ASSERT_EQ(L"1", std::dynamic_pointer_cast<IntegerLiteral>(value->getElement(0))->valueAsString);
    ASSERT_EQ(L"2", std::dynamic_pointer_cast<IntegerLiteral>(value->getElement(1))->valueAsString);
    ASSERT_EQ(L"3", std::dynamic_pointer_cast<IntegerLiteral>(value->getElement(2))->valueAsString);

}

TEST(TestDeclaration, testLet_Multiple)
{
    PARSE_STATEMENT(L"let a=[k1 : 1, k2 : 2], b : Int = 2");
    ValueBindingsPtr c;
    ValueBindingPtr b;
    IdentifierPtr id;
    TypeIdentifierPtr Int;
    DictionaryLiteralPtr dict;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_TRUE(c->isReadOnly());
    ASSERT_EQ(2, c->numBindings());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(c->get(0)->getName()));
    ASSERT_EQ(L"a", id->getIdentifier());
    ASSERT_NOT_NULL(dict = std::dynamic_pointer_cast<DictionaryLiteral>(c->get(0)->getInitializer()));
    ASSERT_EQ(2, dict->numElements());

    ASSERT_NOT_NULL(b = c->get(1));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(b->getName()));
    ASSERT_EQ(L"b", id->getIdentifier());
    ASSERT_NOT_NULL(Int = std::dynamic_pointer_cast<TypeIdentifier>(b->getDeclaredType()));
    ASSERT_EQ(L"Int", Int->getName());

}

TEST(TestDeclaration, testLet_Tuple)
{
    PARSE_STATEMENT(L"let (a, b) : Int = (1, 2)");
    ValueBindingsPtr c;
    TuplePtr tuple;
    TypeIdentifierPtr type;
    ParenthesizedExpressionPtr p;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_TRUE(c->isReadOnly());
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<Tuple>(c->get(0)->getName()));
    ASSERT_EQ(2, tuple->numElements());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(c->get(0)->getDeclaredType()));
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ParenthesizedExpression>(c->get(0)->getInitializer()));
    ASSERT_EQ(2, p->numExpressions());

}

TEST(TestDeclaration, testVar)
{
    PARSE_STATEMENT(L"var currentLoginAttempt = 0");
    ValueBindingsPtr vars;
    ValueBindingPtr var;
    IdentifierPtr id;
    IntegerLiteralPtr i;

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_TRUE(!vars->isReadOnly());
    ASSERT_EQ(1, vars->numBindings());
    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ValueBinding>(vars->get(0)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
    ASSERT_EQ(L"currentLoginAttempt", id->getIdentifier());

    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
    ASSERT_EQ(L"0", i->valueAsString);


}
TEST(TestDeclaration, testVar_Multiple)
{
    PARSE_STATEMENT(L"var x = 0.0, y = 0.0, z = 0.0");
    ValueBindingsPtr vars;
    ValueBindingPtr var;
    IdentifierPtr id;
    FloatLiteralPtr f;

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_TRUE(!vars->isReadOnly());
    ASSERT_EQ(3, vars->numBindings());

    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ValueBinding>(vars->get(0)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
    ASSERT_EQ(L"x", id->getIdentifier());
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FloatLiteral>(var->getInitializer()));
    ASSERT_EQ(L"0.0", f->valueAsString);


    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ValueBinding>(vars->get(1)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
    ASSERT_EQ(L"y", id->getIdentifier());
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FloatLiteral>(var->getInitializer()));
    ASSERT_EQ(L"0.0", f->valueAsString);


    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ValueBinding>(vars->get(2)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
    ASSERT_EQ(L"z", id->getIdentifier());
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FloatLiteral>(var->getInitializer()));
    ASSERT_EQ(L"0.0", f->valueAsString);


}

TEST(TestDeclaration, testVar_Typed)
{
    PARSE_STATEMENT(L"var welcomeMessage: String");
    ValueBindingsPtr vars;
    ValueBindingPtr var;
    IdentifierPtr id;
    TypeIdentifierPtr t;

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(root));
    ASSERT_TRUE(!vars->isReadOnly());
    ASSERT_EQ(1, vars->numBindings());
    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ValueBinding>(vars->get(0)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(var->getName()));
    ASSERT_EQ(L"welcomeMessage", id->getIdentifier());

    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(var->getDeclaredType()));
    ASSERT_EQ(L"String", t->getName());


}

TEST(TestDeclaration, testOperator)
{
    PARSE_STATEMENT(L"operator infix +- { associativity left precedence 140 }");
    OperatorDefPtr o;
    ASSERT_NOT_NULL(o = std::dynamic_pointer_cast<OperatorDef>(root));
    ASSERT_EQ(L"+-", o->getName());
    ASSERT_EQ(Associativity::Left, o->getAssociativity());
    ASSERT_EQ(140, o->getPrecedence());


}

TEST(TestDeclaration, TypeAlias)
{
    PARSE_STATEMENT(L"typealias NewType = Int");
    ASSERT_EQ(0, compilerResults.numResults());
    TypeAliasPtr typealias;
    TypeIdentifierPtr Int;
    ASSERT_NOT_NULL(typealias = std::dynamic_pointer_cast<TypeAlias>(root));
    ASSERT_EQ(L"NewType", typealias->getName());
    ASSERT_NOT_NULL(Int = std::dynamic_pointer_cast<TypeIdentifier>(typealias->getType()));
    ASSERT_EQ(L"Int", Int->getName());
}

TEST(TestDeclaration, TypeAlias_Protocol)
{
    PARSE_STATEMENT(L"protocol MyProtocol { typealias NewType = Int }");
    ASSERT_EQ(0, compilerResults.numResults());
    ProtocolDefPtr protocol;
    TypeAliasPtr typealias;
    TypeIdentifierPtr Int;
    ASSERT_NOT_NULL(protocol = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, protocol->numDeclarations());

    ASSERT_NOT_NULL(typealias = std::dynamic_pointer_cast<TypeAlias>(protocol->getDeclaration(0)));
    ASSERT_EQ(L"NewType", typealias->getName());
    ASSERT_NOT_NULL(Int = std::dynamic_pointer_cast<TypeIdentifier>(typealias->getType()));
    ASSERT_EQ(L"Int", Int->getName());
}

TEST(TestDeclaration, TypeAlias_ProtocolNoType)
{
    PARSE_STATEMENT(L"protocol MyProtocol { typealias NewType }");
    ASSERT_EQ(0, compilerResults.numResults());
    ProtocolDefPtr protocol;
    TypeAliasPtr typealias;
    TypeIdentifierPtr Int;
    ASSERT_NOT_NULL(protocol = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, protocol->numDeclarations());

    ASSERT_NOT_NULL(typealias = std::dynamic_pointer_cast<TypeAlias>(protocol->getDeclaration(0)));
    ASSERT_EQ(L"NewType", typealias->getName());
    ASSERT_NULL(typealias->getType());

}


TEST(TestDeclaration, TypeAlias_NoType)
{
    PARSE_STATEMENT(L"typealias NewType");
    ASSERT_EQ(1, compilerResults.numResults());
}


