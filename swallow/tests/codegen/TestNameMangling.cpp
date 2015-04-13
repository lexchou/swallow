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
#include "semantics/FunctionOverloadedSymbol.h"
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

TEST(TestNameMangling, Func7)
{
    SEMANTIC_ANALYZE(L"func test7(a : [Int], b : Bool?, c : Bool!, d : [String : Int], e : (Int, Double))\n"
            L"{\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"test7"));
    ASSERT_EQ(L"_TF4main5test7FTGSaSi_GSqSb_GSQSb_GVSs10DictionarySSSi_TSiSd__T_", mangling.encode(s));
}

TEST(TestNameMangling, Func8)
{
    SEMANTIC_ANALYZE(L"func composition(a : protocol<Printable, DebugPrintable>)\n"
            L"{\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"composition"));
    ASSERT_EQ(L"_TF4main11compositionFPSs14DebugPrintableSs9Printable_T_", mangling.encode(s));
}

TEST(TestNameMangling, Func9)
{
    SEMANTIC_ANALYZE(L"class OuterClass\n"
            L"{\n"
            L"    class Nested\n"
            L"    {\n"
            L"        class Inner\n"
            L"        {\n"
            L"            func innerFunc() //_TFCCC4main10OuterClass6Nested5Inner9innerFuncfS2_FT_T_\n"
            L"            {\n"
            L"\n"
            L"            }\n"
            L"        }\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s1, s2, s3, s4, s5;
    ASSERT_NOT_NULL(s1 = scope->lookup(L"OuterClass"));
    ASSERT_NOT_NULL(s2 = dynamic_pointer_cast<Type>(s1)->getMember(L"Nested"));
    ASSERT_NOT_NULL(s3 = dynamic_pointer_cast<Type>(s2)->getMember(L"Inner"));
    ASSERT_NOT_NULL(s4 = dynamic_pointer_cast<Type>(s3)->getMember(L"innerFunc"));
    ASSERT_NOT_NULL(s5 = *dynamic_pointer_cast<FunctionOverloadedSymbol>(s4)->begin());
    ASSERT_EQ(L"_TFCCC4main10OuterClass6Nested5Inner9innerFuncfS2_FT_T_", mangling.encode(s5));
}

TEST(TestNameMangling, Func10)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol{}\n"
            L"func constraint<A : protocol<MyProtocol, Printable>, B : RawRepresentable where B.RawValue == Int>(arg : (A, B)) // _TF4main10constraintUSs11ReflectableS_10MyProtocol_Ss16RawRepresentable__FTQ_Q0__T_\n"
            L"{\n"
            L"\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"constraint"));
    ASSERT_EQ(L"_TF4main10constraintUSs9PrintableS_10MyProtocol_Ss16RawRepresentable__FTQ_Q0__T_", mangling.encode(s));
}

static FunctionSymbolPtr getMethod(const TypePtr& type, const wstring& name)
{
    SymbolPtr s = type->getMember(name);
    if(!s)
        s = type->getDeclaredStaticMember(name);
    if(!s)
        return nullptr;
    if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(s))
        return func;
    FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(s);
    if(!funcs || funcs->numOverloads() == 0)
        return nullptr;
    return *funcs->begin();
}

TEST(TestNameMangling, Func_Struct)
{
    SEMANTIC_ANALYZE(L"struct SSSS\n"
            L"{\n"
            L"    func method() // _TFV4main4SSSS6methodfS0_FT_T_\n"
            L"    {\n"
            L"    }\n"
            L"    static func smethod() -> Int // _TFV4main4SSSS7smethodfMS0_FT_Si\n"
            L"    {\n"
            L"        return 0\n"
            L"    }\n"
            L"    mutating func mmethod() // _TFV4main4SSSS7mmethodfRS0_FT_T_\n"
            L"    {\n"
            L"\n"
            L"    }\n"
            L"}")
    ASSERT_NO_ERRORS();
    TypePtr SSSS;
    SymbolPtr m;
    ASSERT_NOT_NULL(SSSS = dynamic_pointer_cast<Type>(scope->lookup(L"SSSS")));
    ASSERT_NOT_NULL(m = getMethod(SSSS, L"method"));
    ASSERT_EQ(L"_TFV4main4SSSS6methodfS0_FT_T_", mangling.encode(m));
    ASSERT_NOT_NULL(m = getMethod(SSSS, L"smethod"));
    ASSERT_EQ(L"_TFV4main4SSSS7smethodfMS0_FT_Si", mangling.encode(m));
    ASSERT_NOT_NULL(m = getMethod(SSSS, L"mmethod"));
    ASSERT_EQ(L"_TFV4main4SSSS7mmethodfRS0_FT_T_", mangling.encode(m));
}
TEST(TestNameMangling, Func_Class)
{
    SEMANTIC_ANALYZE(L"class CCCC\n"
            L"{\n"
            L"    init() // _TFC4main4CCCCcfMS0_FT_S0_  allocating_init:_TFC4main4CCCCCfMS0_FT_S0_\n"
            L"    {\n"
            L"    }\n"
            L"    init?(a : Int) // _TFC4main4CCCCcfMS0_FT1aSi_GSqS0__ allocating_init:_TFC4main4CCCCCfMS0_FT1aSi_GSqS0__\n"
            L"    { \n"
            L"    }\n"
            L"    init!(a : Int, b : Int) // _TFC4main4CCCCcfMS0_FT1aSi1bSi_GSQS0__ allocating_init:_TFC4main4CCCCCfMS0_FT1aSi1bSi_GSQS0__\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Bool) // _TFC4main4CCCCcfMS0_FT1aSb_S0_ allocating_init:_TFC4main4CCCCCfMS0_FT1aSb_S0_\n"
            L"    {\n"
            L"        self.init();\n"
            L"    }\n"
            L"    deinit // _TFC4main4CCCCd deallocating_init: _TFC4main4CCCCD\n"
            L"    {\n"
            L"    }\n"
            L"    func method() //_TFC4main4CCCC6methodfS0_FT_T_\n"
            L"    {\n"
            L"    }\n"
            L"    class func classFunc() // _TFC4main4CCCC9classFuncfMS0_FT_T_\n"
            L"    {\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
    TypePtr CCCC;
    ASSERT_NOT_NULL(CCCC = dynamic_pointer_cast<Type>(scope->lookup(L"CCCC")));
    ASSERT_EQ(4, CCCC->getDeclaredInitializer()->numOverloads());
    SymbolPtr m;
    auto iter = CCCC->getDeclaredInitializer()->begin();
    ASSERT_NOT_NULL(m = *iter++);
    ASSERT_EQ(L"_TFC4main4CCCCcfMS0_FT_S0_", mangling.encode(m));
    ASSERT_NOT_NULL(m = *iter++);
    ASSERT_EQ(L"_TFC4main4CCCCcfMS0_FT1aSi_GSqS0__", mangling.encode(m));
    ASSERT_NOT_NULL(m = *iter++);
    ASSERT_EQ(L"_TFC4main4CCCCcfMS0_FT1aSi1bSi_GSQS0__", mangling.encode(m));
    ASSERT_NOT_NULL(m = *iter++);
    ASSERT_EQ(L"_TFC4main4CCCCcfMS0_FT1aSb_S0_", mangling.encode(m));

    ASSERT_EQ(L"_TFC4main4CCCCd", mangling.encode(CCCC->getDeinit()));

    ASSERT_NOT_NULL(m = getMethod(CCCC, L"method"));
    ASSERT_EQ(L"_TFC4main4CCCC6methodfS0_FT_T_", mangling.encode(m));

    ASSERT_NOT_NULL(m = getMethod(CCCC, L"classFunc"));
    ASSERT_EQ(L"_TFC4main4CCCC9classFuncfMS0_FT_T_", mangling.encode(m));
    //TODO: test allocating/deallocating initializer mangling
}

TEST(TestNameMangling, Func11)
{
    SEMANTIC_ANALYZE(L"func add(a : Int, b : Int) -> Int //_TF4main3addFTSiSi_Si\n"
            L"{\n"
            L"    return a + b\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"add"));
    ASSERT_EQ(L"_TF4main3addFTSiSi_Si", mangling.encode(s));
}

#if 0
TEST(TestNameMangling, Func12)
{
    SEMANTIC_ANALYZE(L"func add2(a : Int) (c : Bool) (b : Int) -> Int // _TF4main4add2fSifT1cSb_FT1bSi_Si\n"
            L"{\n"
            L"    return a + b\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"add"));
    ASSERT_EQ(L"_TF4main4add2fSifT1cSb_FT1bSi_Si", mangling.encode(s));
}
#endif

TEST(TestNameMangling, Func13)
{
    SEMANTIC_ANALYZE(L"func makeOpt(inout a : Int) -> Int? // _TF4main7makeOptFRSiGSqSi_\n"
            L"{\n"
            L"    return nil\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"makeOpt"));
    ASSERT_EQ(L"_TF4main7makeOptFRSiGSqSi_", mangling.encode(s));
}

TEST(TestNameMangling, Func14)
{
    SEMANTIC_ANALYZE(L"func highOrder(a : (Int, Int) -> Bool) -> Int    //_TF4main9highOrderFFTSiSi_SbSi\n"
            L"{\n"
            L"    return 0;\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"highOrder"));
    ASSERT_EQ(L"_TF4main9highOrderFFTSiSi_SbSi", mangling.encode(s));
}

TEST(TestNameMangling, Func15)
{
    SEMANTIC_ANALYZE(L"func makeTuple<T1, T2, T3>(t1 : T1, t2 : T2, t3 : T3) -> (T1, T2, T3) //_TF4main9makeTupleU____FTQ_Q0_Q1__TQ_Q0_Q1__\n"
            L"{\n"
            L"    return (t1, t2, t3);\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"makeTuple"));
    ASSERT_EQ(L"_TF4main9makeTupleU____FTQ_Q0_Q1__TQ_Q0_Q1__", mangling.encode(s));
}

TEST(TestNameMangling, Func16)
{
    SEMANTIC_ANALYZE(L"func dec(a : Int) -> Int //_TF4main3decFSiSi\n"
            L"{\n"
            L"    return a - 1;\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"dec"));
    ASSERT_EQ(L"_TF4main3decFSiSi", mangling.encode(s));
}

#if 0
//TODO: Add punycode encoding
TEST(TestNameMangling, Func17)
{
    SEMANTIC_ANALYZE(L"func 他们为什么不说中文(a : Int) -> Int //_TF4mainX24ihqwcrbEcvIaIdqgAFGpqjyeFSiSi\n"
            "{\n"
            "    return a - 1;\n"
            "}");
    ASSERT_NO_ERRORS();
    SymbolPtr s;
    ASSERT_NOT_NULL(s = scope->lookup(L"他们为什么不说中文"));
    ASSERT_EQ(L"_TF4mainX24ihqwcrbEcvIaIdqgAFGpqjyeFSiSi", mangling.encode(s));
}
#endif

TEST(TestNameMangling, Enum)
{
    SEMANTIC_ANALYZE(L"enum ENUM\n"
            L"{\n"
            L"    case A // _TFO4main4ENUM1AFMS0_S0_\n"
            L"    case B(Int) // _TFO4main4ENUM1BfMS0_FSiS0_\n"
            L"    func foo() // _TFO4main4ENUM3foofS0_FT_T_\n"
            L"    {\n"
            L"    }\n"
            L"    init(a : Int) // _TFO4main4ENUMCfMS0_FT1aSi_S0_\n"
            L"    {\n"
            L"        self = .A\n"
            L"    }\n"
            L"    static func sfoo() // _TFO4main4ENUM4sfoofMS0_FT_T_\n"
            L"    {\n"
            L"    }\n"
            L"}")
    ASSERT_NO_ERRORS();
    TypePtr ENUM;
    FunctionSymbolPtr c;
    ASSERT_NOT_NULL(ENUM = dynamic_pointer_cast<Type>(scope->lookup(L"ENUM")));
    ASSERT_NOT_NULL(c = ENUM->getEnumCase(L"B")->constructor);
    ASSERT_EQ(L"_TFO4main4ENUM1BfMS0_FSiS0_", mangling.encode(c));

    ASSERT_NOT_NULL(c = ENUM->getEnumCase(L"A")->constructor);
    ASSERT_EQ(L"_TFO4main4ENUM1AFMS0_S0_", mangling.encode(c));

    ASSERT_NOT_NULL(c = getMethod(ENUM, L"foo"));
    ASSERT_EQ(L"_TFO4main4ENUM3foofS0_FT_T_", mangling.encode(c));

    ASSERT_NOT_NULL(c = getMethod(ENUM, L"sfoo"));
    ASSERT_EQ(L"_TFO4main4ENUM4sfoofMS0_FT_T_", mangling.encode(c));

    ASSERT_NOT_NULL(c = getMethod(ENUM, L"init"));
    ASSERT_EQ(L"_TFO4main4ENUMCfMS0_FT1aSi_S0_", mangling.encode(c));

}

TEST(TestNameMangling, Enum2)
{
    SEMANTIC_ANALYZE(L"enum RAW_VALUE : String\n"
            L"{\n"
            L"    case A = \"A\" // _TFO4main9RAW_VALUE1AFMS0_S0_\n"
            L"    case B = \"B\" // _TFO4main9RAW_VALUE1BFMS0_S0_\n"
            L"    func foo() // _TFO4main9RAW_VALUE3foofS0_FT_T_\n"
            L"    {\n"
            L"\n"
            L"    }\n"
            L"    // main.RAW_VALUE.init (main.RAW_VALUE.Type)(rawValue : Swift.String) -> Swift.Optional<main.RAW_VALUE>\n"
            L"    // sil @_TFO4main9RAW_VALUECfMS0_FT8rawValueSS_GSqS0__ : $@thin (@owned String, @thin RAW_VALUE.Type) -> Optional<RAW_VALUE>\n"
            L"    mutating func mfoo()  //_TFO4main9RAW_VALUE4mfoofRS0_FT_T_\n"
            L"    {\n"
            L"\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
    TypePtr RAW_VALUE;
    FunctionSymbolPtr c;
    ASSERT_NOT_NULL(RAW_VALUE = dynamic_pointer_cast<Type>(scope->lookup(L"RAW_VALUE")));
    ASSERT_NOT_NULL(c = RAW_VALUE->getEnumCase(L"A")->constructor);
    ASSERT_EQ(L"_TFO4main9RAW_VALUE1AFMS0_S0_", mangling.encode(c));

    ASSERT_NOT_NULL(c = RAW_VALUE->getEnumCase(L"B")->constructor);
    ASSERT_EQ(L"_TFO4main9RAW_VALUE1BFMS0_S0_", mangling.encode(c));

    ASSERT_NOT_NULL(c = getMethod(RAW_VALUE, L"foo"));
    ASSERT_EQ(L"_TFO4main9RAW_VALUE3foofS0_FT_T_", mangling.encode(c));

    ASSERT_NOT_NULL(c = getMethod(RAW_VALUE, L"init"));
    ASSERT_EQ(L"_TFO4main9RAW_VALUECfMS0_FT8rawValueSS_GSqS0__", mangling.encode(c));

    ASSERT_NOT_NULL(c = getMethod(RAW_VALUE, L"mfoo"));
    ASSERT_EQ(L"_TFO4main9RAW_VALUE4mfoofRS0_FT_T_", mangling.encode(c));

}

TEST(TestNameMangling, Operators)
{
    SEMANTIC_ANALYZE(L"infix operator +++{}\n"
            L"infix operator +=+{}\n"
            L"prefix operator ---{}\n"
            L"postfix operator !%^&*|~/<>?{}\n"
            L"func +++(a : Int, b : Bool) // _TF4mainoi3pppFTSiSb_T_\n"
            L"{\n"
            L"\n"
            L"}\n"
            L"func +=+(a : Int, b : Bool) -> Bool // _TF4mainoi3pepFTSiSb_Sb\n"
            L"{\n"
            L"    return true\n"
            L"}\n"
            L"prefix func ---(a : String) -> String // _TF4mainop3sssFSSSS\n"
            L"{\n"
            L"    return \"\"\n"
            L"}\n"
            L"postfix func !%^&*|~/<>?(a : Int) -> Int // _TF4mainoP11nrxamotdlgqFSiSi\n"
            L"{\n"
            L"    return 0\n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr f;

    ASSERT_NOT_NULL(f = scope->lookup(L"+++"));
    ASSERT_EQ(L"_TF4mainoi3pppFTSiSb_T_", mangling.encode(f));

    ASSERT_NOT_NULL(f = scope->lookup(L"+=+"));
    ASSERT_EQ(L"_TF4mainoi3pepFTSiSb_Sb", mangling.encode(f));

    ASSERT_NOT_NULL(f = scope->lookup(L"---"));
    ASSERT_EQ(L"_TF4mainop3sssFSSSS", mangling.encode(f));

    ASSERT_NOT_NULL(f = scope->lookup(L"!%^&*|~/<>?"));
    ASSERT_EQ(L"_TF4mainoP11nrxamotdlgqFSiSi", mangling.encode(f));


}
TEST(TestNameMangling, Extension)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol\n"
            L"{\n"
            L"    func asInt() -> Int\n"
            L"}\n"
            L"\n"
            L"extension Int : MyProtocol\n"
            L"{\n"
            L"    func asInt() -> Int //_TFE4mainSi5asIntfSiFT_Si,  witness: _TTWSi4main10MyProtocolFS0_5asIntUS0___fRQPS0_FT_Si\n"
            L"    {\n"
            L"        return self\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
    TypePtr Int = scope->getExtension(L"Int");
    FunctionSymbolPtr asInt;
    ASSERT_NOT_NULL(asInt = getMethod(Int, L"asInt"));

    ASSERT_EQ(L"_TFE4mainSi5asIntfSiFT_Si", mangling.encode(asInt));

    //TODO: test witness function encoding

}