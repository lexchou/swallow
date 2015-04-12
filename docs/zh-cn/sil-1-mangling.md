早在Swift刚发布不久，就有老外做了这个研究了：[Friday Q&A 2014-08-08: Swift Name Mangling](https://mikeash.com/pyblog/friday-qa-2014-08-15-swift-name-mangling.html)，不过介于Swift在那之后又有多次更新，该作者又是参考的Beta版的Swift编译器研究的，而且有些东西该作者也没有研究深入（比如泛型特化后的符号，操作符重载等），所以有必要参照他的方法重新做一次总结。

## Property的 Mangling 规则
测试代码，SIL中对应的符号写在注释里的：
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

根据生成的SIL不难发现： struct里的stored property 则是优化成了偏移访问，局部变量也不需要符号名，这个符合常识，因为这些不需要参与链接，只需要知道大小和偏移即可。
对比这些符号可以得出以下规律：

- _T是必须存在的前缀，表示其为一个swift符号
- v 表示这是一个variable符号
- V 还不确定，有待后续分析
- F 表示这是一个函数符号
- Full Qualified Name的编码格式是每个部分编码为长度接字符串（不需要\0结尾）
- g表示这是一个property getter, s 表示是setter
- w 表示是 willSet， W表示是didSet
- 访问限定符Public和Internal只在语义阶段有意义，不影响符号编码，而Private则会编码为P
- Private Property 会有一个 _ 接模块的hash
- a 表示这是个mutable addressor，这例子里两个静态属性都存在一个addressor，根据SIL内容，addressor是一个返回该变量指针的函数，后期再做详细分析
- Si 表示整数类型，即Swift.Int 缩写，关于类型的编码规则，参考后面一节。



# 类型的Mangling 规则

测试代码与其对应的符号名：
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
从中不难看出，自带的几个『基本』类型极其对应的缩写形式为：

类型 | 缩写
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

对比之前的规律，不难总结：

- O 表示Enum, V表示Struct， C表示Class， P表示Protocol，重复多次用来表明嵌套类型，比如 _TFCCC4main10OuterClass6Nested5Inner9innerFuncfS2_FT_T_ 出现了三次C，表示 OuterClass/Nested/Inner 都是Class
- S如果后面跟了_ 表示后面是当前模块内的类型
- Ss表示后面的一个类型名是来自Swift模块内的，即标准库类型
- S后面跟其他字母则按上面的表来反查
- S后面跟一个数字n以及下划线则表示引用编码规则中第n次出现的非缩写类型，n下标以0开始算，如果是序列化的成员符号，则成员所属的类型属于第0个， 例如 S5_ 表示第5个出现过的类型，泛型特化/函数/元组类型则不会被引用。
- G表示泛型特化，G后面第一个类型表示泛型类型，之后接着出现的类型都为泛型参数，然后以_结尾
- T表示Tuple，与泛型特化类似，后面接的都是元素类型，然后以_结尾
- F 表示函数类型，后面跟一个类型Tuple，然后一个返回值，这个没有_结尾标志
- P 表示Protocol Composition, 后面跟protocol列表，然后以_结尾
- 对于单个的protocol类型，也是当做Protocol Composition处理的。即 `var a : MyProtocol` 完全等价于 `var a : protocol<MyProtocol>`，因此单个Protocol在编码的时候也会有_结尾
- U 表示定义泛型参数，后面跟n+1个下划线_，表示有n个泛型参数，下划线前面有一个可选的constraint，接一组Protocol名列表
- Q 后面用来引用泛型参数，用Q_表示第0个，Qn_表示第n+1个
- 缩写以外的类型则按符号的编码规则进行编码



因此 OSs9Character 即表示 Swift模块内名为Character的Enum，而VSs5Int64表示Swift模块内名为Int64的struct。
按照以上规则，可以很容易的看出 `_Tv4main1bGOS_1aSiGOS_2acSiSi__` 可以分解为 `_Tv 4main 1b GOS_1a<Si, GOS_2ac<Si,Si>_>_` 即 `var b : a<Int, ac<Int, Int>>`。

## Method 的 Mangling 规则

测试代码与对应的SIL符号：
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
根据SIL发现，每个init编译器都会生成一组对应函数签名的 __allocating_init ，而其他的规则：

- V 表示是struct的成员
- F 表示是函数类型，详细可以参考后面的介绍
- S0 表示一个Self类型参数，即当前类型，关于S0的详细描述参考
- 函数名后面的F表示参数列表开始， 而f表示Curried Function 参数列表开始
- T 表示一个Tuple，在函数的签名例子里是表示参数列表，后面跟着参数，每个参数分别是  External Name : 标志 : 类型，例如 1aRSi 表示 `inout a a : Swift.Int`
- 如果函数参数只有一个，则会省略掉Tuple表示参数
- _ 参数列表分隔符
- M 参数是一个meta type
- R 表示参数是一个reference
- C 表示是class的成员
- 类名后的c 表示是init， C 表示是allocating init
- GSqS0 表示返回Optional<Self>的Failable Initializer, GSQS0 表示ImplicitlyUnwrappedOptional<Self>的Failable initializer，这个在类型一节已经说了。
- conveniene 只是语义阶段有意义，不影响符号名生成
- d 表示 deinit, D 表示deallocating init
- X 表示进行了punycode编码的名字，详细描述参考 [RFC 3492](http://www.ietf.org/rfc/rfc3492.txt)。Apple在这个基础之上进行了修改，比如上面例子中的『他们为什么不说中文』编码后是ihqwcrbEcvIaIdqgAFGpqjye，而按照RFC 3492标准编码后应该是ihqwcrb4cv8a8dqg056pqjye，有一些差异还需后续研究。

关于M和R需要注明的是，对于静态函数而言，M出现在第一个Curried Function的第一个参数上，参考代码：
```
struct Foo
{
    static func bar(args : Int){}
}
```
编译器理解为：
```
func bar(self : Foo.Type)(args : Int){}
```
静态函数也有self参数，不过传递进去的是元类型。
而对于R而言，R表示这个参数是一个引用类型，让签名出现R有两种方法，第一个是mutating function，第二个是inout parameter。因为对于成员函数，编译器会安排一个隐性参数self，所以对于mutating function：
```
struct Foo
{
    mutating func bar(args : Int) {}
}
```
编译器理解为：
```
func bar(inout self : Foo)(args : Int) {}
```

对于构造函数：
```
init(a : Int)
```
编译器理解为：
```
func init(class : Self.Type) ( a : Int) -> Self
```
因此构造函数的参数列表上总会有个fS0_这种形式


# Enum 的编码规则
Enum 分为两种，一种是Union Enum，一种是 Raw-Values Enum。这两种的成员极其对应的符号为：
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
根据对比可以得知Union Enum的普通case（不带tuple的）和Raw Values Enum的case，编译器是一样对待的。都生成的是等价的符号：
```
static func A() -> ENUM
```
而带tuple的，则多了一组参数而已：
```
static func B(arg : Int) -> ENUM
```



# 操作符重载的规则
参考代码：
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
可以看出以下规律：

- o 表示是一个操作符重载的函数
- i 表示是一个中缀函数
- p 表示前缀函数
- P 表示后缀函数

操作符编码关系表：

操作符| 缩写
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


# Protocol Witness table 符号规则
在Swift中，有一个很方便的一点是支持extension，现代语言中swift不是第一个发明这个概念的东西，scala trait，C# extension都是提供类似功能的概念。不过Swift的extension和C#不同的是，可以给现有类型加上protocol，这是个很方便很贴心的功能，这一点倒是更像golang的interface，只是需要明确的声明一下。
参考下面的代码
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
SIL结果中得知可以看到这个函数生成的签名里有个E和之前说到的标志都不一样，那明显就是Extension的缩写了。除此之外签名的其他地方编码都是之前描述过的。

除了这个以外，编译器还额外的生成了一个protocol witness，关于protocol witness，我后面会在深入SIL系列中单独开篇细讲，这里就只做符合主题的事情，即描述name mangling规则。

从名字可以看到，_T后面有个TW，W很容易猜到是Witness缩写，T还不太清楚，不符合之前描述的规则。后面就是extension所扩展的具体的类型，再后面接的Protocol名，FS0_5asInt 这个说这个函数是Int的 MyProtocol.asInt的witness，S0指向的第0个类型引用。US0___ 这是一个泛型定义，第一个S0_ 表示第一个泛型参数必须符合S0这个protocol，最后的fRQPS0_FT_Si则是这个函数的类型签名。 R即之前提到的reference，说明这是个inout参数，Q这里是表示泛型类型，不过表示的是.Self类型，后面接了S0表示是第0个类型引用的.Self类型，即这个参数可以描述为 inout a : MyProtocol.Self， P表示什么还不太清楚，通过实验发现去掉P不影响swift-demangle的解码结果。

有这里有三条疑点，但不妨碍我在swallow里做test case：

1. _T后面的T不太清楚表示什么
2. 这里的泛型函数只有一个泛型参数，但是却有三个下划线，按照之前的分析应该只有2个的，可能是和generic constraint有关，需要进一步分析
3. curried function参数列表里的P不知道表示什么意思。

# 最终总结
从上面的分析，可以总结出符号构成规则：  
_T 符号类型（F/v） 所属类型（C/O/P/V）?  模块名 类型名? 属性标志(g getter,s setter, w willSet W didSet)?  (P 模块hash) ?  符号名 符号类型
对应的编码程序的伪码为：
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

# 疑问
目前为止，还有两个地方不太明确无法做到和官方版本一致，第一个是计算私有成员的模块hash，刚开始估计以为这个hash的函数为 h(x)=md5(f(x))，因此只需要md5逆向得出f(x)就差不多可以弄明白f的规则了，然后用GPU计算MD5暴力破解，算了两天没算出来，说明这个hash函数不是那个形式，倒有可能是h(x)=g(md5(f(x)))。
第二个不明确的地方是Punycode的编码，这个也有空慢慢分析，我的test case里这两点就先用简单的形式来处理，后期等大神分析出来了我再来改我的编译器的实现，或者我自己有空再来汇编级跟踪一下看看。
