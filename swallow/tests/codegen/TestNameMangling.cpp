/* TestNameMangling.cpp --
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
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GlobalScope.h"
#include "semantics/GenericArgument.h"
#include "codegen/NameMangling.h"

using namespace Swallow;
using namespace std;


TEST(TestNameMangling, PublicVar)
{
    SEMANTIC_ANALYZE(L"public var publicVar : Int = 3");
    ASSERT_NO_ERRORS();
    SymbolPtr sym = scope->lookup(L"publicVar");
    ASSERT_NOT_NULL(sym);
    wstring s = mangling.encode(sym);
    ASSERT_EQ(L"_Tv4main9publicVarSi", s);
}

TEST(TestNameMangling, PublicVar2)
{
    SEMANTIC_ANALYZE(L"public var publicVar2 : Int { return 3} //_TF4maing10publicVar2Si");
    ASSERT_NO_ERRORS();
    ComputedPropertySymbolPtr sym = dynamic_pointer_cast<ComputedPropertySymbol>(scope->lookup(L"publicVar2"));
    ASSERT_NOT_NULL(sym);
    ASSERT_NOT_NULL(sym->getGetter());
    wstring s = mangling.encode(sym->getGetter());
    ASSERT_EQ(L"_TF4maing10publicVar2Si", s);
}
TEST(TestNameMangling, InternalVar)
{
    SEMANTIC_ANALYZE(L"internal var internalVar : Int = 4  //_Tv4main11internalVarSi");
    ASSERT_NO_ERRORS();
    SymbolPtr sym = scope->lookup(L"internalVar");
    ASSERT_NOT_NULL(sym);
    wstring s = mangling.encode(sym);
    ASSERT_EQ(L"_Tv4main11internalVarSi", s);
}
TEST(TestNameMangling, InternalVar2)
{
    SEMANTIC_ANALYZE(L"internal var internalVar2 : Int { return 4}  //_TF4maing12internalVar2Si");
    ASSERT_NO_ERRORS();
    ComputedPropertySymbolPtr sym = dynamic_pointer_cast<ComputedPropertySymbol>(scope->lookup(L"internalVar2"));
    ASSERT_NOT_NULL(sym);
    ASSERT_NOT_NULL(sym->getGetter());
    wstring s = mangling.encode(sym->getGetter());
    ASSERT_EQ(L"_TF4maing12internalVar2Si", s);
}
 //
 //

TEST(TestNameMangling, PrivateVar)
{
    SEMANTIC_ANALYZE(L"private var privateVar : Int = 5");
    ASSERT_NO_ERRORS();
    SymbolPtr sym = scope->lookup(L"privateVar");
    ASSERT_NOT_NULL(sym);
    wstring s = mangling.encode(sym);
    ASSERT_EQ(L"_Tv4mainP33_fad58de7366495db4650cfefac2fcd6110privateVarSi", s);
}
TEST(TestNameMangling, PrivateVar2)
{
    SEMANTIC_ANALYZE(L"private var privateVar2 : Int { return 5}");
    ASSERT_NO_ERRORS();
    ComputedPropertySymbolPtr sym = dynamic_pointer_cast<ComputedPropertySymbol>(scope->lookup(L"privateVar2"));
    ASSERT_NOT_NULL(sym);
    ASSERT_NOT_NULL(sym->getGetter());
    wstring s = mangling.encode(sym->getGetter());
    ASSERT_EQ(L"_TF4maingP33_fad58de7366495db4650cfefac2fcd6111privateVar2Si", s);
}
TEST(TestNameMangling, PublicLet)
{
    SEMANTIC_ANALYZE(L"public let publicLet : Int = 3");
    ASSERT_NO_ERRORS();
    SymbolPtr sym = scope->lookup(L"publicLet");
    ASSERT_NOT_NULL(sym);
    wstring s = mangling.encode(sym);
    ASSERT_EQ(L"_Tv4main9publicLetSi", s);
}
TEST(TestNameMangling, InternalLet)
{
    SEMANTIC_ANALYZE(L"internal let internalLet : Int = 4");
    ASSERT_NO_ERRORS();
    SymbolPtr sym = scope->lookup(L"internalLet");
    ASSERT_NOT_NULL(sym);
    wstring s = mangling.encode(sym);
    ASSERT_EQ(L"_Tv4main11internalLetSi", s);
}
TEST(TestNameMangling, PrivateLet)
{
    SEMANTIC_ANALYZE(L"private let privateLet : Int = 5");
    ASSERT_NO_ERRORS();
    SymbolPtr sym = scope->lookup(L"privateLet");
    ASSERT_NOT_NULL(sym);
    wstring s = mangling.encode(sym);
    ASSERT_EQ(L"_Tv4mainP33_fad58de7366495db4650cfefac2fcd6110privateLetSi", s);
}

TEST(TestNameMangling, Struct)
{
    SEMANTIC_ANALYZE(L"struct STRUCT\n"
            L"{\n"
            L"    var a : Int = 3\n"
            L"    let b : Int = 4\n"
            L"    var c : Int { return 5} // getter: _TFV4main6STRUCTg1cSi\n"
            L"    static var d : Int = 6 // _TvV4main6STRUCT1dSi (mutable addressor: _TFV4main6STRUCTa1dSi)\n"
            L"    static let e : Int = 7 //_TvV4main6STRUCT1eSi  (mutable addressor: _TFV4main6STRUCTa1eSi)\n"
            L"    var f : Int { get { return 8} set{}} // getter: _TFV4main6STRUCTg1fSi setter:_TFV4main6STRUCTs1fSi\n"
            L"    var g : Int = 9 {  // getter: _TFV4main6STRUCTg1gSi, setter:_TFV4main6STRUCTs1gSi\n"
            L"        willSet {} //_TFV4main6STRUCTw1gSi\n"
            L"        didSet{} // _TFV4main6STRUCTW1gSi\n"
            L"    }\n"
            L"    private var h : Int { return 5} //_TFV4main4SSSSgP33_10900790B424C44FA87F9D97B329E2781hSi\n"
            L"}");
    ASSERT_NO_ERRORS();
    TypePtr STRUCT;
    ASSERT_NOT_NULL(STRUCT = dynamic_pointer_cast<Type>(scope->lookup(L"STRUCT")));
    SymbolPtr s;
    ComputedPropertySymbolPtr p;
    ASSERT_NOT_NULL(s = STRUCT->getMember(L"a"));
    ASSERT_EQ(L"", mangling.encode(s));
    ASSERT_NOT_NULL(s = STRUCT->getMember(L"b"));
    ASSERT_EQ(L"", mangling.encode(s));
    ASSERT_NOT_NULL(p = dynamic_pointer_cast<ComputedPropertySymbol>(STRUCT->getMember(L"c")));
    ASSERT_NOT_NULL(p->getGetter());
    ASSERT_EQ(L"_TFV4main6STRUCTg1cSi", mangling.encode(p->getGetter()));

    ASSERT_NOT_NULL(s = STRUCT->getDeclaredStaticMember(L"d"));
    ASSERT_EQ(L"_TvV4main6STRUCT1dSi", mangling.encode(s));

    ASSERT_NOT_NULL(s = STRUCT->getDeclaredStaticMember(L"e"));
    ASSERT_EQ(L"_TvV4main6STRUCT1eSi", mangling.encode(s));

    ASSERT_NOT_NULL(p = dynamic_pointer_cast<ComputedPropertySymbol>(STRUCT->getMember(L"f")));
    ASSERT_NOT_NULL(p->getGetter());
    ASSERT_EQ(L"_TFV4main6STRUCTg1fSi", mangling.encode(p->getGetter()));
    ASSERT_EQ(L"_TFV4main6STRUCTs1fSi", mangling.encode(p->getSetter()));

    ASSERT_NOT_NULL(p = dynamic_pointer_cast<ComputedPropertySymbol>(STRUCT->getMember(L"g")));
    ASSERT_NOT_NULL(p->getGetter());
    ASSERT_NOT_NULL(p->getSetter());
    ASSERT_NOT_NULL(p->getWillSet());
    ASSERT_NOT_NULL(p->getDidSet());
    ASSERT_EQ(L"_TFV4main6STRUCTg1gSi", mangling.encode(p->getGetter()));
    ASSERT_EQ(L"_TFV4main6STRUCTs1gSi", mangling.encode(p->getSetter()));
    ASSERT_EQ(L"_TFV4main6STRUCTw1gSi", mangling.encode(p->getWillSet()));
    ASSERT_EQ(L"_TFV4main6STRUCTW1gSi", mangling.encode(p->getDidSet()));

    ASSERT_NOT_NULL(p = dynamic_pointer_cast<ComputedPropertySymbol>(STRUCT->getMember(L"h")));
    ASSERT_NOT_NULL(p->getGetter());
    ASSERT_EQ(L"_TFV4main6STRUCTgP33_fad58de7366495db4650cfefac2fcd611hSi", mangling.encode(p->getGetter()));
}


TEST(TestNameMangling, Func1)
{
    SEMANTIC_ANALYZE(L"func test(a : UInt8, b : UInt16, c : UInt32, d : UInt64, e: UInt)\n"
            L"{\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test"));
    ASSERT_EQ(L"_TF4main4testFTVSs5UInt8VSs6UInt16VSs6UInt32VSs6UInt64Su_T_", mangling.encode(s));
}

TEST(TestNameMangling, Func2)
{
    SEMANTIC_ANALYZE(L"func test2(a : Int8, b : Int16, c : Int32, d : Int64, e: Int)\n"
            L"{\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test2"));
    ASSERT_EQ(L"_TF4main5test2FTVSs4Int8VSs5Int16VSs5Int32VSs5Int64Si_T_", mangling.encode(s));
}

TEST(TestNameMangling, Func3)
{
    SEMANTIC_ANALYZE(L"func test3(a : Float32, b : Float64, c : Float80, d : Float, e : Double)\n"
            L"{\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test3"));
    ASSERT_EQ(L"_TF4main5test3FTSfSdVSs7Float80SfSd_T_", mangling.encode(s));
}

TEST(TestNameMangling, Func4)
{
    SEMANTIC_ANALYZE(L"func test4(a : String, b : Bool, c : Character)\n"
            L"{\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test4"));
    ASSERT_EQ(L"_TF4main5test4FTSSSbOSs9Character_T_", mangling.encode(s));
}

TEST(TestNameMangling, Func5)
{
    SEMANTIC_ANALYZE(L"enum MyEnum{case A}\n"
            L"protocol MyProtocol{}\n"
            L"class MyClass{}\n"
            L"struct MyStruct{}\n"
            L"\n"
            L"func test5(a : MyEnum, b : MyProtocol, c : MyClass, d : MyStruct){\n"
            L"//_TF4main5test5FTOS_6MyEnumPS_10MyProtocol_CS_7MyClassVS_8MyStruct_T_\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test5"));
    ASSERT_EQ(L"_TF4main5test5FTOS_6MyEnumPS_10MyProtocol_CS_7MyClassVS_8MyStruct_T_", mangling.encode(s));
}

TEST(TestNameMangling, Func6)
{
    SEMANTIC_ANALYZE(L"enum Maybe<T>\n"
            L"{\n"
            L"    case Nothing, Just(T)\n"
            L"}\n"
            L"\n"
            L"enum MAYBE<T>\n"
            L"{\n"
            L"    case Nothing, Just(T)\n"
            L"}\n"
            L"class MyClass{}\n"
            L"\n"
            L"func test6(c : Int, b : Maybe<MyClass>, a : MAYBE<Int>, d : MAYBE<MyClass>, e : MAYBE<Maybe<MyClass> >)\n"
            L"{\n"
            L"    //_TF4main5test6FTSiGOS_5MaybeCS_7MyClass_GOS_5MAYBESi_GS2_S1__GS2_GS0_S1____T_\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test6"));
    ASSERT_EQ(L"_TF4main5test6FTSiGOS_5MaybeCS_7MyClass_GOS_5MAYBESi_GS2_S1__GS2_GS0_S1____T_", mangling.encode(s));
}
