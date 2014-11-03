/* TestProtocol.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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

TEST(TestProtocol, testEmptyProtocol)
{
    PARSE_STATEMENT(L"protocol SomeProtocol {\n"
                    L"// protocol definition goes here\n"
                    L"}");
    ProtocolDefPtr p;
    TypeIdentifierPtr id;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<TypeIdentifier>(p->getIdentifier()));
    ASSERT_EQ(L"SomeProtocol", id->getName());
}

TEST(TestProtocol, testStruct)
{
    PARSE_STATEMENT(L"struct SomeStructure: FirstProtocol, AnotherProtocol {\n"
                    L"// structure definition goes here\n"
                    L"}");
    StructDefPtr c;

    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, c->numParents());
}

TEST(TestProtocol, testClass)
{
    PARSE_STATEMENT(L"class SomeClass: SomeSuperclass, FirstProtocol, AnotherProtocol {\n"
                    L"// class definition goes here\n"
                    L"}");
    ClassDefPtr c;

    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(3, c->numParents());
}

TEST(TestProtocol, testPropertyRequirements)
{
    PARSE_STATEMENT(L"protocol SomeProtocol {\n"
                    L"var mustBeSettable: Int { get set }\n"
                    L"var doesNotNeedToBeSettable: Int { get }\n"
                    L"}");
    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(2, p->numDeclarations());
    ComputedPropertyPtr var;

    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(p->getDeclaration(0)));
    ASSERT_EQ(L"mustBeSettable", var->getName());
    ASSERT_EQ(0, var->getSpecifiers());
    ASSERT_NOT_NULL(var->getGetter());
    ASSERT_NOT_NULL(var->getSetter());



    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(p->getDeclaration(1)));
    ASSERT_EQ(L"doesNotNeedToBeSettable", var->getName());
    ASSERT_NOT_NULL(var->getGetter());
    ASSERT_NULL(var->getSetter());

}

TEST(TestProtocol, testPropertyRequirements2)
{
    PARSE_STATEMENT(L"protocol AnotherProtocol {\n"
                    L"class var someTypeProperty: Int { get set }\n"
                    L"}");
    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, p->numDeclarations());
    ComputedPropertyPtr var;

    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(p->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Class, var->getSpecifiers());
    ASSERT_EQ(L"someTypeProperty", var->getName());
    ASSERT_NOT_NULL(var->getGetter());
    ASSERT_NOT_NULL(var->getSetter());

}
TEST(TestProtocol, testPropertyRequirements3)
{
    PARSE_STATEMENT(L"protocol FullyNamed {\n"
                    L"var fullName: String { get }\n"
                    L"}");
    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, p->numDeclarations());
    ComputedPropertyPtr var;

    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(p->getDeclaration(0)));
    ASSERT_EQ(0, var->getSpecifiers());
    ASSERT_EQ(L"fullName", var->getName());
    ASSERT_NOT_NULL(var->getGetter());
    ASSERT_NULL(var->getSetter());

}

TEST(TestProtocol, testMethodRequirements)
{
    PARSE_STATEMENT(L"protocol SomeProtocol {\n"
                    L"class func someTypeMethod()\n"
                    L"}");

    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, p->numDeclarations());
    FunctionDefPtr f;

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Class, f->getSpecifiers());
    ASSERT_EQ(L"someTypeMethod", f->getName());
    //ASSERT_NULL(f->getBody());
}

TEST(TestProtocol, testMethodRequirements2)
{
    PARSE_STATEMENT(L"protocol RandomNumberGenerator {\n"
                    L"func random() -> Double\n"
                    L"}");

    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, p->numDeclarations());
    FunctionDefPtr f;

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
    ASSERT_EQ(0, f->getSpecifiers());
    ASSERT_EQ(L"random", f->getName());
    //ASSERT_NULL(f->getBody());
}



TEST(TestProtocol, testMutatingMethod)
{
    PARSE_STATEMENT(L"protocol Togglable {\n"
                    L"mutating func toggle()\n"
                    L"}");

    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, p->numDeclarations());
    FunctionDefPtr f;

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Mutating, f->getSpecifiers());
    ASSERT_EQ(L"toggle", f->getName());
    //ASSERT_NULL(f->getBody());
}

TEST(TestProtocol, testInheritance)
{
    PARSE_STATEMENT(L"protocol PrettyTextRepresentable: TextRepresentable {\n"
                    L"func asPrettyText() -> String\n"
                    L"}");

    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(1, p->numDeclarations());
    ASSERT_EQ(1, p->numParents());
    ASSERT_EQ(L"TextRepresentable", p->getParent(0)->getName());


    FunctionDefPtr f;

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
    ASSERT_EQ(0, f->getSpecifiers());
    ASSERT_EQ(L"asPrettyText", f->getName());
    //ASSERT_NULL(f->getBody());


}
TEST(TestProtocol, testOptional)
{
    PARSE_STATEMENT(L"@objc protocol CounterDataSource {\n"
                    L"@optional func incrementForCount(count: Int) -> Int\n"
                    L"@optional var fixedIncrement: Int { get }\n"
                    L"}");
    ProtocolDefPtr p;
    ASSERT_NOT_NULL(p = std::dynamic_pointer_cast<ProtocolDef>(root));
    ASSERT_EQ(2, p->numDeclarations());
    ASSERT_NOT_NULL(p->getAttribute(L"objc"));



    FunctionDefPtr f;

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(p->getDeclaration(0)));
    ASSERT_EQ(0, f->getSpecifiers());
    ASSERT_EQ(L"incrementForCount", f->getName());
    //ASSERT_NULL(f->getBody());
    ASSERT_NOT_NULL(f->getAttribute(L"optional"));

    ComputedPropertyPtr var;

    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(p->getDeclaration(1)));
    ASSERT_NOT_NULL(var->getAttribute(L"optional"));
    ASSERT_EQ(0, var->getSpecifiers());
    ASSERT_EQ(L"fixedIncrement", var->getName());
    ASSERT_NOT_NULL(var->getGetter());
    ASSERT_NULL(var->getSetter());

}

    
