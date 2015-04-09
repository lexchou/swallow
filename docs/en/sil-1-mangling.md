Gwynne Raskind did a same research just after swift released：[Friday Q&A 2014-08-08: Swift Name Mangling](https://mikeash.com/pyblog/friday-qa-2014-08-15-swift-name-mangling.html), since Gwynne's research was based on a beta version of swift, swift got some minor updates later, I decided to make a same research for my open source project swallow and will cover the aspects that Gwynne was missing(e.g. generic specialization, operator overloading).

## Mangling rule for Property
Test code, the symbols extracted from SIL was added in the comment:
```
public var publicVar : Int = 3     // _Tv4main9publicVarSi
public var publicVar2 : Int { return 3} //_TF4maing10publicVar2Si
internal var internalVar : Int = 4  //_Tv4main11internalVarSi
internal var internalVar2 : Int { return 4}  //_TF4maing12internalVar2Si
private var privateVar : Int = 5 //_Tv4mainP33_10900790B424C44FA87F9D97B329E27810privateVarSi
private var privateVar2 : Int { return 5} //_TF4maingP33_10900790B424C44FA87F9D97B329E27811privateVar2Si


public let publicLet : Int = 3 // _Tv4main9publicLetSi
internal let internalLet : Int = 4 //_Tv4main11internalLetSi
private let privateLet : Int = 5 //_Tv4mainP33_10900790B424C44FA87F9D97B329E27810privateLetSi


struct STRUCT
{
    var a : Int = 3
    let b : Int = 4
    var c : Int { return 5} // getter: _TFV4main6STRUCTg1cSi
    static var d : Int = 6 // _TvV4main6STRUCT1dSi (mutable addressor: _TFV4main6STRUCTa1dSi)
    static let e : Int = 7 //_TvV4main6STRUCT1eSi  (mutable addressor: _TFV4main6STRUCTa1eSi)
    var f : Int { get { return 8} set{}} // getter: _TFV4main6STRUCTg1fSi setter:_TFV4main6STRUCTs1fSi
    var g : Int = 9 {  // getter: _TFV4main6STRUCTg1gSi, setter:_TFV4main6STRUCTs1gSi
        willSet {} //_TFV4main6STRUCTw1gSi
        didSet{} // _TFV4main6STRUCTW1gSi
    }
    private var h : Int { return 5} //_TFV4main4SSSSgP33_10900790B424C44FA87F9D97B329E2781hSi
}
func foo()
{
    var a : Int = 5
    
}
```

According to the generated SIL code, we can find out stored property within a struct was optimized as offset access, and no symbol name generated for local variable, this make sense, because these stuffs don't invoves into symbol linking, only the size and offset matters.
And we can infer the rules from these symbols:

- _T is a must prefix, marks it a swift symbol
- v means it's a symbol for variable
- V Not sure, but we'll find out in the later analysis
- F means it's a function symbol
- Full qualified name was encoded by each part, each part has a prefix of its length, no \0 as terminator
- g means it is a property getter, and s is a setter
- w means its a willSet accessor, W represents a didSet accessor
- Access modifier *Public* and *Internal* only make sense in semantic stage, but *Private* will be encoded with as 'P'
- Private property will generatea a '_' with the hash of module name
- a means its a mutable addressor, the both two static properties in the sample code exists an addressor, according to SIL, it's a function which will return the variable'
s address, I'll analysis it in the following SIL research reports.
- Si here represents an integer type, an abbreviation for Swift.Int, more details about the type's encoding rules is in the next chapter.



# Rules for type mangling

Test code and their symbol names:
```
func test(a : UInt8, b : UInt16, c : UInt32, d : UInt64, e: UInt)
{
    // main.test (Swift.UInt8, Swift.UInt16, Swift.UInt32, Swift.UInt64, Swift.UInt) -> ()
    // sil @_TF4main4testFTVSs5UInt8VSs6UInt16VSs6UInt32VSs6UInt64Su_T_ : $@thin (UInt8, UInt16, UInt32, UInt64, UInt) -> ()
}


func test2(a : Int8, b : Int16, c : Int32, d : Int64, e: Int)
{
    // main.test2 (Swift.Int8, Swift.Int16, Swift.Int32, Swift.Int64, Swift.Int) -> ()
    // sil @_TF4main5test2FTVSs4Int8VSs5Int16VSs5Int32VSs5Int64Si_T_ : $@thin (Int8, Int16, Int32, Int64, Int) -> ()
}
func test3(a : Float32, b : Float64, c : Float80, d : Float, e : Double)
{
    // main.test3 (Swift.Float, Swift.Double, Swift.Float80, Swift.Float, Swift.Double) -> ()
    // sil @_TF4main5test3FTSfSdVSs7Float80SfSd_T_ : $@thin (Float, Double, Float80, Float, Double) -> ()
}
func test4(a : String, b : Bool, c : Character)
{
    // main.test4 (Swift.String, Swift.Bool, Swift.Character) -> ()
    // sil @_TF4main5test4FTSSSbOSs9Character_T_ : $@thin (@owned String, Bool, @owned Character) -> ()
}
enum MyEnum{case A}
protocol MyProtocol{}
class MyClass{}
struct MyStruct{}

func test5(a : MyEnum, b : MyProtocol, c : MyClass, d : MyStruct){
//_TF4main5test5FTOS_6MyEnumPS_10MyProtocol_CS_7MyClassVS_8MyStruct_T_
}
enum Maybe<T>
{
    case Nothing, Just(T)
}

enum MAYBE<T>
{
    case Nothing, Just(T)
}

func test6(c : Int, b : Maybe<MyClass>, a : MAYBE<Int>, d : MAYBE<MyClass>, e : MAYBE<Maybe<MyClass>>)
{
    //_TF4main5test6FTSiGOS_5MaybeCS_7MyClass_GOS_5MAYBESi_GS2_S1__GS2_GS0_S1____T_
}

func test7(a : [Int], b : Bool?, c : Bool!, d : [String : Int], e : (Int, Double))
{
    //_TF4main5test7FTGSaSi_GSqSb_GSQSb_GVSs10DictionarySSSi_TSiSd__T_

}
func composition(a : protocol<Printable, DebugPrintable>) 
{
    //_TF4main11compositionFPSs14DebugPrintableSs9Printable_T_
}

class OuterClass
{
    class Nested
    {
        class Inner
        {
            func innerFunc() //_TFCCC4main10OuterClass6Nested5Inner9innerFuncfS2_FT_T_
            {
                
            }
        }
    }
}

func constraint<A : protocol<MyProtocol, Reflectable>, B : RawRepresentable where B.RawValue == Int>(arg : (A, B)) // _TF4main10constraintUSs11ReflectableS_10MyProtocol_Ss16RawRepresentable__FTQ_Q0__T_
{
    
}

```
It's not too hard to find out the "basic" types and their abbreviations:

Type name | Abbreviation
---------|------
Swift.UInt | Su
Swift.Int | Si
Swift.Float32 | Sf
Swift.Float64 | Sd
Swift.Float | Sf
Swift.Double | Sd
Swift.String | SS
Swift.Bool | Sb
Swift.Array | Sa
Swift.UnicodeScalar | Sc
ObjectiveC | So
Optional | Sq
ImplicitlyUnwrappedOptional | SQ

Now compares with the rules in the previous chapter, it's easy to guess:

- O is an Enum type, V represents Struct， C represents Class， and P represents Protocol，the type flags can exists multiple times to represent a nested type, like _TFCCC4main10OuterClass6Nested5Inner9innerFuncfS2_FT_T_ has C appeared for 3 times, means OuterClass/Nested/Inner are all Class
- S_ will follow a type name which defined in current module.
- Ss will follow a type name which defined in module 'Swift', the types from swift standard library.
- S follows a letter means a type that can be looked up in the table above.
- S follows a digit is used as a type reference that exists in the encoding, the index starts from 0, usually the member symbol will use S0 as the current type.
- G means a specialized generic type, a generic type is followed after G while the rest type arguments are followed next, and ended with _
- T means a tuple type, like the rule for specialized types, the element types are just after the T and ended with _
- F means it's a function type, parameter types(encoded by a tuple) and return type are next to it, no _ is required for function type.
- P means Protocol Composition, followed by all protocol type names, and ended with _
- Single protocol type also treated as protocol composition, declaration `var a : MyProtocol` is totally equivalent to  `var a : protocol<MyProtocol>`, that means for a single protocol there will also be a _
- U defines n generic parameters with n+1 underscores, there's an optional generic constraint for each underscore, with a set of protocol name list.
- Q used to refer the generic parameter, Q_ means the 1st generic argument type, and Qn_ means the (n+1)th argument type
- The type names beyond the abbreviations are encoded with the rule used in symbol name encoding.


So we know OSs9Character represents an *Enum* type named *Character* which defined in module *Swift*, and VSs5Int64 represents a *struct* defined in *Swift* named Int64.

Let's see a more complicated example `_Tv4main1bGOS_1aSiGOS_2acSiSi__`, it can be deconstructed into  `_Tv 4main 1b GOS_1a<Si, GOS_2ac<Si,Si>_>_` which means `var b : a<Int, ac<Int, Int>>`

## Mangling rules for methods

Test code and the SIL symbols:
```
struct SSSS
{
    func method() // _TFV4main4SSSS6methodfS0_FT_T_
    {
        
    }
    static func smethod() -> Int // _TFV4main4SSSS7smethodfMS0_FT_Si
    {
        return 0
    }
    mutating func mmethod() // _TFV4main4SSSS7mmethodfRS0_FT_T_
    {
        
    }
}

class CCCC
{
    init() // _TFC4main4CCCCcfMS0_FT_S0_  allocating_init:_TFC4main4CCCCCfMS0_FT_S0_
    {
        
    }
    init?(a : Int) // _TFC4main4CCCCcfMS0_FT1aSi_GSqS0__ allocating_init:_TFC4main4CCCCCfMS0_FT1aSi_GSqS0__
    { 
        
    }
    init!(a : Int, b : Int) // _TFC4main4CCCCcfMS0_FT1aSi1bSi_GSQS0__ allocating_init:_TFC4main4CCCCCfMS0_FT1aSi1bSi_GSQS0__
    {
        
    }
    convenience init(a : Bool) // _TFC4main4CCCCcfMS0_FT1aSb_S0_ allocating_init:_TFC4main4CCCCCfMS0_FT1aSb_S0_
    {
        self.init();
    }
    deinit // _TFC4main4CCCCd deallocating_init: _TFC4main4CCCCD
    {
        
    }
    func method() //_TFC4main4CCCC6methodfS0_FT_T_
    {
        
    }
    class func classFunc() // _TFC4main4CCCC9classFuncfMS0_FT_T_
    {
        
    }
}

func add(a : Int, b : Int) -> Int //_TF4main3addFTSiSi_Si
{
    return a + b
}

func add2(a : Int) (c : Bool) (b : Int) -> Int // _TF4main4add2fSifT1cSb_FT1bSi_Si
{
    return a + b
}
func makeOpt(inout a : Int) -> Int? // _TF4main7makeOptFRSiGSqSi_
{
    return a
}
func highOrder(a : (Int, Int) -> Bool) -> Int    //_TF4main9highOrderFFTSiSi_SbSi
{
    return 0;
}

func makeTuple<T1, T2, T3>(t1 : T1, t2 : T2, t3 : T3) -> (T1, T2, T3) //_TF4main9makeTupleU____FTQ_Q0_Q1__TQ_Q0_Q1__
{
    return (t1, t2, t3);
}

func dec(a : Int) -> Int //_TF4main3decFSiSi
{
    return a - 1;
}

func 他们为什么不说中文(a : Int) -> Int //_TF4mainX24ihqwcrbEcvIaIdqgAFGpqjyeFSiSi
{
    return a - 1;
}

```

According to SIL code, compiler will generate a `__allocating_init` for each initializer with the same function signature, and the other rules:

- V means it's a member of struct, described in previous chapter.
- F means it's a function type
- S0 here represents the Self type which was already discussed.
- The 'F' after the function name marks the beginning of parameter list, whereas the 'f' marks the beginning of a curried function parameter list.
- T represents a tuple followed by each parameter's type, each parameter are composed by _External Name_ : _function flags_ : _parameter type_, e.g. 1aRSi represents `inout a a : Swift.Int`
- Tuple can be ignored if there's only one parameter
- M means it's a meta type
- R means it's a reference(in Swift, it's _inout_ keyword) 
- 'c' after a type name means it's an initializer, and 'C' means it is an allocating initializer.
- GSqS0 Here means the failable initializer has a return type of Optional<Self>, GSQS0 means a return type of ImplicitlyUnwrappedOptional<Self> for a failable initializer, this was discussed in previous chapter.
- conveniene only make sens in semantic stage, won't make any differences in name manling.
- d represents deinit, D represents deallocating init
- X means the name was encoded by [Punycode RFC 3492](http://www.ietf.org/rfc/rfc3492.txt), Apple made some changes on the punycode encoding/decoding, In the above code, the "他们为什么不说中文" after encoded is ihqwcrbEcvIaIdqgAFGpqjye, but it should be ihqwcrb4cv8a8dqg056pqjye according by RFC 3492.

Some notes for flag **M** and **R**, M appears in the first parameter of the first curried function parameter list, check this:

```
struct Foo
{
    static func bar(args : Int){}
}
```
Compiler will treat it as:
```
func bar(self : Foo.Type)(args : Int){}
```
That means the self exists in a static function, but the self's type is a meta type **Self**.

And for flag **R**, it means it's a reference type, there's two way to make it appear in the signature, mutating function and inout parameter, compiler will make the implicit parameter self mutable by make it passed by reference, so for a mutating function: 

```
struct Foo
{
    mutating func bar(args : Int) {}
}
```
Compiler treat it as:
```
func bar(inout self : Foo)(args : Int) {}
```
And for initializer: 

```
init(a : Int)
```
Compiler treat it as:
```
func init(class : Self.Type) ( a : Int) -> Self
```
That's why there's always a fS0_ in the initializer's parameter list.

# Mangling rules for Enum
There's two Enum kinds, union enum and raw-values enum, the members for them and the symbols generated:
```
enum ENUM
{
    case A // _TFO4main4ENUM1AFMS0_S0_
    case B(Int) // _TFO4main4ENUM1BfMS0_FSiS0_
    func foo() // _TFO4main4ENUM3foofS0_FT_T_
    {
        
    }
    init(a : Int) // _TFO4main4ENUMCfMS0_FT1aSi_S0_
    {
        self = .A
    }
    static func sfoo() // _TFO4main4ENUM4sfoofMS0_FT_T_
    {
        
    }
}
enum RAW_VALUE : String
{
    case A = "A" // _TFO4main9RAW_VALUE1AFMS0_S0_
    case B = "B" // _TFO4main9RAW_VALUE1BFMS0_S0_
    func foo() // _TFO4main9RAW_VALUE3foofS0_FT_T_
    {
        
    }
    // main.RAW_VALUE.init (main.RAW_VALUE.Type)(rawValue : Swift.String) -> Swift.Optional<main.RAW_VALUE>
    // sil @_TFO4main9RAW_VALUECfMS0_FT8rawValueSS_GSqS0__ : $@thin (@owned String, @thin RAW_VALUE.Type) -> Optional<RAW_VALUE>
    mutating func mfoo()  //_TFO4main9RAW_VALUE4mfoofRS0_FT_T_
    {
        
    }
}
```
We can know normal cases in a union enum(with no associated data) are generated just like the cases in raw-values enum:
```
static func A() -> ENUM
```
The cases with associated data are just translated to parameters:
```
static func B(arg : Int) -> ENUM
```



# Rules for operator overloadings
Code:
```

infix operator +++{}
infix operator +=+{}
prefix operator ---{}
postfix operator !%^&*|~/<>?{}
func +++(a : Int, b : Bool) // _TF4mainoi3pppFTSiSb_T_
{
    
}
func +=+(a : Int, b : Bool) -> Bool // _TF4mainoi3pepFTSiSb_Sb
{
    return true
}
prefix func ---(a : String) -> String // _TF4mainop3sssFSSSS
{
    return ""
}
postfix func !%^&*|~/<>?(a : Int) -> Int // _TF4mainoP11nrxamotdlgqFSiSi
{
    return 0
}
```

- o Means it's an operator overloading
- i Means it's an infix operator
- p Means it's a prefix operator
- P Means it's a postfix operator

Operator | Abbreviation
---|---
+ | p
= | e
- | s
! | n
% | r
^ | x
& | a
* | m
\| | o
~ | t
/ | d
< | l
> | g
? | q


# Mangling rules for Protocol Witness table
Extension is a quite sweet feature in swift, though it's not invented by swift, there'are similar concepts in other languages like trait in scala and extension in C#, but swift allows to implement a protocol for an existing type which C#'s extension doesn't, interface in golang is much more like extension in swift in this way, only an explicit declaration is needed. 

Let's see this:
```
protocol MyProtocol
{
    func asInt() -> Int
}

extension Int : MyProtocol
{
    func asInt() -> Int //_TFE4mainSi5asIntfSiFT_Si,  witness: _TTWSi4main10MyProtocolFS0_5asIntUS0___fRQPS0_FT_Si
    {
        return self
    }
}
```

Now we found a new flag **E** in **_TFE4mainSi5asIntfSiFT_Si**, it's the first letter of **e**xtension, the rest parts are already described before.

We found another extra symbol, this is called witness function, I'll write about it in different articles, but here we will only discuss name mangling.
There's a TW after the **_T**, W is an abbreviation of witness, I don't know what the T means here, the extended type's name comes next and the protocol's name, FS0_5asInt can be expanded to MyProtocol.asInt, **US0___** is a generic definition, the S0_ means the first generic parameter must conform to protocol _MyProtocol_, the *fRQPS0_FT_Si* is the signature of the function, **R** is reference, means it's an inout parameter, I'm not quite sure about the Q and P here, according to my experience, removal of P won't affects the decoding result of swift-demangle.

There's 3 questions here, but it won't stop me from writing test cases for my own compiler:

1. Don't know what the T after _T works.
2. There's only one generic parameter, but there's 3 underscores, there should be two generic parameters according to previous research.
3. Not sure what the Q and P means in the curried function parameter list.

# Summary
According to the previous researches, we know a symbol is usually composed by:
**_T** *symbol type (F/v)*  **Owner type(C/O/P/V/E)?**  *Module name* **Type name?**  *Accessor type(g getter,s setter, w willSet W didSet)?*  **(P _ module hash)?**  *Symbol name* **Symbol type**
Pseudocode for encoding:
```
print "_T";
print symbol.kind;
print symbol.ownerType.kind if defined(symbol.ownerType);
print encode(symbol.moduleName);
print encode(symbol.ownerType.name) if defined(symbol.ownerType);
print symbol.accessorType if defined(symbol.accessorType);
print "P".encode("_".hash(symbol.module)) if symbol.accessLevel eq 'private';
print encode(symbol.name);
print encodeType(symbol.type);
```

# Questions
There's two places I can't make it exactly the same like official implementation.

1. The hash calculation for private symbol, I thought the hash function should be something like `h(x)=md5(f(x))`, then I can know what f(x) is only when decrypted the md5 result by brute force, then I tried to use GPU to decrypt it, finally failed after two day's of calculating, so I think the hash function could be something like `h(x)=g(md5(f(x)))`.
2. Another question is about the punycode encoding, I'll use a simple implementation of punycode in my test cases.

I'll change the implementation of my compiler when someone figured it out these questions, or do some assembly-level debugging to trace it if I have time.

