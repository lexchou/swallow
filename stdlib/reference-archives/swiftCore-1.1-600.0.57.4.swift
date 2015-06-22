infix operator < {
    associativity none
    precedence 130
}

infix operator += {
    associativity right
    precedence 90
    assignment
}

infix operator ... {
    associativity none
    precedence 135
}

infix operator -= {
    associativity right
    precedence 90
    assignment
}

infix operator &% {
    associativity left
    precedence 150
}

infix operator && {
    associativity left
    precedence 120
}

infix operator /= {
    associativity right
    precedence 90
    assignment
}

infix operator &* {
    associativity left
    precedence 150
}

infix operator &+ {
    associativity left
    precedence 140
}

infix operator &- {
    associativity left
    precedence 140
}

infix operator &/ {
    associativity left
    precedence 150
}

infix operator === {
    associativity none
    precedence 130
}

infix operator ..< {
    associativity none
    precedence 135
}

infix operator == {
    associativity none
    precedence 130
}

infix operator ^= {
    associativity right
    precedence 90
    assignment
}

infix operator ?? {
    associativity right
    precedence 110
}

infix operator ^ {
    associativity left
    precedence 140
}

infix operator >> {
    associativity none
    precedence 160
}

infix operator &= {
    associativity right
    precedence 90
    assignment
}

infix operator % {
    associativity left
    precedence 150
}

infix operator & {
    associativity left
    precedence 150
}

infix operator *= {
    associativity right
    precedence 90
    assignment
}

infix operator * {
    associativity left
    precedence 150
}

infix operator | {
    associativity left
    precedence 140
}

infix operator + {
    associativity left
    precedence 140
}

infix operator ~> {
    associativity left
    precedence 255
}

infix operator - {
    associativity left
    precedence 140
}

infix operator / {
    associativity left
    precedence 150
}

infix operator <<= {
    associativity right
    precedence 90
    assignment
}

infix operator >= {
    associativity none
    precedence 130
}

infix operator !== {
    associativity none
    precedence 130
}

infix operator << {
    associativity none
    precedence 160
}

infix operator || {
    associativity left
    precedence 110
}

infix operator |= {
    associativity right
    precedence 90
    assignment
}

infix operator <= {
    associativity none
    precedence 130
}

infix operator ~= {
    associativity none
    precedence 130
}

infix operator %= {
    associativity right
    precedence 90
    assignment
}

infix operator != {
    associativity none
    precedence 130
}

infix operator > {
    associativity none
    precedence 130
}

infix operator >>= {
    associativity right
    precedence 90
    assignment
}

prefix operator - {
}

prefix operator ~ {
}

prefix operator -- {
}

prefix operator ++ {
}

prefix operator + {
}

prefix operator ! {
}

postfix operator ++ {
}

postfix operator -- {
}

prefix func !(a: Bool) -> Bool


/// Return the result of inverting `a`\ 's logic value
prefix func !<T : BooleanType>(a: T) -> Bool

func !=(lhs: Int, rhs: Int) -> Bool

func !=<T : Equatable>(lhs: T?, rhs: T?) -> Bool

func !=<T>(lhs: T?, rhs: _OptionalNilComparisonType) -> Bool

func !=(lhs: UInt, rhs: UInt) -> Bool

func !=(lhs: Int64, rhs: Int64) -> Bool

func !=(lhs: UInt64, rhs: UInt64) -> Bool

func !=(lhs: Int32, rhs: Int32) -> Bool

func !=(lhs: UInt32, rhs: UInt32) -> Bool

func !=(lhs: Int16, rhs: Int16) -> Bool

func !=(lhs: UInt16, rhs: UInt16) -> Bool

func !=(lhs: Int8, rhs: Int8) -> Bool

func !=(lhs: UInt8, rhs: UInt8) -> Bool

func !=<T>(lhs: _OptionalNilComparisonType, rhs: T?) -> Bool

func !=<T : Equatable>(lhs: T, rhs: T) -> Bool


/// Returns true if the arrays do not contain the same elements.
func !=<T : Equatable>(lhs: ContiguousArray<T>, rhs: ContiguousArray<T>) -> Bool


/// Returns true if the arrays do not contain the same elements.
func !=<T : Equatable>(lhs: Slice<T>, rhs: Slice<T>) -> Bool


/// Returns true if the arrays do not contain the same elements.
func !=<T : Equatable>(lhs: [T], rhs: [T]) -> Bool

func !=<Key : Equatable, Value : Equatable>(lhs: [Key : Value], rhs: [Key : Value]) -> Bool

func !==(lhs: AnyObject?, rhs: AnyObject?) -> Bool

func %(lhs: UInt32, rhs: UInt32) -> UInt32

func %(lhs: Int16, rhs: Int16) -> Int16

func %(lhs: UInt16, rhs: UInt16) -> UInt16

func %(lhs: Int8, rhs: Int8) -> Int8

func %(lhs: UInt8, rhs: UInt8) -> UInt8


/// Divide `lhs` and `rhs`, returning the remainder and trapping in case of
/// arithmetic overflow (except in -Ounchecked builds).
func %<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

func %(lhs: Float80, rhs: Float80) -> Float80

func %(lhs: Double, rhs: Double) -> Double

func %(lhs: Float, rhs: Float) -> Float

func %(lhs: Int, rhs: Int) -> Int

func %(lhs: Int32, rhs: Int32) -> Int32

func %(lhs: UInt64, rhs: UInt64) -> UInt64

func %(lhs: Int64, rhs: Int64) -> Int64

func %(lhs: UInt, rhs: UInt) -> UInt

func %=(inout lhs: Float, rhs: Float)

func %=(inout lhs: Double, rhs: Double)

func %=(inout lhs: Float80, rhs: Float80)


/// remainder `lhs` and `rhs` and store the result in `lhs`, trapping in
/// case of arithmetic overflow (except in -Ounchecked builds).
func %=<T : _IntegerArithmeticType>(inout lhs: T, rhs: T)

func &(lhs: UInt32, rhs: UInt32) -> UInt32

func &(lhs: UInt16, rhs: UInt16) -> UInt16

func &(lhs: Int16, rhs: Int16) -> Int16

func &(lhs: Int32, rhs: Int32) -> Int32

func &(lhs: UInt64, rhs: UInt64) -> UInt64

func &(lhs: Int64, rhs: Int64) -> Int64

func &(lhs: UInt, rhs: UInt) -> UInt

func &(lhs: Int, rhs: Int) -> Int

func &(lhs: Bool, rhs: Bool) -> Bool

func &<T : _RawOptionSetType>(a: T, b: T) -> T

func &(lhs: UInt8, rhs: UInt8) -> UInt8

func &(lhs: Int8, rhs: Int8) -> Int8


/// remainder `lhs` and `rhs`, silently discarding any overflow.
func &%<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

func &&<T : BooleanType>(lhs: T, rhs: @autoclosure () -> Bool) -> Bool


/// If `lhs` is `false`, return it.  Otherwise, evaluate `rhs` and
/// return its `boolValue`.
@inline(__always) func &&<T : BooleanType, U : BooleanType>(lhs: T, rhs: @autoclosure () -> U) -> Bool


/// multiply `lhs` and `rhs`, silently discarding any overflow.
func &*<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T


/// add `lhs` and `rhs`, silently discarding any overflow.
func &+<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T


/// subtract `lhs` and `rhs`, silently discarding any overflow.
func &-<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T


/// divide `lhs` and `rhs`, silently discarding any overflow.
func &/<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

func &=(inout lhs: Bool, rhs: Bool)

func &=(inout lhs: UInt8, rhs: UInt8)

func &=(inout lhs: Int8, rhs: Int8)

func &=(inout lhs: UInt16, rhs: UInt16)

func &=(inout lhs: Int16, rhs: Int16)

func &=(inout lhs: UInt32, rhs: UInt32)

func &=(inout lhs: Int32, rhs: Int32)

func &=(inout lhs: UInt64, rhs: UInt64)

func &=<T : BitwiseOperationsType>(inout lhs: T, rhs: T)

func &=(inout lhs: Int, rhs: Int)

func &=(inout lhs: UInt, rhs: UInt)

func &=(inout lhs: Int64, rhs: Int64)

func *(lhs: UInt32, rhs: UInt32) -> UInt32

func *(lhs: Int32, rhs: Int32) -> Int32

func *(lhs: UInt64, rhs: UInt64) -> UInt64

func *(lhs: Int64, rhs: Int64) -> Int64

func *(lhs: UInt, rhs: UInt) -> UInt

func *(lhs: Int, rhs: Int) -> Int

func *(lhs: Float, rhs: Float) -> Float

func *(lhs: Double, rhs: Double) -> Double

func *(lhs: Float80, rhs: Float80) -> Float80


/// Multiply `lhs` and `rhs`, returning a result and trapping in case of
/// arithmetic overflow (except in -Ounchecked builds).
func *<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

func *(lhs: Int16, rhs: Int16) -> Int16

func *(lhs: UInt16, rhs: UInt16) -> UInt16

func *(lhs: Int8, rhs: Int8) -> Int8

func *(lhs: UInt8, rhs: UInt8) -> UInt8

func *=(inout lhs: UInt8, rhs: UInt8)

func *=(inout lhs: Int8, rhs: Int8)

func *=(inout lhs: UInt16, rhs: UInt16)

func *=(inout lhs: Int16, rhs: Int16)

func *=(inout lhs: UInt32, rhs: UInt32)

func *=(inout lhs: Int32, rhs: Int32)

func *=(inout lhs: UInt64, rhs: UInt64)

func *=(inout lhs: Int64, rhs: Int64)

func *=(inout lhs: UInt, rhs: UInt)

func *=(inout lhs: Int, rhs: Int)

func *=(inout lhs: Float, rhs: Float)

func *=(inout lhs: Double, rhs: Double)


/// multiply `lhs` and `rhs` and store the result in `lhs`, trapping in
/// case of arithmetic overflow (except in -Ounchecked builds).
func *=<T : _IntegerArithmeticType>(inout lhs: T, rhs: T)

func *=(inout lhs: Float80, rhs: Float80)

prefix func +(x: Double) -> Double

func +(lhs: Float, rhs: Float) -> Float

func +<T>(lhs: Int, rhs: UnsafePointer<T>) -> UnsafePointer<T>

prefix func +(x: Float) -> Float

func +(lhs: Int, rhs: Int) -> Int

func +<T>(lhs: UnsafePointer<T>, rhs: Int) -> UnsafePointer<T>

func +(lhs: UInt, rhs: UInt) -> UInt

func +(lhs: Int64, rhs: Int64) -> Int64

func +(lhs: UInt64, rhs: UInt64) -> UInt64

func +<T>(lhs: Int, rhs: UnsafeMutablePointer<T>) -> UnsafeMutablePointer<T>

func +<T>(lhs: UnsafeMutablePointer<T>, rhs: Int) -> UnsafeMutablePointer<T>

func +(lhs: Int32, rhs: Int32) -> Int32

func +(lhs: UInt32, rhs: UInt32) -> UInt32

func +(lhs: Int16, rhs: Int16) -> Int16

func +(lhs: UInt16, rhs: UInt16) -> UInt16

func +(lhs: Int8, rhs: Int8) -> Int8

func +(lhs: UInt8, rhs: UInt8) -> UInt8

func +(lhs: Double, rhs: Double) -> Double

func +(lhs: String, rhs: String) -> String

prefix func +(x: Float80) -> Float80

func +(lhs: Float80, rhs: Float80) -> Float80


/// Add `lhs` and `rhs`, returning a result and trapping in case of
/// arithmetic overflow (except in -Ounchecked builds).
func +<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

prefix func +<T : _SignedNumberType>(x: T) -> T

func +<C : _ExtensibleCollectionType, S : SequenceType where S.Generator.Element == S.Generator.Element>(lhs: C, rhs: S) -> C

func +<C : _ExtensibleCollectionType, S : SequenceType where S.Generator.Element == S.Generator.Element>(lhs: S, rhs: C) -> C

func +<T : Strideable>(lhs: T.Stride, rhs: T) -> T

func +<C : _ExtensibleCollectionType, S : CollectionType where S.Generator.Element == S.Generator.Element>(lhs: C, rhs: S) -> C

func +<EC1 : _ExtensibleCollectionType, EC2 : _ExtensibleCollectionType where EC1.Generator.Element == EC1.Generator.Element>(lhs: EC1, rhs: EC2) -> EC1

func +<T : Strideable>(lhs: T, rhs: T.Stride) -> T

prefix func ++(inout x: Int8) -> Int8

prefix func ++(inout x: Int16) -> Int16

postfix func ++<T : _Incrementable>(inout x: T) -> T

prefix func ++<T : _Incrementable>(inout x: T) -> T

postfix func ++(inout lhs: Float80) -> Float80

prefix func ++(inout rhs: Float80) -> Float80

postfix func ++(inout lhs: Double) -> Double

prefix func ++(inout rhs: Double) -> Double

postfix func ++(inout lhs: Float) -> Float

prefix func ++(inout rhs: Float) -> Float

postfix func ++(inout x: Int) -> Int

prefix func ++(inout x: Int) -> Int

postfix func ++(inout x: UInt) -> UInt

prefix func ++(inout x: UInt) -> UInt

postfix func ++(inout x: Int64) -> Int64

prefix func ++(inout x: Int64) -> Int64

postfix func ++(inout x: UInt64) -> UInt64

prefix func ++(inout x: UInt64) -> UInt64

postfix func ++(inout x: Int32) -> Int32

prefix func ++(inout x: Int32) -> Int32

postfix func ++(inout x: UInt32) -> UInt32

prefix func ++(inout x: UInt32) -> UInt32

postfix func ++(inout x: Int16) -> Int16

postfix func ++(inout x: UInt16) -> UInt16

prefix func ++(inout x: UInt16) -> UInt16

postfix func ++(inout x: Int8) -> Int8

prefix func ++(inout x: UInt8) -> UInt8

postfix func ++(inout x: UInt8) -> UInt8


/// Extend `lhs` with the elements of `rhs`
func +=<T, S : SequenceType where T == T>(inout lhs: ContiguousArray<T>, rhs: S)

func +=(inout lhs: UInt8, rhs: UInt8)


/// Extend `lhs` with the elements of `rhs`
func +=<T, C : CollectionType where T == T>(inout lhs: ContiguousArray<T>, rhs: C)

func +=<T>(inout lhs: UnsafeMutablePointer<T>, rhs: Int)

func +=(inout lhs: String, rhs: String)

func +=<T : Strideable>(inout lhs: T, rhs: T.Stride)


/// add `lhs` and `rhs` and store the result in `lhs`, trapping in
/// case of arithmetic overflow (except in -Ounchecked builds).
func +=<T : _IntegerArithmeticType>(inout lhs: T, rhs: T)

func +=(inout lhs: Float80, rhs: Float80)

func +=(inout lhs: Double, rhs: Double)

func +=(inout lhs: Float, rhs: Float)

func +=(inout lhs: Int, rhs: Int)

func +=(inout lhs: UInt, rhs: UInt)

func +=(inout lhs: Int64, rhs: Int64)


/// Extend `lhs` with the elements of `rhs`
func +=<T, S : SequenceType where T == T>(inout lhs: Slice<T>, rhs: S)


/// Extend `lhs` with the elements of `rhs`
func +=<T, C : CollectionType where T == T>(inout lhs: Slice<T>, rhs: C)

func +=(inout lhs: UInt64, rhs: UInt64)

func +=(inout lhs: Int32, rhs: Int32)

func +=(inout lhs: UInt32, rhs: UInt32)

func +=(inout lhs: Int16, rhs: Int16)


/// Extend `lhs` with the elements of `rhs`
func +=<T, S : SequenceType where T == T>(inout lhs: [T], rhs: S)


/// Extend `lhs` with the elements of `rhs`
func +=<T, C : CollectionType where T == T>(inout lhs: [T], rhs: C)


/// Append the elements of rhs to lhs
func +=<T, C : CollectionType where T == T>(inout lhs: _ContiguousArrayBuffer<T>, rhs: C)


/// Append rhs to lhs
func +=<T>(inout lhs: _ContiguousArrayBuffer<T>, rhs: T)

func +=(inout lhs: UInt16, rhs: UInt16)

func +=(inout lhs: Int8, rhs: Int8)

func +=<T>(inout lhs: UnsafePointer<T>, rhs: Int)

func -(lhs: UInt64, rhs: UInt64) -> UInt64

func -(lhs: Int64, rhs: Int64) -> Int64

func -(lhs: Int32, rhs: Int32) -> Int32

func -(lhs: UInt, rhs: UInt) -> UInt

func -(lhs: Int, rhs: Int) -> Int

prefix func -(x: Float) -> Float

func -(lhs: UInt32, rhs: UInt32) -> UInt32

func -(lhs: Int16, rhs: Int16) -> Int16

func -(lhs: UInt16, rhs: UInt16) -> UInt16

func -(lhs: Int8, rhs: Int8) -> Int8

func -(lhs: UInt8, rhs: UInt8) -> UInt8

func -<T>(lhs: UnsafePointer<T>, rhs: UnsafePointer<T>) -> Int

func -<T>(lhs: UnsafePointer<T>, rhs: Int) -> UnsafePointer<T>

func -<T>(lhs: UnsafeMutablePointer<T>, rhs: UnsafeMutablePointer<T>) -> Int

func -<T>(lhs: UnsafeMutablePointer<T>, rhs: Int) -> UnsafeMutablePointer<T>

func -<T : Strideable>(lhs: T, rhs: T) -> T.Stride

func -<T : Strideable>(lhs: T, rhs: T.Stride) -> T

prefix func -<T : _SignedNumberType>(x: T) -> T


/// Subtract `lhs` and `rhs`, returning a result and trapping in case of
/// arithmetic overflow (except in -Ounchecked builds).
func -<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

func -(lhs: Float80, rhs: Float80) -> Float80

prefix func -(x: Float80) -> Float80

func -(lhs: Double, rhs: Double) -> Double

prefix func -(x: Double) -> Double

func -(lhs: Float, rhs: Float) -> Float

prefix func --(inout x: Int) -> Int

postfix func --(inout lhs: Float) -> Float

postfix func --(inout x: UInt) -> UInt

prefix func --(inout x: UInt) -> UInt

prefix func --(inout rhs: Double) -> Double

postfix func --(inout x: Int64) -> Int64

prefix func --(inout x: Int64) -> Int64

postfix func --(inout lhs: Double) -> Double

postfix func --(inout x: UInt64) -> UInt64

prefix func --(inout rhs: Float80) -> Float80

prefix func --(inout x: UInt8) -> UInt8

postfix func --(inout lhs: Float80) -> Float80

prefix func --(inout x: UInt64) -> UInt64

postfix func --(inout x: UInt8) -> UInt8

prefix func --(inout x: Int8) -> Int8

postfix func --(inout x: Int) -> Int

postfix func --(inout x: Int32) -> Int32

prefix func --(inout x: Int32) -> Int32

postfix func --<T : _BidirectionalIndexType>(inout x: T) -> T

prefix func --(inout x: UInt32) -> UInt32

postfix func --(inout x: UInt32) -> UInt32

postfix func --(inout x: Int8) -> Int8

prefix func --(inout x: UInt16) -> UInt16

prefix func --<T : _BidirectionalIndexType>(inout x: T) -> T

postfix func --(inout x: UInt16) -> UInt16

prefix func --(inout x: Int16) -> Int16

postfix func --(inout x: Int16) -> Int16

prefix func --(inout rhs: Float) -> Float

func -=(inout lhs: UInt64, rhs: UInt64)

func -=(inout lhs: Int64, rhs: Int64)

func -=(inout lhs: UInt8, rhs: UInt8)

func -=(inout lhs: Int8, rhs: Int8)

func -=(inout lhs: UInt16, rhs: UInt16)

func -=<T>(inout lhs: UnsafePointer<T>, rhs: Int)

func -=(inout lhs: Int32, rhs: Int32)

func -=<T>(inout lhs: UnsafeMutablePointer<T>, rhs: Int)

func -=<T : Strideable>(inout lhs: T, rhs: T.Stride)


/// subtract `lhs` and `rhs` and store the result in `lhs`, trapping in
/// case of arithmetic overflow (except in -Ounchecked builds).
func -=<T : _IntegerArithmeticType>(inout lhs: T, rhs: T)

func -=(inout lhs: UInt32, rhs: UInt32)

func -=(inout lhs: Int16, rhs: Int16)

func -=(inout lhs: Float80, rhs: Float80)

func -=(inout lhs: Double, rhs: Double)

func -=(inout lhs: Float, rhs: Float)

func -=(inout lhs: UInt, rhs: UInt)

func -=(inout lhs: Int, rhs: Int)


/// Returns a closed interval from `start` through `end`
func ...<T : Comparable>(start: T, end: T) -> ClosedInterval<T>


/// Forms a closed range that contains both `start` and `end`.
/// Requres: `start <= end`
func ...<Pos : ForwardIndexType where Pos : Comparable>(start: Pos, end: Pos) -> Range<Pos>


/// Forms a closed range that contains both `minimum` and `maximum`.
func ...<Pos : ForwardIndexType>(minimum: Pos, maximum: Pos) -> Range<Pos>


/// Forms a half-open range that contains `start`, but not
/// `end`.  Requires: `start <= end`
func ..<<Pos : ForwardIndexType where Pos : Comparable>(start: Pos, end: Pos) -> Range<Pos>


/// Forms a half-open range that contains `minimum`, but not
/// `maximum`.
func ..<<Pos : ForwardIndexType>(minimum: Pos, maximum: Pos) -> Range<Pos>


/// Returns a half-open interval from `start` to `end`
func ..<<T : Comparable>(start: T, end: T) -> HalfOpenInterval<T>

func /(lhs: Float, rhs: Float) -> Float

func /(lhs: Double, rhs: Double) -> Double

func /(lhs: Float80, rhs: Float80) -> Float80


/// Divide `lhs` and `rhs`, returning a result and trapping in case of
/// arithmetic overflow (except in -Ounchecked builds).
func /<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T

func /(lhs: Int, rhs: Int) -> Int

func /(lhs: Int64, rhs: Int64) -> Int64

func /(lhs: UInt64, rhs: UInt64) -> UInt64

func /(lhs: Int32, rhs: Int32) -> Int32

func /(lhs: UInt32, rhs: UInt32) -> UInt32

func /(lhs: Int16, rhs: Int16) -> Int16

func /(lhs: UInt16, rhs: UInt16) -> UInt16

func /(lhs: Int8, rhs: Int8) -> Int8

func /(lhs: UInt8, rhs: UInt8) -> UInt8

func /(lhs: UInt, rhs: UInt) -> UInt

func /=(inout lhs: Float, rhs: Float)

func /=(inout lhs: Double, rhs: Double)

func /=(inout lhs: Float80, rhs: Float80)


/// divide `lhs` and `rhs` and store the result in `lhs`, trapping in
/// case of arithmetic overflow (except in -Ounchecked builds).
func /=<T : _IntegerArithmeticType>(inout lhs: T, rhs: T)

func <(lhs: String.UnicodeScalarView.Index, rhs: String.UnicodeScalarView.Index) -> Bool

func <(lhs: UInt, rhs: UInt) -> Bool

func <(lhs: String.Index, rhs: String.Index) -> Bool

func <(lhs: String, rhs: String) -> Bool


/// Compare two Strideables
func <<T : _Strideable>(x: T, y: T) -> Bool

func <<T : _Comparable>(lhs: T?, rhs: T?) -> Bool

func <(lhs: Float80, rhs: Float80) -> Bool

func <(lhs: Double, rhs: Double) -> Bool

func <(lhs: Float, rhs: Float) -> Bool

func <(lhs: Int, rhs: Int) -> Bool

func <(lhs: Int64, rhs: Int64) -> Bool

func <(lhs: UInt64, rhs: UInt64) -> Bool

func <(lhs: Int32, rhs: Int32) -> Bool

func <(lhs: UInt32, rhs: UInt32) -> Bool

func <(lhs: Int16, rhs: Int16) -> Bool

func <(lhs: UInt16, rhs: UInt16) -> Bool

func <(lhs: Int8, rhs: Int8) -> Bool

func <(lhs: UInt8, rhs: UInt8) -> Bool

func <<Key : Hashable, Value>(lhs: DictionaryIndex<Key, Value>, rhs: DictionaryIndex<Key, Value>) -> Bool

func <(lhs: Character, rhs: Character) -> Bool

func <(lhs: Bit, rhs: Bit) -> Bool

func <<T>(lhs: UnsafePointer<T>, rhs: UnsafePointer<T>) -> Bool

func <(lhs: UnicodeScalar, rhs: UnicodeScalar) -> Bool

func <<T>(lhs: UnsafeMutablePointer<T>, rhs: UnsafeMutablePointer<T>) -> Bool

func <<(lhs: UInt8, rhs: UInt8) -> UInt8

func <<(lhs: Int, rhs: Int) -> Int

func <<(lhs: UInt, rhs: UInt) -> UInt

func <<(lhs: Int64, rhs: Int64) -> Int64

func <<(lhs: Int8, rhs: Int8) -> Int8

func <<(lhs: UInt16, rhs: UInt16) -> UInt16

func <<(lhs: Int16, rhs: Int16) -> Int16

func <<(lhs: UInt64, rhs: UInt64) -> UInt64

func <<(lhs: Int32, rhs: Int32) -> Int32

func <<(lhs: UInt32, rhs: UInt32) -> UInt32

func <<=(inout lhs: UInt8, rhs: UInt8)

func <<=(inout lhs: Int8, rhs: Int8)

func <<=(inout lhs: UInt16, rhs: UInt16)

func <<=(inout lhs: Int16, rhs: Int16)

func <<=(inout lhs: UInt32, rhs: UInt32)

func <<=(inout lhs: Int32, rhs: Int32)

func <<=(inout lhs: UInt64, rhs: UInt64)

func <<=(inout lhs: Int64, rhs: Int64)

func <<=(inout lhs: UInt, rhs: UInt)

func <<=(inout lhs: Int, rhs: Int)

func <=(lhs: Int16, rhs: Int16) -> Bool

func <=<T : _Comparable>(lhs: T, rhs: T) -> Bool

func <=<T : _Comparable>(lhs: T?, rhs: T?) -> Bool

func <=(lhs: Int, rhs: Int) -> Bool

func <=(lhs: UInt, rhs: UInt) -> Bool

func <=(lhs: Int64, rhs: Int64) -> Bool

func <=(lhs: UInt64, rhs: UInt64) -> Bool

func <=(lhs: Int32, rhs: Int32) -> Bool

func <=(lhs: UInt32, rhs: UInt32) -> Bool

func <=(lhs: UInt16, rhs: UInt16) -> Bool

func <=(lhs: Int8, rhs: Int8) -> Bool

func <=(lhs: UInt8, rhs: UInt8) -> Bool

func ==(x: ObjectIdentifier, y: ObjectIdentifier) -> Bool

func ==<T>(lhs: UnsafePointer<T>, rhs: UnsafePointer<T>) -> Bool

func ==(lhs: UInt8, rhs: UInt8) -> Bool

func ==(lhs: Bit, rhs: Bit) -> Bool

func ==(lhs: Int8, rhs: Int8) -> Bool

func ==(lhs: UInt16, rhs: UInt16) -> Bool

func ==<Base : CollectionType>(lhs: FilterCollectionViewIndex<Base>, rhs: FilterCollectionViewIndex<Base>) -> Bool

func ==(lhs: Int16, rhs: Int16) -> Bool

func ==<Key : Equatable, Value : Equatable>(lhs: [Key : Value], rhs: [Key : Value]) -> Bool

func ==<Key : Hashable, Value>(lhs: DictionaryIndex<Key, Value>, rhs: DictionaryIndex<Key, Value>) -> Bool

func ==(lhs: UInt32, rhs: UInt32) -> Bool

func ==<T : _RawOptionSetType>(a: T, b: T) -> Bool

func ==(lhs: Character, rhs: Character) -> Bool

func ==(lhs: Int, rhs: Int) -> Bool

func ==(lhs: Int32, rhs: Int32) -> Bool

func ==(lhs: Float, rhs: Float) -> Bool

func ==<T>(lhs: CFunctionPointer<T>, rhs: CFunctionPointer<T>) -> Bool

func ==(lhs: Double, rhs: Double) -> Bool

func ==(lhs: COpaquePointer, rhs: COpaquePointer) -> Bool

func ==<I>(lhs: ReverseBidirectionalIndex<I>, rhs: ReverseBidirectionalIndex<I>) -> Bool

func ==<T>(lhs: AutoreleasingUnsafeMutablePointer<T>, rhs: AutoreleasingUnsafeMutablePointer<T>) -> Bool

func ==(lhs: Bool, rhs: Bool) -> Bool


/// Returns true if these arrays contain the same elements.
func ==<T : Equatable>(lhs: [T], rhs: [T]) -> Bool


/// Returns true if these arrays contain the same elements.
func ==<T : Equatable>(lhs: Slice<T>, rhs: Slice<T>) -> Bool


/// Returns true if these arrays contain the same elements.
func ==<T : Equatable>(lhs: ContiguousArray<T>, rhs: ContiguousArray<T>) -> Bool

func ==(lhs: UInt64, rhs: UInt64) -> Bool

func ==<I>(lhs: ReverseRandomAccessIndex<I>, rhs: ReverseRandomAccessIndex<I>) -> Bool

func ==(lhs: String.UTF8View.Index, rhs: String.UTF8View.Index) -> Bool

func ==<T : _Strideable>(x: T, y: T) -> Bool

func ==(lhs: String, rhs: String) -> Bool

func ==<T>(lhs: UnsafeMutablePointer<T>, rhs: UnsafeMutablePointer<T>) -> Bool

func ==(lhs: UnicodeScalar, rhs: UnicodeScalar) -> Bool

func ==(lhs: UInt, rhs: UInt) -> Bool

func ==(lhs: String.Index, rhs: String.Index) -> Bool

func ==(lhs: Int64, rhs: Int64) -> Bool

func ==<T>(lhs: Range<T>, rhs: Range<T>) -> Bool

func ==(lhs: String.UnicodeScalarView.Index, rhs: String.UnicodeScalarView.Index) -> Bool

func ==(lhs: Float80, rhs: Float80) -> Bool

func ==(lhs: FloatingPointClassification, rhs: FloatingPointClassification) -> Bool

func ==<Value, Element>(lhs: HeapBuffer<Value, Element>, rhs: HeapBuffer<Value, Element>) -> Bool


/// Two `HalfOpenInterval`\ s are equal if their `start` and `end` are
/// equal
func ==<T : Comparable>(lhs: HalfOpenInterval<T>, rhs: HalfOpenInterval<T>) -> Bool


/// Two `ClosedInterval`\ s are equal if their `start` and `end` are
/// equal
func ==<T : Comparable>(lhs: ClosedInterval<T>, rhs: ClosedInterval<T>) -> Bool

func ==<T : Equatable>(lhs: T?, rhs: T?) -> Bool

func ==<T>(lhs: T?, rhs: _OptionalNilComparisonType) -> Bool

func ==<T>(lhs: _OptionalNilComparisonType, rhs: T?) -> Bool

func ===(lhs: AnyObject?, rhs: AnyObject?) -> Bool

func ><T : _Comparable>(lhs: T?, rhs: T?) -> Bool

func >(lhs: Int, rhs: Int) -> Bool

func >(lhs: UInt, rhs: UInt) -> Bool

func ><T : _Comparable>(lhs: T, rhs: T) -> Bool

func >(lhs: Int64, rhs: Int64) -> Bool

func >(lhs: UInt64, rhs: UInt64) -> Bool

func >(lhs: Int32, rhs: Int32) -> Bool

func >(lhs: UInt32, rhs: UInt32) -> Bool

func >(lhs: UInt8, rhs: UInt8) -> Bool

func >(lhs: Int16, rhs: Int16) -> Bool

func >(lhs: UInt16, rhs: UInt16) -> Bool

func >(lhs: Int8, rhs: Int8) -> Bool

func >=(lhs: UInt8, rhs: UInt8) -> Bool

func >=(lhs: Int8, rhs: Int8) -> Bool

func >=(lhs: UInt16, rhs: UInt16) -> Bool

func >=(lhs: Int16, rhs: Int16) -> Bool

func >=(lhs: UInt32, rhs: UInt32) -> Bool

func >=(lhs: Int32, rhs: Int32) -> Bool

func >=(lhs: UInt64, rhs: UInt64) -> Bool

func >=(lhs: Int64, rhs: Int64) -> Bool

func >=(lhs: UInt, rhs: UInt) -> Bool

func >=(lhs: Int, rhs: Int) -> Bool

func >=<T : _Comparable>(lhs: T?, rhs: T?) -> Bool

func >=<T : _Comparable>(lhs: T, rhs: T) -> Bool

func >>(lhs: Int8, rhs: Int8) -> Int8

func >>(lhs: UInt8, rhs: UInt8) -> UInt8

func >>(lhs: Int, rhs: Int) -> Int

func >>(lhs: Int16, rhs: Int16) -> Int16

func >>(lhs: UInt16, rhs: UInt16) -> UInt16

func >>(lhs: UInt32, rhs: UInt32) -> UInt32

func >>(lhs: UInt, rhs: UInt) -> UInt

func >>(lhs: Int32, rhs: Int32) -> Int32

func >>(lhs: Int64, rhs: Int64) -> Int64

func >>(lhs: UInt64, rhs: UInt64) -> UInt64

func >>=(inout lhs: Int, rhs: Int)

func >>=(inout lhs: UInt16, rhs: UInt16)

func >>=(inout lhs: Int64, rhs: Int64)

func >>=(inout lhs: Int8, rhs: Int8)

func >>=(inout lhs: UInt8, rhs: UInt8)

func >>=(inout lhs: Int16, rhs: Int16)

func >>=(inout lhs: UInt, rhs: UInt)

func >>=(inout lhs: UInt64, rhs: UInt64)

func >>=(inout lhs: Int32, rhs: Int32)

func >>=(inout lhs: UInt32, rhs: UInt32)

func ??<T>(optional: T?, defaultValue: @autoclosure () -> T) -> T

func ??<T>(optional: T?, defaultValue: @autoclosure () -> T?) -> T?


/// A type that supports an "absolute value" function.
protocol AbsoluteValuable : SignedNumberType {

    /// Returns the absolute value of `x`
    class func abs(x: Self) -> Self
}


/// The protocol to which all types implicitly conform
typealias Any = protocol<>


/// The protocol to which all class types implicitly conform.
///
/// When used as a concrete type, all known `@objc` `class` methods and
/// properties are available, as implicitly-unwrapped-optional methods
/// and properties respectively, on each instance of `AnyClass`. For
/// example:
///
/// .. parsed-literal:
///
///   class C {
///     @objc class var cValue: Int { return 42 }
///   }
///
///   // If x has an @objc cValue: Int, return its value.  
///   // Otherwise, return nil.
///   func getCValue(x: AnyClass) -> Int? {
///     return **x.cValue**
///   }
///
/// See also: `AnyObject`
typealias AnyClass = AnyObject.Type


/// The protocol to which all classes implicitly conform.
///
/// When used as a concrete type, all known `@objc` methods and
/// properties are available, as implicitly-unwrapped-optional methods
/// and properties respectively, on each instance of `AnyObject`.  For
/// example:
///
/// .. parsed-literal:
///
///   class C {
///     @objc func getCValue() -> Int { return 42 }
///   }
///
///   // If x has a method @objc getValue()->Int, call it and
///   // return the result.  Otherwise, return nil.
///   func getCValue1(x: AnyObject) -> Int? {
///     if let f: ()->Int = **x.getCValue** {
///       return f()
///     }
///     return nil
///   }
///
///   // A more idiomatic implementation using "optional chaining"
///   func getCValue2(x: AnyObject) -> Int? {
///     return **x.getCValue?()**
///   }
///
///   // An implementation that assumes the required method is present
///   func getCValue3(x: AnyObject) -> **Int** {
///     return **x.getCValue()** // x.getCValue is implicitly unwrapped.
///   }
///
/// See also: `AnyClass`
@objc protocol AnyObject {
}


/// Conceptually_, `Array` is an efficient, tail-growable random-access
/// collection of arbitrary elements.
///
/// Common Properties of Array Types
/// ================================
///
/// The information in this section applies to all three of Swift's
/// array types, `Array<T>`, `ContiguousArray<T>`, and `Slice<T>`.
/// When you read the word "array" here in a normal typeface, it
/// applies to all three of them.
///
/// Value Semantics
/// ---------------
///
/// Each array variable, `let` binding, or stored property has an
/// independent value that includes the values of all of its elements.
/// Therefore, mutations to the array are not observable through its
/// copies::
///
///   var a = [1, 2, 3]
///   var b = a
///   b[0] = 4
///   println("a=\(a), b=\(b)")     // a=[1, 2, 3], b=[4, 2, 3]
///
/// (Of course, if the array stores `class` references, the objects
/// are shared; only the values of the references are independent)
///
/// Arrays use Copy-on-Write so that their storage and elements are
/// only copied lazily, upon mutation, when more than one array
/// instance is using the same buffer.  Therefore, the first in any
/// sequence of mutating operations may cost `O(N)` time and space,
/// where `N` is the length of the array.
///
/// Growth and Capacity
/// -------------------
///
/// When an array's contiguous storage fills up, new storage must be
/// allocated and elements must be moved to the new storage.  `Array`,
/// `ContiguousArray`, and `Slice` share an exponential growth
/// strategy that makes `append` a constant time operation *when
/// amortized over many invocations*.  In addition to a `count`
/// property, these array types have a `capacity` that reflects their
/// potential to store elements without reallocation, and when you
/// know how many elements you'll store, you can call
/// `reserveCapacity` to pre-emptively reallocate and prevent
/// intermediate reallocations.
///
/// .. _Conceptually:
///
/// Objective-C Bridge
/// ==================
///
/// The main distinction between `Array` and the other array types is
/// that it interoperates seamlessly and efficiently with Objective-C.
///
/// `Array<T>` is considered bridged to Objective-C iff `T` is bridged
/// to Objective-C.
///
/// When `T` is a `class` or `@objc` protocol type, `Array` may store
/// its elements in an `NSArray`.  Since any arbitrary subclass of
/// `NSArray` can become an `Array`, there are no guarantees about
/// representation or efficiency in this case (see also
/// `ContiguousArray`).  Since `NSArray` is immutable, it is just as
/// though the storage was shared by some copy: the first in any
/// sequence of mutating operations causes elements to be copied into
/// unique, contiguous storage which may cost `O(N)` time and space,
/// where `N` is the length of the array (or more, if the underlying
/// `NSArray` is has unusual performance characteristics).
///
/// Bridging to Objective-C
/// -----------------------
///
/// Any bridged `Array` can be implicitly converted to an `NSArray`.
/// When `T` is a `class` or `@objc` protocol, bridging takes O(1)
/// time and O(1) space.  Other `Array`\ s must be bridged
/// element-by-element, allocating a new object for each element, at a
/// cost of at least O(`count`) time and space.
///
/// Bridging from Objective-C
/// -------------------------
///
/// An `NSArray` can be implicitly or explicitly converted to any
/// bridged `Array<T>`.  This conversion calls `copyWithZone` on the
/// `NSArray`, to ensure it won't be modified, and stores the result
/// in the `Array`.  Type-checking, to ensure the `NSArray`\ 's
/// elements match or can be bridged to `T`, is deferred until the
/// first element access.
struct Array<T> : MutableCollectionType, Sliceable {

    /// The type of element stored by this `Array`
    typealias Element = T

    /// Always zero, which is the index of the first element when non-empty.
    var startIndex: Int { get }

    /// A "past-the-end" element index; the successor of the last valid
    /// subscript argument.
    var endIndex: Int { get }
    subscript (index: Int) -> T

    /// Return a *generator* over the elements.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<[T]>

    /// A type that can represent a sub-range of an `Array` 
    typealias SubSlice = Slice<T>
    subscript (subRange: Range<Int>) -> Slice<T>

    /// Initialization from an existing buffer does not have "array.init"
    /// semantics because the caller may retain an alias to buffer.
    init(_ buffer: _ArrayBuffer<T>)
}

extension Array : ArrayLiteralConvertible {

    /// Create an instance containing `elements`.
    init(arrayLiteral elements: T...)
}



extension Array {

    /// Construct an empty Array
    init()

    /// Construct from an arbitrary sequence with elements of type `T`
    init<S : SequenceType where T == T>(_ s: S)

    /// Construct a Array of `count` elements, each initialized to
    /// `repeatedValue`.
    init(count: Int, repeatedValue: T)

    /// How many elements the Array stores
    var count: Int { get }

    /// How many elements the `Array` can store without reallocation
    var capacity: Int { get }

    /// `true` if and only if the `Array` is empty
    var isEmpty: Bool { get }

    /// The first element, or `nil` if the array is empty
    var first: T? { get }

    /// The last element, or `nil` if the array is empty
    var last: T? { get }

    /// Reserve enough space to store minimumCapacity elements.
    ///
    /// PostCondition: `capacity >= minimumCapacity` and the array has
    /// mutable contiguous storage.
    ///
    /// Complexity: O(`count`)
    mutating func reserveCapacity(minimumCapacity: Int)

    /// Append newElement to the Array
    ///
    /// Complexity: amortized O(1) unless `self`'s storage is shared with another live array; O(`count`) if `self` does not wrap a bridged `NSArray`; otherwise the efficiency is unspecified.
    mutating func append(newElement: T)

    /// Append the elements of `newElements` to `self`.
    ///
    /// Complexity: O(*length of result*) 
    /// 
    mutating func extend<S : SequenceType where T == T>(newElements: S)

    /// Remove an element from the end of the Array in O(1).
    /// Requires: count > 0
    mutating func removeLast() -> T

    /// Insert `newElement` at index `i`.
    ///
    /// Requires: `i <= count`
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func insert(newElement: T, atIndex i: Int)

    /// Remove and return the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAtIndex(index: Int) -> T

    /// Remove all elements.
    ///
    /// Postcondition: `capacity == 0` iff `keepCapacity` is `false`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeAll(keepCapacity: Bool = default)

    /// Interpose `self` between each consecutive pair of `elements`,
    /// and concatenate the elements of the resulting sequence.  For
    /// example, `[-1, -2].join([[1, 2, 3], [4, 5, 6], [7, 8, 9]])`
    /// yields `[1, 2, 3, -1, -2, 4, 5, 6, -1, -2, 7, 8, 9]`
    func join<S : SequenceType where [T] == [T]>(elements: S) -> [T]

    /// Return the result of repeatedly calling `combine` with an
    /// accumulated value initialized to `initial` and each element of
    /// `self`, in turn, i.e. return
    /// `combine(combine(...combine(combine(initial, self[0]),
    /// self[1]),...self[count-2]), self[count-1])`.
    func reduce<U>(initial: U, combine: (U, T) -> U) -> U

    /// Sort `self` in-place according to `isOrderedBefore`.  Requires:
    /// `isOrderedBefore` induces a `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    mutating func sort(isOrderedBefore: (T, T) -> Bool)

    /// Return a copy of `self` that has been sorted according to
    /// `isOrderedBefore`.  Requires: `isOrderedBefore` induces a
    /// `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    func sorted(isOrderedBefore: (T, T) -> Bool) -> [T]

    /// Return an `Array` containing the results of calling
    /// `transform(x)` on each element `x` of `self`
    func map<U>(transform: (T) -> U) -> [U]

    /// A Array containing the elements of `self` in reverse order
    func reverse() -> [T]

    /// Return an `Array` containing the elements `x` of `self` for which
    /// `includeElement(x)` is `true`
    func filter(includeElement: (T) -> Bool) -> [T]
}

extension Array : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Array : Printable, DebugPrintable {

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}



extension Array {

    /// Call `body(p)`, where `p` is a pointer to the `Array`\ 's
    /// contiguous storage. If no such storage exists, it is first created.
    ///
    /// Often, the optimizer can eliminate bounds checks within an
    /// array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    func withUnsafeBufferPointer<R>(body: (UnsafeBufferPointer<T>) -> R) -> R

    /// Call `body(p)`, where `p` is a pointer to the `Array`\ 's
    /// mutable contiguous storage. If no such storage exists, it is first created.
    ///
    /// Often, the optimizer can eliminate bounds- and uniqueness-checks
    /// within an array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    mutating func withUnsafeMutableBufferPointer<R>(body: (inout UnsafeMutableBufferPointer<T>) -> R) -> R
}

extension Array {

    /// This function "seeds" the ArrayLiteralConvertible protocol
    static func convertFromHeapArray(base: Builtin.RawPointer, owner: Builtin.NativeObject, count: Builtin.Word) -> [T]
}

extension Array {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if `subRange.endIndex
    /// == self.endIndex` and `isEmpty(newElements)`\ , O(N) otherwise.
    mutating func replaceRange<C : CollectionType where T == T>(subRange: Range<Int>, with newElements: C)

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count + countElements(newElements)`\ ).
    mutating func splice<S : CollectionType where T == T>(newElements: S, atIndex i: Int)

    /// Remove the indicated `subRange` of elements
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeRange(subRange: Range<Int>)
}

extension Array {

    /// Construct from the given `_SwiftNSArrayRequiredOverridesType`.
    ///
    /// If `noCopy` is `true`, either `source` must be known to be immutable,
    /// or the resulting / `Array` must not survive across code that could mutate
    /// `source`.
    init(_fromCocoaArray source: _SwiftNSArrayRequiredOverridesType, noCopy: Bool = default)
}


/// Conforming types can be initialized with array literals
protocol ArrayLiteralConvertible {
    typealias Element

    /// Create an instance initialized with `elements`.
    init(arrayLiteral elements: Element...)
}


/// A mutable pointer-to-ObjC-pointer argument.
///
/// This type has implicit conversions to allow passing any of the following
/// to a C or ObjC API:
///
/// - 'nil', which gets passed as a null pointer,
/// - an inout argument of the referenced type, which gets passed as a pointer
///   to a writeback temporary with autoreleasing ownership semantics,
/// - an UnsafeMutablePointer<T>, which is passed as-is.
///
/// Passing pointers to mutable arrays of ObjC class pointers is not
/// directly supported. Unlike UnsafeMutablePointer<T>,
/// AutoreleasingUnsafeMutablePointer must reference storage that does
/// not own a reference count to the referenced
/// value. UnsafeMutablePointer's operations, by contrast, assume that
/// the referenced storage owns values loaded from or stored to it.
///
/// This type does not carry an owner pointer unlike the other C*Pointer types
/// because it only needs to reference the results of inout conversions, which
/// already have writeback-scoped lifetime.
struct AutoreleasingUnsafeMutablePointer<T> : Equatable, NilLiteralConvertible {

    /// Access the underlying raw memory, getting and
    /// setting values.
    var memory: T { get nonmutating set }
    subscript (i: Int) -> T { get }

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())

    /// Returns `nil`
    static func null() -> AutoreleasingUnsafeMutablePointer<T>

    /// Initialize to a null pointer.
    init()

    /// Explicit construction from an UnsafeMutablePointer.
    ///
    /// This is inherently unsafe; UnsafeMutablePointer assumes the
    /// referenced memory has +1 strong ownership semantics, whereas
    /// AutoreleasingUnsafeMutablePointer implies +0 semantics.
    init<U>(_ ptr: UnsafeMutablePointer<U>)
}

extension AutoreleasingUnsafeMutablePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// An *index* that can step backwards via application of its
/// `predecessor()` method.
protocol BidirectionalIndexType : ForwardIndexType, _BidirectionalIndexType {
}


/// The lazy `CollectionType` returned by `reverse(c)` where `c` is a
/// `CollectionType` with an `Index` conforming to `BidirectionalIndexType`
struct BidirectionalReverseView<T : CollectionType where T.Index : BidirectionalIndexType> : CollectionType {

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = ReverseBidirectionalIndex<T.Index>

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = IndexingGenerator<BidirectionalReverseView<T>>

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<BidirectionalReverseView<T>>

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Index { get }
    subscript (position: Index) -> T.Generator.Element { get }
}


/// A `RandomAccessIndexType` that has two possible values.  Used as
/// the `Index` type for `SequenceOfOne<T>`.
enum Bit : Int, RandomAccessIndexType, Reflectable {
    case Zero
    case One

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: `self == .Zero`.
    func successor() -> Bit

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: `self != .Zero`.
    func predecessor() -> Bit

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Bit) -> Int

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(distance: Int) -> Bit

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Bit : IntegerArithmeticType {

    /// Add `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    static func addWithOverflow(lhs: Bit, _ rhs: Bit) -> (Bit, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    static func subtractWithOverflow(lhs: Bit, _ rhs: Bit) -> (Bit, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    static func multiplyWithOverflow(lhs: Bit, _ rhs: Bit) -> (Bit, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Bit, _ rhs: Bit) -> (Bit, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning the remainder and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Bit, _ rhs: Bit) -> (Bit, overflow: Bool)

    /// Represent this number using Swift's widest native signed integer
    /// type.
    func toIntMax() -> IntMax
}


/// A set type with O(1) standard bitwise operators.
///
/// Each instance is a subset of `~Self.allZeros`
///
/// Axioms, where `x` is an instance of `Self`::
///
///   x | Self.allZeros == x
///   x ^ Self.allZeros == x
///   x & Self.allZeros == .allZeros
///   x & ~Self.allZeros == x
///   ~x == x ^ ~Self.allZeros
protocol BitwiseOperationsType {

    /// Returns the intersection of bits set in `lhs` and `rhs`.
    ///
    /// Complexity: O(1)
    func &(lhs: Self, rhs: Self) -> Self

    /// Returns the union of bits set in `lhs` and `rhs`
    ///
    /// Complexity: O(1)
    func |(lhs: Self, rhs: Self) -> Self

    /// Returns the bits that are set in exactly one of `lhs` and `rhs`
    ///
    /// Complexity: O(1)
    func ^(lhs: Self, rhs: Self) -> Self

    /// Returns `x ^ ~Self.allZeros`
    ///
    /// Complexity: O(1)
    prefix func ~(x: Self) -> Self

    /// The empty bitset.
    ///
    /// Also the `identity element
    /// <http://en.wikipedia.org/wiki/Identity_element>`_ for `|` and
    /// `^`, and the `fixed point
    /// <http://en.wikipedia.org/wiki/Fixed_point_(mathematics)>`_ for
    /// `&`.
    class var allZeros: Self { get }
}


/// A value type whose instances are either `true` or `false`.
struct Bool {

    /// Default-initialize Boolean value to `false`.
    init()
}

extension Bool : BooleanLiteralConvertible {
    init(_builtinBooleanLiteral value: Builtin.Int1)

    /// Create an instance initialized to `value`.
    init(booleanLiteral value: Bool)
}

extension Bool : BooleanType {

    /// Identical to `self`.
    var boolValue: Bool { get }

    /// Construct an instance representing the same logical value as
    /// `value`
    init<T : BooleanType>(_ value: T)
}

extension Bool : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Bool : Equatable, Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Bool : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// Conforming types can be initialized with the boolean literals
/// `true` and `false`.
protocol BooleanLiteralConvertible {
    typealias BooleanLiteralType

    /// Create an instance initialized to `value`.
    init(booleanLiteral value: BooleanLiteralType)
}


/// The default type for an otherwise-unconstrained boolean literal
typealias BooleanLiteralType = Bool


/// A type that represents a boolean value.
///
/// Types that conform to the `BooleanType` protocol can be used as
/// the condition in control statements (`if`, `while`, C-style `for`)
/// and other logical value contexts (e.g., `case` statement guards).
///
/// Only two types provided by Swift, `Bool` and `ObjCBool`, conform
/// to `BooleanType`. Expanding this set to include types that
/// represent more than simple boolean values is discouraged.
protocol BooleanType {

    /// The value of `self`, expressed as a `Bool`.
    var boolValue: Bool { get }
}


/// The C '_Bool' and C++ 'bool' type.
typealias CBool = Bool


/// The C 'char' type.
///
/// This will be the same as either `CSignedChar` (in the common
/// case) or `CUnsignedChar`, depending on the platform.
typealias CChar = Int8


/// The C++11 'char16_t' type, which has UTF-16 encoding.
typealias CChar16 = UInt16


/// The C++11 'char32_t' type, which has UTF-32 encoding.
typealias CChar32 = UnicodeScalar


/// The C 'double' type.
typealias CDouble = Double


/// The C 'float' type.
typealias CFloat = Float


/// The family of C function pointer types.
///
/// In imported APIs, `T` is a Swift function type such as
/// `(Int)->String`.
/// 
/// Though not directly useful in Swift, `CFunctionPointer<T>` can be
/// used to safely pass a C function pointer, received from one C or
/// Objective-C API, to another C or Objective-C API.
struct CFunctionPointer<T> : Equatable, Hashable, NilLiteralConvertible {

    /// Create a `nil` instance.
    init()

    /// Reinterpret the bits of `value` as `CFunctionPointer<T>`.
    ///
    /// .. Warning:: This is a fundamentally unsafe operation, equivalent to
    ///      `unsafeBitCast(value, CFunctionPointer<T>.self)`
    init(_ value: COpaquePointer)

    /// Returns `nil`
    static func null() -> CFunctionPointer<T>

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())
}

extension CFunctionPointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// The C 'int' type.
typealias CInt = Int32


/// The C 'long' type.
typealias CLong = Int


/// The C 'long long' type.
typealias CLongLong = Int64


/// A wrapper around an opaque C pointer.
///
/// Opaque pointers are used to represent C pointers to types that
/// cannot be represented in Swift, such as incomplete struct types.
struct COpaquePointer : Equatable, Hashable, NilLiteralConvertible {

    /// Construct a `nil` instance.
    init()

    /// Construct a `COpaquePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: Word)

    /// Construct a `COpaquePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: UWord)

    /// Convert a typed `UnsafePointer` to an opaque C pointer.
    init<T>(_ value: UnsafePointer<T>)

    /// Convert a typed `UnsafeMutablePointer` to an opaque C pointer.
    init<T>(_ value: UnsafeMutablePointer<T>)

    /// Returns `nil`
    static func null() -> COpaquePointer

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())
}

extension COpaquePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}

extension COpaquePointer {

    /// Reinterpret the bits of `value` as `COpaquePointer`.
    ///
    /// .. Warning:: This is a fundamentally unsafe operation, equivalent to
    ///      `unsafeBitCast(value, COpaquePointer.self)`
    init<T>(_ value: CFunctionPointer<T>)
}

extension COpaquePointer : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// The C 'short' type.
typealias CShort = Int16


/// The C 'signed char' type.
typealias CSignedChar = Int8


/// The C 'unsigned char' type.
typealias CUnsignedChar = UInt8


/// The C 'unsigned int' type.
typealias CUnsignedInt = UInt32


/// The C 'unsigned long' type.
typealias CUnsignedLong = UInt


/// The C 'unsigned long long' type.
typealias CUnsignedLongLong = UInt64


/// The C 'unsigned short' type.
typealias CUnsignedShort = UInt16


/// The corresponding Swift type to `va_list` in imported C APIs.
struct CVaListPointer {
}

extension CVaListPointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// Instances of conforming types can be encoded, and appropriately
/// passed, as elements of a C `va_list`.
///
/// This protocol is useful in presenting C "varargs" APIs natively in
/// Swift.  It only works for APIs that have a `va_list` variant, so
/// for example, it isn't much use if all you have is::
///
///   int f(int n, ...)
///
/// Given a version like this, though, ::
///
///   int f(int, va_list arguments)
///
/// you can write::
///
///   func swiftF(x: Int, arguments: CVarArgType...) -> Int {
///     return withVaList(arguments) { f(x, $0) }
///   }
protocol CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// The C++ 'wchar_t' type.
typealias CWideChar = UnicodeScalar


/// Access to the raw argc value from C.
var C_ARGC: CInt


/// Access to the raw argv value from C. Accessing the argument vector
/// through this pointer is unsafe.
var C_ARGV: UnsafeMutablePointer<UnsafeMutablePointer<Int8>>


/// `Character` represents some Unicode grapheme cluster as
/// defined by a canonical, localized, or otherwise tailored
/// segmentation algorithm.
enum Character : ExtendedGraphemeClusterLiteralConvertible, Equatable, Hashable, Comparable {
    case LargeRepresentation(OnHeap<String>)
    case SmallRepresentation

    /// Construct a `Character` containing just the given `scalar`.
    init(_ scalar: UnicodeScalar)
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32)

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: Character)
    init(_builtinExtendedGraphemeClusterLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1)

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: Character)

    /// Create an instance from a single-character `String`.
    ///
    /// Requires: `s` contains exactly one extended grapheme cluster.
    init(_ s: String)

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Character : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Character : Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target)
}

struct ClosedInterval<T : Comparable> : IntervalType, Equatable, Printable, DebugPrintable, Reflectable {

    /// The type of the `Interval`\ 's endpoints
    typealias Bound = T

    /// Construct a copy of `x`
    init(_ x: ClosedInterval<T>)

    /// Construct an interval with the given bounds.  Requires: `start`
    /// <= `end`.
    init(_ start: T, _ end: T)

    /// The `Interval`\ 's lower bound. Invariant: `start` <= `end`
    var start: T { get }

    /// The `Interval`\ 's upper bound. Invariant: `start` <= `end`
    var end: T { get }

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }

    /// Returns `true` iff the `Interval` contains `x`
    func contains(x: T) -> Bool

    /// Return `intervalToClamp` clamped to `self`.  The bounds of the
    /// result, even if it is empty, are always limited to the bounds of
    /// `self`
    func clamp(intervalToClamp: ClosedInterval<T>) -> ClosedInterval<T>

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension ClosedInterval {

    /// `true` iff the `Interval` is empty.  In the case of
    /// `ClosedInterval`, always returns `false`
    var isEmpty: Bool { get }
}


/// A collection containing a single element of type `T`.
struct CollectionOfOne<T> : CollectionType {

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = Bit

    /// Construct an instance containing just `element`.
    init(_ element: T)

    /// The position of the first element.
    var startIndex: Index { get }

    /// The "past the end" position; always identical to
    /// `startIndex.successor()`.
    ///
    /// Note: `endIndex` is not a valid argument to `subscript`.
    var endIndex: Index { get }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> GeneratorOfOne<T>
    subscript (position: Index) -> T { get }
}

extension CollectionOfOne : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A multi-pass *sequence* with addressable positions.
///
/// Positions are represented by an associated `Index` type.  Whereas
/// an arbitrary *sequence* may be consumed as it is traversed, a
/// *collection* is multi-pass: any element may be revisited merely by
/// saving its index.
///
/// The sequence view of the elements is identical to the collection
/// view.  In other words, the following code binds the same series of
/// values to `x` as does `for x in self {}`::
///
///   for i in startIndex..<endIndex {
///     let x = self[i]
///   }
protocol CollectionType : _CollectionType, SequenceType {
    subscript (position: Self.Index) -> Self.Generator.Element { get }
}


/// Instances of conforming types can be compared using relational
/// operators, which define a `strict total order
/// <http://en.wikipedia.org/wiki/Total_order#Strict_total_order>`_.
///
/// A type conforming to `Comparable` need only supply the `<` and
/// `==` operators; default implementations of `<=`, `>`, `>=`, and
/// `!=` are supplied by the standard library::
///
///   struct Singular : Comparable {}
///   func ==(x: Singular, y: Singular) -> Bool { return true }
///   func <(x: Singular, y: Singular) -> Bool { return false }
///
/// **Axioms**, in addition to those of `Equatable`:
///
/// - `x == y` implies `x <= y`, `x >= y`, `!(x < y)`, and `!(x > y)`
/// - `x < y` implies `x <= y` and `y > x`
/// - `x > y` implies `x >= y` and `y < x`
/// - `x <= y` implies `y >= x`
/// - `x >= y` implies `y <= x`
protocol Comparable : _Comparable, Equatable {
    func <=(lhs: Self, rhs: Self) -> Bool
    func >=(lhs: Self, rhs: Self) -> Bool
    func >(lhs: Self, rhs: Self) -> Bool
}


/// A fast, contiguously-stored array of `T`.
///
/// Efficiency is equivalent to that of `Array`, unless `T` is a
/// `class` or `@objc` `protocol` type, in which case using
/// `ContiguousArray` may be more efficient.  Note, however, that
/// `ContiguousArray` does not bridge to Objective-C.  See `Array`,
/// with which `ContiguousArray` shares most properties, for more
/// detail.
struct ContiguousArray<T> : MutableCollectionType, Sliceable {

    /// The type of element stored by this `ContiguousArray`
    typealias Element = T

    /// Always zero, which is the index of the first element when non-empty.
    var startIndex: Int { get }

    /// A "past-the-end" element index; the successor of the last valid
    /// subscript argument.
    var endIndex: Int { get }
    subscript (index: Int) -> T

    /// Return a *generator* over the elements.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<ContiguousArray<T>>

    /// A type that can represent a sub-range of a ContiguousArray 
    typealias SubSlice = Slice<T>
    subscript (subRange: Range<Int>) -> Slice<T>

    /// Initialization from an existing buffer does not have "array.init"
    /// semantics because the caller may retain an alias to buffer.
    init(_ buffer: _ContiguousArrayBuffer<T>)
}

extension ContiguousArray : ArrayLiteralConvertible {

    /// Create an instance containing `elements`.
    init(arrayLiteral elements: T...)
}



extension ContiguousArray {

    /// Construct an empty ContiguousArray
    init()

    /// Construct from an arbitrary sequence with elements of type `T`
    init<S : SequenceType where T == T>(_ s: S)

    /// Construct a ContiguousArray of `count` elements, each initialized to
    /// `repeatedValue`.
    init(count: Int, repeatedValue: T)

    /// How many elements the ContiguousArray stores
    var count: Int { get }

    /// How many elements the `ContiguousArray` can store without reallocation
    var capacity: Int { get }

    /// `true` if and only if the `ContiguousArray` is empty
    var isEmpty: Bool { get }

    /// The first element, or `nil` if the array is empty
    var first: T? { get }

    /// The last element, or `nil` if the array is empty
    var last: T? { get }

    /// Reserve enough space to store minimumCapacity elements.
    ///
    /// PostCondition: `capacity >= minimumCapacity` and the array has
    /// mutable contiguous storage.
    ///
    /// Complexity: O(`count`)
    mutating func reserveCapacity(minimumCapacity: Int)

    /// Append newElement to the ContiguousArray
    ///
    /// Complexity: amortized O(1) unless `self`'s storage is shared with another live array; O(`count`) otherwise.
    mutating func append(newElement: T)

    /// Append the elements of `newElements` to `self`.
    ///
    /// Complexity: O(*length of result*) 
    /// 
    mutating func extend<S : SequenceType where T == T>(newElements: S)

    /// Remove an element from the end of the ContiguousArray in O(1).
    /// Requires: count > 0
    mutating func removeLast() -> T

    /// Insert `newElement` at index `i`.
    ///
    /// Requires: `i <= count`
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func insert(newElement: T, atIndex i: Int)

    /// Remove and return the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAtIndex(index: Int) -> T

    /// Remove all elements.
    ///
    /// Postcondition: `capacity == 0` iff `keepCapacity` is `false`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeAll(keepCapacity: Bool = default)

    /// Interpose `self` between each consecutive pair of `elements`,
    /// and concatenate the elements of the resulting sequence.  For
    /// example, `[-1, -2].join([[1, 2, 3], [4, 5, 6], [7, 8, 9]])`
    /// yields `[1, 2, 3, -1, -2, 4, 5, 6, -1, -2, 7, 8, 9]`
    func join<S : SequenceType where ContiguousArray<T> == ContiguousArray<T>>(elements: S) -> ContiguousArray<T>

    /// Return the result of repeatedly calling `combine` with an
    /// accumulated value initialized to `initial` and each element of
    /// `self`, in turn, i.e. return
    /// `combine(combine(...combine(combine(initial, self[0]),
    /// self[1]),...self[count-2]), self[count-1])`.
    func reduce<U>(initial: U, combine: (U, T) -> U) -> U

    /// Sort `self` in-place according to `isOrderedBefore`.  Requires:
    /// `isOrderedBefore` induces a `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    mutating func sort(isOrderedBefore: (T, T) -> Bool)

    /// Return a copy of `self` that has been sorted according to
    /// `isOrderedBefore`.  Requires: `isOrderedBefore` induces a
    /// `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    func sorted(isOrderedBefore: (T, T) -> Bool) -> ContiguousArray<T>

    /// Return a ContiguousArray containing the results of calling
    /// `transform(x)` on each element `x` of `self`
    func map<U>(transform: (T) -> U) -> ContiguousArray<U>

    /// A ContiguousArray containing the elements of `self` in reverse order
    func reverse() -> ContiguousArray<T>

    /// Return a ContiguousArray containing the elements `x` of `self` for which
    /// `includeElement(x)` is `true`
    func filter(includeElement: (T) -> Bool) -> ContiguousArray<T>
}

extension ContiguousArray : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension ContiguousArray : Printable, DebugPrintable {

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}



extension ContiguousArray {

    /// Call `body(p)`, where `p` is a pointer to the `ContiguousArray`\ 's
    /// contiguous storage.
    ///
    /// Often, the optimizer can eliminate bounds checks within an
    /// array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    func withUnsafeBufferPointer<R>(body: (UnsafeBufferPointer<T>) -> R) -> R

    /// Call `body(p)`, where `p` is a pointer to the `ContiguousArray`\ 's
    /// mutable contiguous storage.
    ///
    /// Often, the optimizer can eliminate bounds- and uniqueness-checks
    /// within an array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    mutating func withUnsafeMutableBufferPointer<R>(body: (inout UnsafeMutableBufferPointer<T>) -> R) -> R
}

extension ContiguousArray {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if `subRange.endIndex
    /// == self.endIndex` and `isEmpty(newElements)`\ , O(N) otherwise.
    mutating func replaceRange<C : CollectionType where T == T>(subRange: Range<Int>, with newElements: C)

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count + countElements(newElements)`\ ).
    mutating func splice<S : CollectionType where T == T>(newElements: S, atIndex i: Int)

    /// Remove the indicated `subRange` of elements
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeRange(subRange: Range<Int>)
}


/// A type with a customized textual representation for debugging
/// purposes.
///
/// This textual representation is used when objects are written to an
/// *output stream* by `debugPrint` and `debugPrintln`, and is
/// typically more verbose than the text provided by a `Printable`\ 's
/// `description` property.
protocol DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// A hash-based mapping from `Key` to `Value` instances.  Also a
/// collection of key-value pairs with no defined ordering.
struct Dictionary<Key : Hashable, Value> : CollectionType, DictionaryLiteralConvertible {
    typealias Element = (Key, Value)
    typealias Index = DictionaryIndex<Key, Value>

    /// Create a dictionary with at least the given number of
    /// elements worth of storage.  The actual capacity will be the
    /// smallest power of 2 that's >= `minimumCapacity`.
    init()

    /// Create a dictionary with at least the given number of
    /// elements worth of storage.  The actual capacity will be the
    /// smallest power of 2 that's >= `minimumCapacity`.
    init(minimumCapacity: Int)

    /// The position of the first element in a non-empty dictionary.
    ///
    /// Identical to `endIndex` in an empty dictionary
    ///
    /// Complexity: amortized O(1) if `self` does not wrap a bridged
    /// `NSDictionary`, O(N) otherwise.
    var startIndex: DictionaryIndex<Key, Value> { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    ///
    /// Complexity: amortized O(1) if `self` does not wrap a bridged
    /// `NSDictionary`, O(N) otherwise.
    var endIndex: DictionaryIndex<Key, Value> { get }

    /// Returns the `Index` for the given key, or `nil` if the key is not
    /// present in the dictionary.
    func indexForKey(key: Key) -> DictionaryIndex<Key, Value>?
    subscript (position: DictionaryIndex<Key, Value>) -> (Key, Value) { get }
    subscript (key: Key) -> Value?

    /// Update the value stored in the dictionary for the given key, or, if they
    /// key does not exist, add a new key-value pair to the dictionary.
    ///
    /// Returns the value that was replaced, or `nil` if a new key-value pair
    /// was added.
    mutating func updateValue(value: Value, forKey key: Key) -> Value?

    /// Remove the key-value pair at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAtIndex(index: DictionaryIndex<Key, Value>)

    /// Remove a given key and the associated value from the dictionary.
    /// Returns the value that was removed, or `nil` if the key was not present
    /// in the dictionary.
    mutating func removeValueForKey(key: Key) -> Value?

    /// Remove all elements.
    ///
    /// Postcondition: `capacity == 0` iff `keepCapacity` is `false`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAll(keepCapacity: Bool = default)

    /// The number of entries in the dictionary.
    ///
    /// Complexity: O(1)
    var count: Int { get }

    /// Return a *generator* over the (key, value) pairs.
    ///
    /// Complexity: O(1)
    func generate() -> DictionaryGenerator<Key, Value>

    /// Create an instance initialized with `elements`.
    init(dictionaryLiteral elements: (Key, Value)...)

    /// True iff `count == 0`
    var isEmpty: Bool { get }

    /// A collection containing just the keys of `self`
    ///
    /// Keys appear in the same order as they occur as the `.0` member
    /// of key-value pairs in `self`.  Each key in the result has a
    /// unique value.
    var keys: LazyBidirectionalCollection<MapCollectionView<[Key : Value], Key>> { get }

    /// A collection containing just the values of `self`
    ///
    /// Values appear in the same order as they occur as the `.1` member
    /// of key-value pairs in `self`.
    var values: LazyBidirectionalCollection<MapCollectionView<[Key : Value], Value>> { get }
}

extension Dictionary : Printable, DebugPrintable {

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}

extension Dictionary : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}




/// A generator over the key-value pairs of a `Dictionary<Key, Value>`
struct DictionaryGenerator<Key : Hashable, Value> : GeneratorType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: no preceding call to `self.next()` has returned `nil`.
    mutating func next() -> (Key, Value)?
}


/// Used to access the key-value pairs in an instance of
/// `Dictionary<Key,Value>`.
///
/// Remember that Dictionary has two subscripting interfaces:
///
/// 1. Subscripting with a key, yielding an optional value::
///
///      v = d[k]!
///
/// 2. Subscripting with an index, yielding a key-value pair:
///
///      (k,v) = d[i]
struct DictionaryIndex<Key : Hashable, Value> : BidirectionalIndexType, Comparable {

    /// Identical to `self.dynamicType`
    typealias Index = DictionaryIndex<Key, Value>

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> DictionaryIndex<Key, Value>

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> DictionaryIndex<Key, Value>
}


/// Conforming types can be initialized with dictionary literals
protocol DictionaryLiteralConvertible {
    typealias Key
    typealias Value

    /// Create an instance initialized with `elements`.
    init(dictionaryLiteral elements: (Key, Value)...)
}

struct Double {

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Double)
}

extension Double : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Double : FloatingPointType {

    /// The positive infinity.
    static var infinity: Double { get }

    /// A quiet NaN.
    static var NaN: Double { get }

    /// A quiet NaN.
    static var quietNaN: Double { get }

    /// `true` iff `self` is negative
    var isSignMinus: Bool { get }

    /// `true` iff `self` is normal (not zero, subnormal, infinity, or
    /// NaN).
    var isNormal: Bool { get }

    /// `true` iff `self` is zero, subnormal, or normal (not infinity
    /// or NaN).
    var isFinite: Bool { get }

    /// `true` iff `self` is +0.0 or -0.0.
    var isZero: Bool { get }

    /// `true` iff `self` is subnormal.
    var isSubnormal: Bool { get }

    /// `true` iff `self` is infinity.
    var isInfinite: Bool { get }

    /// `true` iff `self` is NaN.
    var isNaN: Bool { get }

    /// `true` iff `self` is a signaling NaN.
    var isSignaling: Bool { get }
}

extension Double {

    /// The IEEE 754 "class" of this type.
    var floatingPointClass: FloatingPointClassification { get }
}

extension Double : IntegerLiteralConvertible {
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64)
}

extension Double {
    init(_builtinFloatLiteral value: Builtin.FPIEEE80)
}

extension Double : FloatLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(floatLiteral value: Double)
}

extension Double : Comparable {
}

extension Double : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Double : AbsoluteValuable {

    /// Returns the absolute value of `x`
    static func abs(x: Double) -> Double
}

extension Double {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: UInt64)
    init(_ v: Int64)
    init(_ v: UInt)
    init(_ v: Int)
}

extension Double {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Double : Strideable {

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Double) -> Double

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    func advancedBy(amount: Double) -> Double
}

extension Double : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Double : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A collection whose element type is `T` but that is always empty.
struct EmptyCollection<T> : CollectionType {

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = Int

    /// Construct an instance.
    init()

    /// Always zero, just like `endIndex`.
    var startIndex: Index { get }

    /// Always zero, just like `startIndex`.
    var endIndex: Index { get }

    /// Returns an empty *generator*.
    ///
    /// Complexity: O(1)
    func generate() -> EmptyGenerator<T>
    subscript (position: Index) -> T { get }
}

extension EmptyCollection : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A generator that never produces an element.
///
/// See also: `EmptyCollection<T>`.
struct EmptyGenerator<T> : GeneratorType, SequenceType {

    /// `EmptyGenerator` is also a `SequenceType`, so it `generate`\ 's
    /// a copy of itself
    func generate() -> EmptyGenerator<T>

    /// Return `nil`, indicating that there are no more elements.
    mutating func next() -> T?
}


/// The `GeneratorType` for `EnumerateSequence`.  `EnumerateGenerator`
/// wraps a `Base` `GeneratorType` and yields successive `Int` values,
/// starting at zero, along with the elements of the underlying
/// `Base`::
///
///   var g = EnumerateGenerator(["foo", "bar"].generate())
///   g.next() // (0, "foo")
///   g.next() // (1, "bar")
///   g.next() // nil
///
/// Note:: idiomatic usage is to call `enumerate` instead of
/// constructing an `EnumerateGenerator` directly.
struct EnumerateGenerator<Base : GeneratorType> : GeneratorType, SequenceType {

    /// The type of element returned by `next()`.
    typealias Element = (index: Int, element: Base.Element)

    /// Construct from a `Base` generator
    init(_ base: Base)

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: no preceding call to `self.next()` has returned `nil`.
    mutating func next() -> Element?

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = EnumerateGenerator<Base>

    /// `EnumerateGenerator` is also a `SequenceType`, so it `generate`\
    /// 's a copy of itself
    func generate() -> EnumerateGenerator<Base>
}


/// The `SequenceType` returned by `enumerate()`.  `EnumerateSequence`
/// is a sequence of pairs (*n*, *x*), where *n*\ s are consecutive
/// `Int`\ s starting at zero, and *x*\ s are the elements of a `Base`
/// `SequenceType`::
///
///   var s = EnumerateSequence(["foo", "bar"])
///   Array(s) // [(0, "foo"), (1, "bar")]
///
/// Note:: idiomatic usage is to call `enumerate` instead of
/// constructing an `EnumerateSequence` directly.
struct EnumerateSequence<Base : SequenceType> : SequenceType {

    /// Construct from a `Base` sequence
    init(_ base: Base)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> EnumerateGenerator<Base.Generator>
}


/// Instances of conforming types can be compared for value equality
/// using operators `==` and `!=`.
///
/// When adopting `Equatable`, only the `==` operator is required to be
/// implemented.  The standard library provides an implementation for `!=`.
protocol Equatable {

    /// Return true if `lhs` is equal to `rhs`.
    ///
    /// **Equality implies substitutability**.  When `x == y`, `x` and
    /// `y` are interchangeable in any code that only depends on their
    /// values.
    ///
    /// Class instance identity as distinguished by triple-equals `===`
    /// is notably not part of an instance's value.  Exposing other
    /// non-value aspects of `Equatable` types is discouraged, and any
    /// that *are* exposed should be explicitly pointed out in
    /// documentation.
    /// 
    /// **Equality is an equivalence relation**
    ///
    /// - `x == x` is `true`
    /// - `x == y` implies `y == x`
    /// - `x == y` and `y == z` implies `x == z`
    ///
    /// **Inequality is the inverse of equality**, i.e. `!(x == y)` iff
    /// `x != y`
    func ==(lhs: Self, rhs: Self) -> Bool
}


/// Conforming types can be initialized with string literals
/// containing a single Unicode extended grapheme cluster.
protocol ExtendedGraphemeClusterLiteralConvertible : UnicodeScalarLiteralConvertible {
    typealias ExtendedGraphemeClusterLiteralType

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: ExtendedGraphemeClusterLiteralType)
}


/// The default type for an otherwise-unconstrained unicode extended
/// grapheme cluster literal
typealias ExtendedGraphemeClusterType = String


/// A collection type that can be efficiently appended-to.
protocol ExtensibleCollectionType : _ExtensibleCollectionType {
}


/// A lazy `CollectionType` wrapper that includes the elements of an
/// underlying collection that satisfy a predicate.  Not
/// automatically returned by `filter(x)` for two reasons:
///
/// * The O(1) guarantee of our `Index` would be iffy at best, since
///   it advances an underlying `Index` until the predicate is
///   satisfied.  Be aware that a `FilterCollectionView` may not offer
///   the expected efficiency for this reason.
///
/// * Constructing an `Array` from a `CollectionType` measures the length
///   of the collection before traversing it to read the elements.
///   This causes the filter predicate to be called twice for each
///   element of the underlying collection, which is surprising.
struct FilterCollectionView<Base : CollectionType> : CollectionType {

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = FilterCollectionViewIndex<Base>

    /// Construct an instance containing the elements of `base` that
    /// satisfy `predicate`.
    init(_ base: Base, includeElement predicate: (Base.Generator.Element) -> Bool)

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    ///
    /// Complexity: O(N), where N is the ratio between unfiltered and
    /// filtered collection counts.
    var startIndex: FilterCollectionViewIndex<Base> { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    ///
    /// Complexity: O(1)
    var endIndex: FilterCollectionViewIndex<Base> { get }
    subscript (position: FilterCollectionViewIndex<Base>) -> Base.Generator.Element { get }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> FilterGenerator<Base.Generator>
}


/// The `Index` used for subscripting a `FilterCollectionView`
struct FilterCollectionViewIndex<Base : CollectionType> : ForwardIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> FilterCollectionViewIndex<Base>
}


/// The `GeneratorType` used by `FilterSequenceView` and
/// `FilterCollectionView`
struct FilterGenerator<Base : GeneratorType> : GeneratorType, SequenceType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.
    mutating func next() -> Base.Element?

    /// `FilterGenerator` is also a `SequenceType`, so it `generate`\ 's
    /// a copy of itself
    func generate() -> FilterGenerator<Base>
}


/// The lazy `SequenceType` returned by `filter(c)` where `c` is a
/// `SequenceType`
struct FilterSequenceView<Base : SequenceType> : SequenceType {

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> FilterGenerator<Base.Generator>
}

struct Float {

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Float)
}

extension Float : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Float : FloatingPointType {

    /// The positive infinity.
    static var infinity: Float { get }

    /// A quiet NaN.
    static var NaN: Float { get }

    /// A quiet NaN.
    static var quietNaN: Float { get }

    /// `true` iff `self` is negative
    var isSignMinus: Bool { get }

    /// `true` iff `self` is normal (not zero, subnormal, infinity, or
    /// NaN).
    var isNormal: Bool { get }

    /// `true` iff `self` is zero, subnormal, or normal (not infinity
    /// or NaN).
    var isFinite: Bool { get }

    /// `true` iff `self` is +0.0 or -0.0.
    var isZero: Bool { get }

    /// `true` iff `self` is subnormal.
    var isSubnormal: Bool { get }

    /// `true` iff `self` is infinity.
    var isInfinite: Bool { get }

    /// `true` iff `self` is NaN.
    var isNaN: Bool { get }

    /// `true` iff `self` is a signaling NaN.
    var isSignaling: Bool { get }
}

extension Float {

    /// The IEEE 754 "class" of this type.
    var floatingPointClass: FloatingPointClassification { get }
}

extension Float : IntegerLiteralConvertible {
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64)
}

extension Float {
    init(_builtinFloatLiteral value: Builtin.FPIEEE80)
}

extension Float : FloatLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(floatLiteral value: Float)
}

extension Float : Comparable {
}

extension Float : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Float : AbsoluteValuable {

    /// Returns the absolute value of `x`
    static func abs(x: Float) -> Float
}

extension Float {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: UInt64)
    init(_ v: Int64)
    init(_ v: UInt)
    init(_ v: Int)
}

extension Float {

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Float : Strideable {

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Float) -> Float

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    func advancedBy(amount: Float) -> Float
}

extension Float : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Float : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 32-bit floating point type
typealias Float32 = Float


/// A 64-bit floating point type
typealias Float64 = Double

struct Float80 {

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Float80)
}

extension Float80 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Float80 : IntegerLiteralConvertible {
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64)
}

extension Float80 {
    init(_builtinFloatLiteral value: Builtin.FPIEEE80)
}

extension Float80 : FloatLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(floatLiteral value: Float80)
}

extension Float80 : Comparable {
}

extension Float80 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Float80 : AbsoluteValuable {

    /// Returns the absolute value of `x`
    static func abs(x: Float80) -> Float80
}

extension Float80 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: UInt64)
    init(_ v: Int64)
    init(_ v: UInt)
    init(_ v: Int)
}

extension Float80 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)
}

extension Float80 : Strideable {

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Float80) -> Float80

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    func advancedBy(amount: Float80) -> Float80
}


/// Conforming types can be initialized with floating point literals
protocol FloatLiteralConvertible {
    typealias FloatLiteralType

    /// Create an instance initialized to `value`.
    init(floatLiteral value: FloatLiteralType)
}


/// The default type for an otherwise-unconstrained floating point literal
typealias FloatLiteralType = Double


/// The set of possible IEEE 754 "classes"
enum FloatingPointClassification {
    case SignalingNaN
    case QuietNaN
    case NegativeInfinity
    case NegativeNormal
    case NegativeSubnormal
    case NegativeZero
    case PositiveZero
    case PositiveSubnormal
    case PositiveNormal
    case PositiveInfinity
}

extension FloatingPointClassification : Equatable {
}


/// A set of common requirements for Swift's floating point types.
protocol FloatingPointType : Strideable {
    typealias _BitsType
    class func _fromBitPattern(bits: _BitsType) -> Self
    func _toBitPattern() -> _BitsType

    /// Create an instance initialized to `value`.
    init(_ value: UInt8)

    /// Create an instance initialized to `value`.
    init(_ value: Int8)

    /// Create an instance initialized to `value`.
    init(_ value: UInt16)

    /// Create an instance initialized to `value`.
    init(_ value: Int16)

    /// Create an instance initialized to `value`.
    init(_ value: UInt32)

    /// Create an instance initialized to `value`.
    init(_ value: Int32)

    /// Create an instance initialized to `value`.
    init(_ value: UInt64)

    /// Create an instance initialized to `value`.
    init(_ value: Int64)

    /// Create an instance initialized to `value`.
    init(_ value: UInt)

    /// Create an instance initialized to `value`.
    init(_ value: Int)

    /// The positive infinity.
    class var infinity: Self { get }

    /// A quiet NaN.
    class var NaN: Self { get }

    /// A quiet NaN.
    class var quietNaN: Self { get }

    /// @{
    /// IEEE 754-2008 Non-computational operations.
    /// The IEEE 754 "class" of this type.
    var floatingPointClass: FloatingPointClassification { get }

    /// `true` iff `self` is negative
    var isSignMinus: Bool { get }

    /// `true` iff `self` is normal (not zero, subnormal, infinity, or
    /// NaN).
    var isNormal: Bool { get }

    /// `true` iff `self` is zero, subnormal, or normal (not infinity
    /// or NaN).
    var isFinite: Bool { get }

    /// `true` iff `self` is +0.0 or -0.0.
    var isZero: Bool { get }

    /// `true` iff `self` is subnormal.
    var isSubnormal: Bool { get }

    /// `true` iff `self` is infinity.
    var isInfinite: Bool { get }

    /// `true` iff `self` is NaN.
    var isNaN: Bool { get }

    /// `true` iff `self` is a signaling NaN.
    var isSignaling: Bool { get }
}


/// Represents a discrete value in a series, where a value's
/// successor, if any, is reachable by applying the value's
/// `successor()` method.
protocol ForwardIndexType : _ForwardIndexType {
}


/// A type-erased generator.
///
/// The generator for `SequenceOf<T>`.  Forwards operations to an
/// arbitrary underlying generator with the same `Element` type,
/// hiding the specifics of the underlying generator type.
///
/// See also: `SequenceOf<T>`.
struct GeneratorOf<T> : GeneratorType, SequenceType {

    /// Construct an instance whose `next()` method calls `nextElement`.
    init(_ nextElement: () -> T?)

    /// Construct an instance whose `next()` method pulls its results
    /// from `base`.
    init<G : GeneratorType where T == T>(_ base: G)

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.
    mutating func next() -> T?

    /// `GeneratorOf<T>` is also a `SequenceType`, so it `generate`\
    /// 's a copy of itself
    func generate() -> GeneratorOf<T>
}


/// A generator that produces one or fewer instances of `T`.
struct GeneratorOfOne<T> : GeneratorType, SequenceType {

    /// Construct an instance that generates `element!`, or an empty
    /// sequence if `element == nil`.
    init(_ element: T?)

    /// `GeneratorOfOne` is also a `SequenceType`, so it `generate`\
    /// 's a copy of itself
    func generate() -> GeneratorOfOne<T>

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.
    mutating func next() -> T?
}


/// A sequence built around a generator of type `G`.
///
/// Useful mostly to recover the ability to use `for`\ ...\ `in`,
/// given just a generator `g`::
///
///   for x in GeneratorSequence(g) { ... }
struct GeneratorSequence<G : GeneratorType> : GeneratorType, SequenceType {

    /// Construct an instance whose generator is a copy of `base`.
    init(_ base: G)

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.
    mutating func next() -> G.Element?

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> GeneratorSequence<G>
}


/// Encapsulates iteration state and interface for iteration over a
/// *sequence*.
///
/// **Note:** While it is safe to copy a *generator*, advancing one
/// copy may invalidate the others.
///
/// Any code that uses multiple generators (or `for`\ ...\ `in` loops)
/// over a single *sequence* should have static knowledge that the
/// specific *sequence* is multi-pass, either because its concrete
/// type is known or because it is constrained to `CollectionType`.
/// Also, the generators must be obtained by distinct calls to the
/// *sequence's* `generate()` method, rather than by copying.
protocol GeneratorType {

    /// The type of element generated by `self`.
    typealias Element

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.  Specific implementations of this protocol
    /// are encouraged to respond to violations of this requirement by
    /// calling `preconditionFailure("...")`.
    mutating func next() -> Element?
}

struct HalfOpenInterval<T : Comparable> : IntervalType, Equatable, Printable, DebugPrintable, Reflectable {

    /// The type of the `Interval`\ 's endpoints
    typealias Bound = T

    /// Construct a copy of `x`
    init(_ x: HalfOpenInterval<T>)

    /// Construct an interval with the given bounds.  Requires: `start`
    /// <= `end`.
    init(_ start: T, _ end: T)

    /// The `Interval`\ 's lower bound. Invariant: `start` <= `end`
    var start: T { get }

    /// The `Interval`\ 's upper bound. Invariant: `start` <= `end`
    var end: T { get }

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }

    /// Returns `true` iff the `Interval` contains `x`
    func contains(x: T) -> Bool

    /// Return `intervalToClamp` clamped to `self`.  The bounds of the
    /// result, even if it is empty, are always limited to the bounds of
    /// `self`
    func clamp(intervalToClamp: HalfOpenInterval<T>) -> HalfOpenInterval<T>

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension HalfOpenInterval {

    /// `true` iff the `Interval` is empty.
    var isEmpty: Bool { get }
}


/// Instances of conforming types provide an integer `hashValue` and
/// can be used as `Dictionary` keys.
protocol Hashable : Equatable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}


/// Management API for `HeapBufferStorage<Value, Element>`
struct HeapBuffer<Value, Element> : Equatable {
    typealias Storage = HeapBufferStorage<Value, Element>

    /// Create a `HeapBuffer` with `self.value = initializer` and
    /// `self._capacity() >= capacity`.
    init(_ storageClass: AnyClass, _ initializer: Value, _ capacity: Int)

    /// True if storage is non-\ `nil`
    var hasStorage: Bool { get }
    mutating func isUniquelyReferenced() -> Bool
}


/// A class containing an ivar "value" of type Value, and
/// containing storage for an array of Element whose size is
/// determined at create time.
///
/// The analogous C++-ish class template would be::
///
///   template <class Value, class Element>
///   struct HeapBuffer {
///     Value value;
///     Element baseAddress[];        // length determined at creation time
///
///     HeapBuffer() = delete
///     static shared_ptr<HeapBuffer> create(Value init, int capacity);
///   }
///
/// Note that the Element array is RAW MEMORY.  You are expected to
/// construct and---if necessary---destroy Elements there yourself,
/// either in a derived class, or it can be in some manager object
/// that owns the HeapBuffer.
class HeapBufferStorage<Value, Element> {

    /// The type used to actually manage instances of
    /// `HeapBufferStorage<Value,Element>`
    typealias Buffer = HeapBuffer<Value, Element>
    deinit 
}


/// An optional type that allows implicit member access (via compiler
/// magic).
///
/// The compiler has special knowledge of the existence of
/// ImplicitlyUnwrappedOptional<T>, but always interacts with it using the
/// library intrinsics below.
enum ImplicitlyUnwrappedOptional<T> : Reflectable, NilLiteralConvertible {
    case None
    case Some(T)

    /// Construct a `nil` instance.
    init()

    /// Construct a non-\ `nil` instance that stores `some`.
    init(_ some: T)

    /// Construct an instance from an explicitly unwrapped optional
    /// (`T?`).
    init(_ v: T?)

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())

    /// If `self == nil`, returns `nil`.  Otherwise, returns `f(self!)`.
    func map<U>(f: (T) -> U) -> U!

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension ImplicitlyUnwrappedOptional : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension ImplicitlyUnwrappedOptional : _ObjectiveCBridgeable {
}


/// A *generator* for an arbitrary *collection*.  Provided `C`
/// conforms to the other requirements of *CollectionType*,
/// `IndexingGenerator<C>` can be used as the result of `C`\ 's
/// `generate()` method.  For example:
///
/// .. parsed-literal::
///
///    struct MyCollection : CollectionType {
///      struct Index : ForwardIndexType { *implementation hidden* }
///      subscript(i: Index) -> MyElement { *implementation hidden* }
///      func generate() -> **IndexingGenerator<MyCollection>** {
///        return IndexingGenerator(self)
///      }
///    }
struct IndexingGenerator<C : _CollectionType> : GeneratorType, SequenceType {

    /// Create a *generator* over the given collection
    init(_ seq: C)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<C>

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: no preceding call to `self.next()` has returned `nil`.
    mutating func next() -> C._Element?
}


/// A 64-bit signed integer value
/// type.
struct Int : SignedIntegerType {
    var value: Builtin.Word

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Int)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: Int)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: Int)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: Int { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: Int { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: Int { get }
    static var max: Int { get }
    static var min: Int { get }
}

extension Int : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Int : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Int : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int
}

extension Int {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int, _ rhs: Int) -> (Int, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int, _ rhs: Int) -> (Int, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int, _ rhs: Int) -> (Int, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int, _ rhs: Int) -> (Int, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int, _ rhs: Int) -> (Int, overflow: Bool)

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax
}

extension Int : SignedNumberType {
}

extension Int {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: UInt64)

    /// Construct a `Int` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `Int` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `Int` having the same memory representation as
    /// the `UInt` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt)
}

extension Int : BitwiseOperationsType {

    /// The empty bitset of type Int.
    static var allZeros: Int { get }
}

extension Int {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Int : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Int : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 16-bit signed integer value
/// type.
struct Int16 : SignedIntegerType {
    var value: Builtin.Int16

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Int16)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: Int16)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: Int16)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int16)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: Int16 { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: Int16 { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: Int16 { get }
    static var max: Int16 { get }
    static var min: Int16 { get }
}

extension Int16 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Int16 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Int16 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int16

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int16

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int16) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int16
}

extension Int16 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int16, _ rhs: Int16) -> (Int16, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int16, _ rhs: Int16) -> (Int16, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int16, _ rhs: Int16) -> (Int16, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int16, _ rhs: Int16) -> (Int16, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int16, _ rhs: Int16) -> (Int16, overflow: Bool)

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax
}

extension Int16 : SignedNumberType {
}

extension Int16 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: UInt32)

    /// Construct a `Int16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt32)
    init(_ v: Int32)

    /// Construct a `Int16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int32)
    init(_ v: UInt64)

    /// Construct a `Int16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `Int16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `Int16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt)
    init(_ v: Int)

    /// Construct a `Int16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int)

    /// Construct a `Int16` having the same memory representation as
    /// the `UInt16` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int16` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt16)
}

extension Int16 : BitwiseOperationsType {

    /// The empty bitset of type Int16.
    static var allZeros: Int16 { get }
}

extension Int16 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Int16 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Int16 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 32-bit signed integer value
/// type.
struct Int32 : SignedIntegerType {
    var value: Builtin.Int32

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Int32)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: Int32)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: Int32)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int32)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: Int32 { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: Int32 { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: Int32 { get }
    static var max: Int32 { get }
    static var min: Int32 { get }
}

extension Int32 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Int32 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Int32 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int32

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int32

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int32) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int32
}

extension Int32 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool)

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax
}

extension Int32 : SignedNumberType {
}

extension Int32 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: UInt64)

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt)
    init(_ v: Int)

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int)

    /// Construct a `Int32` having the same memory representation as
    /// the `UInt32` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int32` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt32)
}

extension Int32 : BitwiseOperationsType {

    /// The empty bitset of type Int32.
    static var allZeros: Int32 { get }
}

extension Int32 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Int32 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Int32 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 64-bit signed integer value
/// type.
struct Int64 : SignedIntegerType {
    var value: Builtin.Int64

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Int64)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: Int64)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: Int64)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: Int64 { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: Int64 { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: Int64 { get }
    static var max: Int64 { get }
    static var min: Int64 { get }
}

extension Int64 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Int64 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Int64 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int64

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int64

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int64) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int64
}

extension Int64 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int64, _ rhs: Int64) -> (Int64, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int64, _ rhs: Int64) -> (Int64, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int64, _ rhs: Int64) -> (Int64, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int64, _ rhs: Int64) -> (Int64, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int64, _ rhs: Int64) -> (Int64, overflow: Bool)

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax
}

extension Int64 : SignedNumberType {
}

extension Int64 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: UInt64)
    init(_ v: UInt)
    init(_ v: Int)

    /// Construct a `Int64` having the same memory representation as
    /// the `UInt64` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int64` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt64)
}

extension Int64 : BitwiseOperationsType {

    /// The empty bitset of type Int64.
    static var allZeros: Int64 { get }
}

extension Int64 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Int64 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Int64 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 8-bit signed integer value
/// type.
struct Int8 : SignedIntegerType {
    var value: Builtin.Int8

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: Int8)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int8)
    static var max: Int8 { get }
    static var min: Int8 { get }
}

extension Int8 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension Int8 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension Int8 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int8

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int8

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int8) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int8
}

extension Int8 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool)

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax
}

extension Int8 : SignedNumberType {
}

extension Int8 {
    init(_ v: UInt8)
    init(_ v: UInt16)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt16)
    init(_ v: Int16)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int16)
    init(_ v: UInt32)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt32)
    init(_ v: Int32)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int32)
    init(_ v: UInt64)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt)
    init(_ v: Int)

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int)

    /// Construct a `Int8` having the same memory representation as
    /// the `UInt8` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int8` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt8)
}

extension Int8 : BitwiseOperationsType {

    /// The empty bitset of type Int8.
    static var allZeros: Int8 { get }
}

extension Int8 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension Int8 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Int8 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// The largest native signed integer type
typealias IntMax = Int64


/// The common requirements for types that support integer arithmetic.
protocol IntegerArithmeticType : _IntegerArithmeticType, Comparable {

    /// Add `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func +(lhs: Self, rhs: Self) -> Self

    /// Subtract `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func -(lhs: Self, rhs: Self) -> Self

    /// Multiply `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func *(lhs: Self, rhs: Self) -> Self

    /// Divide `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func /(lhs: Self, rhs: Self) -> Self

    /// Divide `lhs` and `rhs`, returning the remainder and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func %(lhs: Self, rhs: Self) -> Self

    /// Explicitly convert to `IntMax`, trapping on overflow (except in
    /// -Ounchecked builds).
    func toIntMax() -> IntMax
}


/// Conforming types can be initialized with integer literals
protocol IntegerLiteralConvertible {
    typealias IntegerLiteralType

    /// Create an instance initialized to `value`.
    init(integerLiteral value: IntegerLiteralType)
}


/// The default type for an otherwise-unconstrained integer literal
typealias IntegerLiteralType = Int


/// A set of common requirements for Swift's integer types.
protocol IntegerType : _IntegerType, RandomAccessIndexType {
}


/// An interval over a `Comparable` type.
protocol IntervalType {

    /// The type of the `Interval`\ 's endpoints
    typealias Bound : Comparable

    /// Returns `true` iff the interval contains `value`
    func contains(value: Bound) -> Bool

    /// Return `rhs` clamped to `self`.  The bounds of the result, even
    /// if it is empty, are always within the bounds of `self`
    func clamp(intervalToClamp: Self) -> Self

    /// True iff `self` is empty
    var isEmpty: Bool { get }

    /// The `Interval`\ 's lower bound. Invariant: `start` <= `end`
    var start: Bound { get }

    /// The `Interval`\ 's upper bound. Invariant: `start` <= `end`
    var end: Bound { get }
}


/// A collection that forwards its implementation to an underlying
/// collection instance while exposing lazy computations as methods.
struct LazyBidirectionalCollection<S : CollectionType where S.Index : BidirectionalIndexType> : CollectionType {

    /// Construct an instance with `base` as its underlying collection
    /// instance.
    init(_ base: S)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> S.Generator

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: S.Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: S.Index { get }

    /// True if and only if the collection is empty
    var isEmpty: Bool { get }

    /// The first element, or `nil` if `self` is empty
    var first: S.Generator.Element? { get }

    /// The last element, or `nil` if `self` is empty
    var last: S.Generator.Element? { get }
    subscript (position: S.Index) -> S.Generator.Element { get }

    /// an Array, created on-demand, containing the elements of this
    /// lazy CollectionType.
    var array: [S.Generator.Element] { get }
}

extension LazyBidirectionalCollection {

    /// Return a lazy SequenceType containing the elements `x` of `source` for
    /// which `includeElement(x)` is `true`
    func filter(includeElement: (S.Generator.Element) -> Bool) -> LazySequence<FilterSequenceView<S>>
}

extension LazyBidirectionalCollection {

    /// Return a `MapCollectionView` over this `LazyBidirectionalCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func map<U>(transform: (S.Generator.Element) -> U) -> LazyBidirectionalCollection<MapCollectionView<S, U>>
}

extension LazyBidirectionalCollection {

    /// Return a `BidirectionalReverseView` over this `LazyBidirectionalCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func reverse() -> LazyBidirectionalCollection<BidirectionalReverseView<S>>
}


/// A collection that forwards its implementation to an underlying
/// collection instance while exposing lazy computations as methods.
struct LazyForwardCollection<S : CollectionType where S.Index : ForwardIndexType> : CollectionType {

    /// Construct an instance with `base` as its underlying collection
    /// instance.
    init(_ base: S)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> S.Generator

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: S.Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: S.Index { get }

    /// True if and only if the collection is empty
    var isEmpty: Bool { get }

    /// The first element, or `nil` if `self` is empty
    var first: S.Generator.Element? { get }
    subscript (position: S.Index) -> S.Generator.Element { get }

    /// an Array, created on-demand, containing the elements of this
    /// lazy CollectionType.
    var array: [S.Generator.Element] { get }
}

extension LazyForwardCollection {

    /// Return a lazy SequenceType containing the elements `x` of `source` for
    /// which `includeElement(x)` is `true`
    func filter(includeElement: (S.Generator.Element) -> Bool) -> LazySequence<FilterSequenceView<S>>
}

extension LazyForwardCollection {

    /// Return a `MapCollectionView` over this `LazyForwardCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func map<U>(transform: (S.Generator.Element) -> U) -> LazyForwardCollection<MapCollectionView<S, U>>
}


/// A collection that forwards its implementation to an underlying
/// collection instance while exposing lazy computations as methods.
struct LazyRandomAccessCollection<S : CollectionType where S.Index : RandomAccessIndexType> : CollectionType {

    /// Construct an instance with `base` as its underlying collection
    /// instance.
    init(_ base: S)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> S.Generator

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: S.Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: S.Index { get }

    /// True if and only if the collection is empty
    var isEmpty: Bool { get }

    /// The first element, or `nil` if `self` is empty
    var first: S.Generator.Element? { get }

    /// The last element, or `nil` if `self` is empty
    var last: S.Generator.Element? { get }
    subscript (position: S.Index) -> S.Generator.Element { get }

    /// an Array, created on-demand, containing the elements of this
    /// lazy CollectionType.
    var array: [S.Generator.Element] { get }
}

extension LazyRandomAccessCollection {

    /// Return a lazy SequenceType containing the elements `x` of `source` for
    /// which `includeElement(x)` is `true`
    func filter(includeElement: (S.Generator.Element) -> Bool) -> LazySequence<FilterSequenceView<S>>
}

extension LazyRandomAccessCollection {

    /// Return a `MapCollectionView` over this `LazyRandomAccessCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func map<U>(transform: (S.Generator.Element) -> U) -> LazyRandomAccessCollection<MapCollectionView<S, U>>
}

extension LazyRandomAccessCollection {

    /// Return a `RandomAccessReverseView` over this `LazyRandomAccessCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func reverse() -> LazyBidirectionalCollection<RandomAccessReverseView<S>>
}


/// A sequence that forwards its implementation to an underlying
/// sequence instance while exposing lazy computations as methods.
struct LazySequence<S : SequenceType> : SequenceType {

    /// Construct an instance with `base` as its underlying sequence
    /// instance.
    init(_ base: S)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> S.Generator

    /// an Array, created on-demand, containing the elements of this
    /// lazy SequenceType.
    var array: [S.Generator.Element] { get }
}

extension LazySequence {

    /// Return a lazy SequenceType containing the elements `x` of `source` for
    /// which `includeElement(x)` is `true`
    func filter(includeElement: (S.Generator.Element) -> Bool) -> LazySequence<FilterSequenceView<S>>
}

extension LazySequence {

    /// Return a `MapSequenceView` over this `LazySequence`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func map<U>(transform: (S.Generator.Element) -> U) -> LazySequence<MapSequenceView<S, U>>
}


/// A `CollectionType` whose elements consist of those in a `Base`
/// `CollectionType` passed through a transform function returning `T`.
/// These elements are computed lazily, each time they're read, by
/// calling the transform function on a base element.
struct MapCollectionView<Base : CollectionType, T> : CollectionType {

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: Base.Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Base.Index { get }
    subscript (position: Base.Index) -> T { get }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> MapSequenceGenerator<Base.Generator, T>
}


/// The `GeneratorType` used by `MapSequenceView` and `MapCollectionView`.
/// Produces each element by passing the output of the `Base`
/// `GeneratorType` through a transform function returning `T`
struct MapSequenceGenerator<Base : GeneratorType, T> : GeneratorType, SequenceType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.
    mutating func next() -> T?

    /// `MapSequenceGenerator` is also a `SequenceType`, so it
    /// `generate`\ 's a copy of itself
    func generate() -> MapSequenceGenerator<Base, T>
}


/// A `SequenceType` whose elements consist of those in a `Base`
/// `SequenceType` passed through a transform function returning `T`.
/// These elements are computed lazily, each time they're read, by
/// calling the transform function on a base element.
struct MapSequenceView<Base : SequenceType, T> : SequenceType {

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> MapSequenceGenerator<Base.Generator, T>
}


/// How children of this value should be presented in the IDE.
enum MirrorDisposition {
    case Struct
    case Class
    case Enum
    case Tuple
    case Aggregate
    case IndexContainer
    case KeyContainer
    case MembershipContainer
    case Container
    case Optional
    case ObjCObject
}


/// The type returned by `reflect(x)`; supplies an API for runtime
/// reflection on `x`
protocol MirrorType {

    /// The instance being reflected
    var value: Any { get }

    /// Identical to `value.dynamicType`
    var valueType: Any.Type { get }

    /// A unique identifier for `value` if it is a class instance; `nil`
    /// otherwise.
    var objectIdentifier: ObjectIdentifier? { get }

    /// The count of `value`\ 's logical children 
    var count: Int { get }
    subscript (i: Int) -> (String, MirrorType) { get }

    /// A string description of `value`.
    var summary: String { get }

    /// A rich representation of `value` for an IDE, or `nil` if none is supplied.
    var quickLookObject: QuickLookObject? { get }

    /// How `value` should be presented in an IDE.
    var disposition: MirrorDisposition { get }
}


/// A *collection* that supports subscript assignment.
///
/// For any instance `a` of a type conforming to
/// `MutableCollectionType`, ::
///
///   a[i] = x
///   let y = a[i]
///
/// is equivalent to ::
///
///   a[i] = x
///   let y = x
///
protocol MutableCollectionType : CollectionType {
    subscript (position: Self.Index) -> Self.Generator.Element { get set }
}


/// A *collection* with mutable slices.
///
/// For example,
///
/// .. parsed-literal:
///
///      x[i..<j] = *someExpression*
///      x[i..<j].\ *mutatingMethod*\ ()
protocol MutableSliceable : Sliceable, MutableCollectionType {
    subscript (_: Range<Self.Index>) -> Self.SubSlice { get set }
}


/// Conforming types can be initialized with `nil`.
protocol NilLiteralConvertible {

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())
}


/// A unique identifier for a class instance. This can be used by reflection
/// clients to recognize cycles in the object graph.
///
/// In Swift, only class instances have unique identities. There is no notion
/// of identity for structs, enums, or tuples.
struct ObjectIdentifier : Hashable {

    /// Convert to a `UInt` that captures the full value of `self`.
    ///
    /// Axiom: `a.uintValue() == b.uintValue()` iff `a == b`
    func uintValue() -> UInt

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }

    /// Construct an instance that uniquely identifies `x`.
    init(_ x: AnyObject)
}

struct OnHeap<T> {
}

enum Optional<T> : Reflectable, NilLiteralConvertible {
    case None
    case Some(T)

    /// Construct a `nil` instance.
    init()

    /// Construct a non-\ `nil` instance that stores `some`.
    init(_ some: T)

    /// If `self == nil`, returns `nil`.  Otherwise, returns `f(self!)`.
    func map<U>(f: (T) -> U) -> U?

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())
}

extension Optional : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// A target of text streaming operations.
protocol OutputStreamType {

    /// Append the given `string` to this stream.
    mutating func write(string: String)
}


/// A *generator* that adapts a *collection* `C` and any *sequence* of
/// its `Index` type to present the collection's elements in a
/// permuted order.
struct PermutationGenerator<C : CollectionType, Indices : SequenceType where C.Index == C.Index> : GeneratorType, SequenceType {

    /// The type of element returned by `next()`.
    typealias Element = C.Generator.Element

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: no preceding call to `self.next()` has returned `nil`.
    mutating func next() -> Element?

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = PermutationGenerator<C, Indices>

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> PermutationGenerator<C, Indices>

    /// Construct a *generator* over a permutation of `elements` given
    /// by `indices`.
    ///
    /// Requires: `elements[i]` is valid for every `i` in `indices`.
    init(elements: C, indices: Indices)
}


/// A type with a customized textual representation.
///
/// This textual representation is used when objects are written to an
/// *output stream*, for example, by `print` and `println`.
protocol Printable {

    /// A textual representation of `self`.
    var description: String { get }
}


/// An instance that exposes API for interaction with processes
let Process: _Process


/// The sum of types that can be used as a quick look representation.
///
/// This type must be binary-compatible with the 'QuickLookObject' struct in
/// stdlib/Runtime/Reflection.mm, and 'QuickLookObject?' must be binary
/// compatible with 'OptionalQuickLookObject' from the same.
///
/// NB: This type is somewhat carefully laid out to *suppress* enum layout
/// optimization so that it is easier to manufacture in the C++ runtime
/// implementation.
enum QuickLookObject {
    case Text(String)
    case Int(Int64)
    case UInt(UInt64)
    case Float(Double)
    case Image(Any)
    case Sound(Any)
    case Color(Any)
    case BezierPath(Any)
    case AttributedString(Any)
    case Rectangle(Double, Double, Double, Double)
    case Point(Double, Double)
    case Size(Double, Double)
    case Logical(Bool)
    case Range(UInt64, UInt64)
    case View(Any)
    case Sprite(Any)
    case URL(String)
}


/// An *index* that can be offset by an arbitrary number of positions,
/// and can measure the distance to any reachable value, in O(1).
protocol RandomAccessIndexType : BidirectionalIndexType, _RandomAccessIndexType {
}


/// The lazy `CollectionType` returned by `reverse(c)` where `c` is a
/// `CollectionType` with an `Index` conforming to `RandomAccessIndexType`
struct RandomAccessReverseView<T : CollectionType where T.Index : RandomAccessIndexType> : CollectionType {

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = ReverseRandomAccessIndex<T.Index>

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = IndexingGenerator<RandomAccessReverseView<T>>

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<RandomAccessReverseView<T>>

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Index { get }
    subscript (position: Index) -> T.Generator.Element { get }
}


/// A collection of consecutive discrete index values.
///
/// :param: `T` is both the element type and the index type of the
///   collection.
///
/// Like other collections, a range containing one element has an
/// `endIndex` that is the successor of its `startIndex`; and an empty
/// range has `startIndex == endIndex`.
///
/// Axiom: for any `Range` `r`, `r[i] == i`.
///
/// Therefore, if `T` has a maximal value, it can serve as an
/// `endIndex`, but can never be contained in a `Range<T>`.
///
/// It also follows from the axiom above that `(-99..<100)[0] == 0`.
/// To prevent confusion (because some expect the result to be `-99`),
/// in a context where `T` is known to be an integer type,
/// subscripting with `T` is a compile-time error::
///
///   // error: could not find an overload for 'subscript'...
///   println( Range<Int>(start:-99, end:100)[0] )
///
/// However, subscripting that range still works in a generic context::
///
///   func brackets<T:ForwardIndexType>(x: Range<T>, i: T) -> T {
///     return x[i] // Just forward to subscript
///   }
///   println(brackets(Range<Int>(start:-99, end:100), 0)) // prints 0
struct Range<T : ForwardIndexType> : Equatable, CollectionType, Printable, DebugPrintable {

    /// Construct a copy of `x`
    init(_ x: Range<T>)

    /// Construct a range with `startIndex == start` and `endIndex ==
    /// end`.
    init(start: T, end: T)

    /// `true` iff the range is empty, i.e. `startIndex == endIndex`
    var isEmpty: Bool { get }

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = T
    typealias Slice = Range<T>
    subscript (position: T) -> T { get }
    subscript (_: T._DisabledRangeIndex) -> T { get }

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = RangeGenerator<T>

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> RangeGenerator<T>

    /// The range's lower bound
    ///
    /// Identical to `endIndex` in an empty range.
    var startIndex: T

    /// The range's upper bound
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: T

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}

extension Range {

    /// Return an array containing the results of calling
    /// `transform(x)` on each element `x` of `self`.
    func map<U>(transform: (T) -> U) -> [U]
}

extension Range : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A generator over the elements of `Range<T>`
struct RangeGenerator<T : ForwardIndexType> : GeneratorType, SequenceType {

    /// The type of element returned by `next()`.
    typealias Element = T

    /// Construct an instance that traverses the elements of `bounds`
    init(_ bounds: Range<T>)

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    mutating func next() -> T?

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = RangeGenerator<T>

    /// `RangeGenerator` is also a `SequenceType`, so it
    /// `generate`\ 's a copy of itself
    func generate() -> RangeGenerator<T>

    /// The lower bound of the remaining range.
    var startIndex: T

    /// The upper bound of the remaining range; not included in the
    /// generated sequence.
    var endIndex: T
}


/// A *collection* that supports replacement of an arbitrary subRange
/// of elements with the elements of another collection.
protocol RangeReplaceableCollectionType : ExtensibleCollectionType {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if
    /// `subRange.endIndex == self.endIndex` and `isEmpty(newElements)`\ ,
    /// O(\ `countElements(self)`\ + \`countElements(newElements)`\ ) otherwise.
    mutating func replaceRange<C : CollectionType where `Self`.Generator.Element == Self.Generator.Element>(subRange: Range<Self.Index>, with newElements: C)

    /// Insert `newElement` at index `i`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    ///
    /// Can be implemented as::
    ///
    ///   Swift.insert(&self, newElement, atIndex: i)
    mutating func insert(newElement: Self.Generator.Element, atIndex i: Self.Index)

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self) + countElements(newElements)`\ ).
    ///
    /// Can be implemented as::
    ///
    ///   Swift.splice(&self, newElements, atIndex: i)
    mutating func splice<S : CollectionType where `Self`.Generator.Element == Self.Generator.Element>(newElements: S, atIndex i: Self.Index)

    /// Remove the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    ///
    /// Can be implemented as::
    ///
    ///   Swift.removeAtIndex(&self, i)
    mutating func removeAtIndex(i: Self.Index) -> Self.Generator.Element

    /// Remove the indicated `subRange` of elements
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    ///
    /// Can be implemented as::
    ///
    ///   Swift.removeRange(&self, subRange)
    mutating func removeRange(subRange: Range<Self.Index>)

    /// Remove all elements
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// :param: `keepCapacity`, if `true`, is a non-binding request to
    ///    avoid releasing storage, which can be a useful optimization
    ///    when `self` is going to be grown again.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    ///
    /// Can be implemented as::
    ///
    ///   Swift.removeAll(&self, keepCapacity: keepCapacity)
    mutating func removeAll(#keepCapacity: Bool)
}


/// A byte-sized thing that isn't designed to interoperate with
/// any other types; it makes a decent parameter to UnsafeMutablePointer when
/// you just want to do bytewise pointer arithmetic.
struct RawByte {
}


/// Protocol for `NS_OPTIONS` imported from Objective-C
protocol RawOptionSetType : _RawOptionSetType, BitwiseOperationsType, NilLiteralConvertible {
}


/// A type that can be converted to an associated "raw" type, then
/// converted back to produce an instance equivalent to the original.
protocol RawRepresentable {

    /// The "raw" type that can be used to represent all values of `Self`.
    ///
    /// Every distinct value of `self` has a corresponding unique
    /// value of `RawValue`, but `RawValue` may have representations
    /// that do not correspond to an value of `Self`.
    typealias RawValue

    /// Convert from a value of `RawValue`, yielding `nil` iff
    /// `rawValue` does not correspond to a value of `Self`.
    init?(rawValue: RawValue)

    /// The corresponding value of the "raw" type.
    ///
    /// `Self(rawValue: self.rawValue)!` is equivalent to `self`.
    var rawValue: RawValue { get }
}


/// Customizes the result of `reflect(x)`, where `x` is a conforming
/// type.
protocol Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A collection whose elements are all identical `T`\ s.
struct Repeat<T> : CollectionType {

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = Int

    /// Construct an instance that contains `count` elements having the
    /// value `repeatedValue`.
    init(count: Int, repeatedValue: T)

    /// Always zero, which is the index of the first element in a
    /// non-empty instance.
    var startIndex: Index { get }

    /// Always equal to `count`, which is one greater than the index of
    /// the last element in a non-empty instance.
    var endIndex: Index { get }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<Repeat<T>>
    subscript (position: Int) -> T { get }

    /// The number of elements in this collection.
    var count: Int

    /// The value of every element in this collection.
    let repeatedValue: T
}


/// A wrapper for a `BidirectionalIndexType` that reverses its
/// direction of traversal
struct ReverseBidirectionalIndex<I : BidirectionalIndexType> : BidirectionalIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> ReverseBidirectionalIndex<I>

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> ReverseBidirectionalIndex<I>
}


/// A wrapper for a `RandomAccessIndexType` that reverses its
/// direction of traversal
struct ReverseRandomAccessIndex<I : RandomAccessIndexType> : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> ReverseRandomAccessIndex<I>

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> ReverseRandomAccessIndex<I>

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: ReverseRandomAccessIndex<I>) -> I.Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: I.Distance) -> ReverseRandomAccessIndex<I>
}


/// A type-erased sequence.
/// 
/// Forwards operations to an arbitrary underlying sequence with the
/// same `Element` type, hiding the specifics of the underlying
/// sequence type.
///
/// See also: `GeneratorOf<T>`.
struct SequenceOf<T> : SequenceType {

    /// Construct an instance whose `generate()` method forwards to
    /// `makeUnderlyingGenerator`
    init<G : GeneratorType where T == T>(_ makeUnderlyingGenerator: () -> G)

    /// Construct an instance whose `generate()` method forwards to
    /// that of `base`.
    init<S : SequenceType where T == T>(_ base: S)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> GeneratorOf<T>
}


/// A type that can be iterated with a `for`\ ...\ `in` loop.
///
/// `SequenceType` makes no requirement on conforming types regarding
/// whether they will be destructively "consumed" by iteration.  To
/// ensure non-destructive iteration, constrain your *sequence* to
/// `CollectionType`.
protocol SequenceType : _Sequence_Type {

    /// A type that provides the *sequence*\ 's iteration interface and
    /// encapsulates its iteration state.
    typealias Generator : GeneratorType

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> Generator
}


/// A set of common requirements for Swift's signed integer types.
protocol SignedIntegerType : _SignedIntegerType, IntegerType {
}


/// Instances of conforming types can be subtracted, arithmetically
/// negated, and initialized from `0`.
///
/// Axioms:
///
/// - `x - 0 == x`
/// - `-x == 0 - x`
/// - `-(-x) == x`
protocol SignedNumberType : _SignedNumberType {

    /// Return the result of negating `x`.
    prefix func -(x: Self) -> Self
}


/// A type-erased sink.
///
/// Forwards operations to an arbitrary underlying sink with the same
/// `Element` type, hiding the specifics of the underlying sink type.
struct SinkOf<T> : SinkType {

    /// Construct an instance whose `put(x)` calls `putElement(x)`
    init(_ putElement: (T) -> ())

    /// Construct an instance whose `put(x)` calls `base.put(x)`
    init<S : SinkType where T == T>(_ base: S)

    /// Write `x` to this sink.
    func put(x: T)
}


/// Instances of conforming types are effectively functions with the
/// signature `(Element) -> Void`.
///
/// Useful mainly when the optimizer's ability to specialize generics
/// outstrips its ability to specialize ordinary closures.  For
/// example, you may find that instead of::
///
///   func f(g: (X)->Void) { ... g(a) ...}
///
/// the following generates better code::
///
///   func f<T: Sink where T.Element == X>(g: T) { ... g.put(a) ...}
protocol SinkType {

    /// The type of element to be written to this sink.
    typealias Element

    /// Write `x` to this sink.
    mutating func put(x: Element)
}


/// The `Array`-like type that represents a sub-sequence of any
/// `Array`, `ContiguousArray`, or other `Slice`.
///
/// `Slice` always uses contiguous storage and does not bridge to
/// Objective-C.
///
/// .. Warning:: Long-term storage of `Slice` instances is discouraged
///
///    Because a `Slice` presents a *view* onto the storage of some
///    larger array even after the original array's lifetime ends,
///    storing the slice may prolong the lifetime of elements that are
///    no longer accessible, which can manifest as apparent memory and
///    object leakage.  To prevent this effect, use `Slice` only for
///    transient computation.
struct Slice<T> : MutableCollectionType, Sliceable {

    /// The type of element stored by this `Slice`
    typealias Element = T

    /// Always zero, which is the index of the first element when non-empty.
    var startIndex: Int { get }

    /// A "past-the-end" element index; the successor of the last valid
    /// subscript argument.
    var endIndex: Int { get }
    subscript (index: Int) -> T

    /// Return a *generator* over the elements.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<Slice<T>>

    /// A type that can represent a sub-range of a `Slice` 
    typealias SubSlice = Slice<T>
    subscript (subRange: Range<Int>) -> Slice<T>

    /// Initialization from an existing buffer does not have "array.init"
    /// semantics because the caller may retain an alias to buffer.
    init(_ buffer: _SliceBuffer<T>)
}

extension Slice : ArrayLiteralConvertible {

    /// Create an instance containing `elements`.
    init(arrayLiteral elements: T...)
}



extension Slice {

    /// Construct an empty Slice
    init()

    /// Construct from an arbitrary sequence with elements of type `T`
    init<S : SequenceType where T == T>(_ s: S)

    /// Construct a Slice of `count` elements, each initialized to
    /// `repeatedValue`.
    init(count: Int, repeatedValue: T)

    /// How many elements the Slice stores
    var count: Int { get }

    /// How many elements the `Slice` can store without reallocation
    var capacity: Int { get }

    /// `true` if and only if the `Slice` is empty
    var isEmpty: Bool { get }

    /// The first element, or `nil` if the array is empty
    var first: T? { get }

    /// The last element, or `nil` if the array is empty
    var last: T? { get }

    /// Reserve enough space to store minimumCapacity elements.
    ///
    /// PostCondition: `capacity >= minimumCapacity` and the array has
    /// mutable contiguous storage.
    ///
    /// Complexity: O(`count`)
    mutating func reserveCapacity(minimumCapacity: Int)

    /// Append newElement to the Slice
    ///
    /// Complexity: amortized O(1) unless `self`'s storage is shared with another live array; O(`count`) otherwise.
    mutating func append(newElement: T)

    /// Append the elements of `newElements` to `self`.
    ///
    /// Complexity: O(*length of result*) 
    /// 
    mutating func extend<S : SequenceType where T == T>(newElements: S)

    /// Remove an element from the end of the Slice in O(1).
    /// Requires: count > 0
    mutating func removeLast() -> T

    /// Insert `newElement` at index `i`.
    ///
    /// Requires: `i <= count`
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func insert(newElement: T, atIndex i: Int)

    /// Remove and return the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAtIndex(index: Int) -> T

    /// Remove all elements.
    ///
    /// Postcondition: `capacity == 0` iff `keepCapacity` is `false`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeAll(keepCapacity: Bool = default)

    /// Interpose `self` between each consecutive pair of `elements`,
    /// and concatenate the elements of the resulting sequence.  For
    /// example, `[-1, -2].join([[1, 2, 3], [4, 5, 6], [7, 8, 9]])`
    /// yields `[1, 2, 3, -1, -2, 4, 5, 6, -1, -2, 7, 8, 9]`
    func join<S : SequenceType where Slice<T> == Slice<T>>(elements: S) -> Slice<T>

    /// Return the result of repeatedly calling `combine` with an
    /// accumulated value initialized to `initial` and each element of
    /// `self`, in turn, i.e. return
    /// `combine(combine(...combine(combine(initial, self[0]),
    /// self[1]),...self[count-2]), self[count-1])`.
    func reduce<U>(initial: U, combine: (U, T) -> U) -> U

    /// Sort `self` in-place according to `isOrderedBefore`.  Requires:
    /// `isOrderedBefore` induces a `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    mutating func sort(isOrderedBefore: (T, T) -> Bool)

    /// Return a copy of `self` that has been sorted according to
    /// `isOrderedBefore`.  Requires: `isOrderedBefore` induces a
    /// `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    func sorted(isOrderedBefore: (T, T) -> Bool) -> Slice<T>

    /// Return a `Slice` containing the results of calling
    /// `transform(x)` on each element `x` of `self`
    func map<U>(transform: (T) -> U) -> Slice<U>

    /// A Slice containing the elements of `self` in reverse order
    func reverse() -> Slice<T>

    /// Return a `Slice` containing the elements `x` of `self` for which
    /// `includeElement(x)` is `true`
    func filter(includeElement: (T) -> Bool) -> Slice<T>
}

extension Slice : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension Slice : Printable, DebugPrintable {

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}



extension Slice {

    /// Call `body(p)`, where `p` is a pointer to the `Slice`\ 's
    /// contiguous storage.
    ///
    /// Often, the optimizer can eliminate bounds checks within an
    /// array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    func withUnsafeBufferPointer<R>(body: (UnsafeBufferPointer<T>) -> R) -> R

    /// Call `body(p)`, where `p` is a pointer to the `Slice`\ 's
    /// mutable contiguous storage.
    ///
    /// Often, the optimizer can eliminate bounds- and uniqueness-checks
    /// within an array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    mutating func withUnsafeMutableBufferPointer<R>(body: (inout UnsafeMutableBufferPointer<T>) -> R) -> R
}

extension Slice {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if `subRange.endIndex
    /// == self.endIndex` and `isEmpty(newElements)`\ , O(N) otherwise.
    mutating func replaceRange<C : CollectionType where T == T>(subRange: Range<Int>, with newElements: C)

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count + countElements(newElements)`\ ).
    mutating func splice<S : CollectionType where T == T>(newElements: S, atIndex i: Int)

    /// Remove the indicated `subRange` of elements
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeRange(subRange: Range<Int>)
}


/// A *collection* from which a sub-range of elements (a "slice")
/// can be efficiently extracted.
protocol Sliceable : _Sliceable {

    /// The *collection* type that represents a sub-range of elements.
    ///
    /// Though it can't currently be enforced by the type system, the
    /// `SubSlice` type in a concrete implementation of `Sliceable`
    /// should also be `Sliceable`.
    typealias SubSlice : _Sliceable
    subscript (bounds: Range<Self.Index>) -> SubSlice { get }
}


/// An simple string designed to represent text that is "knowable at
/// compile-time".
///
/// Logically speaking, each instance looks something like this::
///
///    enum StaticString {
///       case ASCII(start: UnsafePointer<UInt8>, length: Int)
///       case UTF8(start: UnsafePointer<UInt8>, length: Int)
///       case Scalar(UnicodeScalar)
///    }
struct StaticString : UnicodeScalarLiteralConvertible, ExtendedGraphemeClusterLiteralConvertible, StringLiteralConvertible, Printable, DebugPrintable {

    /// A pointer to the beginning of UTF-8 code units
    ///
    /// Requires: `self` stores a pointer to either ASCII or UTF-8 code
    /// units.
    var utf8Start: UnsafePointer<UInt8> { get }

    /// The stored Unicode scalar value
    ///
    /// Requires: `self` stores a single Unicode scalar value.
    var unicodeScalar: UnicodeScalar { get }

    /// If `self` stores a pointer to ASCII or UTF-8 code units, the
    /// length in bytes of that data.
    ///
    /// If `self` stores a single Unicode scalar value, the value of
    /// `byteSize` is unspecified.
    var byteSize: Word { get }

    /// `true` iff `self` stores a pointer to ASCII or UTF-8 code units
    var hasPointerRepresentation: Bool { get }

    /// `true` if `self` stores a pointer to ASCII code units.
    ///
    /// If `self` stores a single Unicode scalar value, the value of
    /// `isASCII` is unspecified.
    var isASCII: Bool { get }

    /// Invoke `body` with a buffer containing the UTF-8 code units of
    /// `self`.
    ///
    /// This method works regardless of what `self` stores.
    func withUTF8Buffer<R>(body: (UnsafeBufferPointer<UInt8>) -> R) -> R

    /// Return a `String` representing the same sequence of Unicode
    /// scalar values as `self` does.
    var stringValue: String { get }

    /// Create an empty instance.
    init()
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32)

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: StaticString)
    init(_builtinExtendedGraphemeClusterLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1)

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: StaticString)
    init(_builtinStringLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1)

    /// Create an instance initialized to `value`.
    init(stringLiteral value: StaticString)

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// A source of text streaming operations.  `Streamable` instances can
/// be written to any *output stream*.
///
/// For example: `String`, `Character`, `UnicodeScalar`.
protocol Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target)
}


/// A `SequenceType` of values formed by striding over a closed interval
struct StrideThrough<T : Strideable> : SequenceType {

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> StrideThroughGenerator<T>
}

extension StrideThrough : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A GeneratorType for StrideThrough<T>
struct StrideThroughGenerator<T : Strideable> : GeneratorType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    mutating func next() -> T?
}


/// A `SequenceType` of values formed by striding over a half-open interval
struct StrideTo<T : Strideable> : SequenceType {

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> StrideToGenerator<T>
}

extension StrideTo : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A GeneratorType for StrideTo<T>
struct StrideToGenerator<T : Strideable> : GeneratorType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    mutating func next() -> T?
}


/// Conforming types are notionally continuous, one-dimensional
/// values that can be offset and measured.
///
/// See also: `stride(from: to: by:)` and `stride(from: through: by:)`
protocol Strideable : Comparable, _Strideable {
}


/// An arbitrary Unicode string value.
///
/// Unicode-Correct
/// ===============
///
/// Swift strings are designed to be Unicode-correct.  In particular,
/// the APIs make it easy to write code that works correctly, and does
/// not surprise end-users, regardless of where you venture in the
/// Unicode character space.  For example,
///
/// * The `==` operator checks for Unicode canonical equivalence,
///   so two different representations of the same string will always
///   compare equal.
///
/// * String elements are `Characters` (Unicode extended grapheme
///   clusters), a unit of text that is meaningful to most humans.
///
/// Locale-Insensitive
/// ==================
///
/// The fundamental operations on Swift strings are not sensitive to
/// locale settings.  That's because, for example, the validity of a
/// `Dictionary<String, T>` in a running program depends on a given
/// string comparison having a single, stable result.  Therefore,
/// Swift always uses the default, un-tailored Unicode algorithms
/// for basic string operations.
///
/// Importing `Foundation` endows swift strings with the full power of
/// the `NSString` API, which allows you to choose more complex
/// locale-sensitive operations explicitly.
///
/// Value Semantics
/// ===============
///
/// Each string variable, `let` binding, or stored property has an
/// independent value, so mutations to the string are not observable
/// through its copies::
///
///   var a = "foo"
///   var b = a
///   b[b.endIndex.predecessor()] = "x"
///   println("a=\(a), b=\(b)")     // a=foo, b=fox
///
/// Strings use Copy-on-Write so that their data is only copied
/// lazily, upon mutation, when more than one string instance is using
/// the same buffer.  Therefore, the first in any sequence of mutating
/// operations may cost `O(N)` time and space, where `N` is the length
/// of the string's (unspecified) underlying representation,.
///
/// Growth and Capacity
/// ===================
///
/// When a string's contiguous storage fills up, new storage must be
/// allocated and characters must be moved to the new storage.
/// `String` uses an exponential growth strategy that makes `append` a
/// constant time operation *when amortized over many invocations*.
///
/// Objective-C Bridge
/// ==================
///
/// `String` is bridged to Objective-C as `NSString`, and a `String`
/// that originated in Objective-C may store its characters in an
/// `NSString`.  Since any arbitrary subclass of `NSSString` can
/// become a `String`, there are no guarantees about representation or
/// efficiency in this case.  Since `NSString` is immutable, it is
/// just as though the storage was shared by some copy: the first in
/// any sequence of mutating operations causes elements to be copied
/// into unique, contiguous storage which may cost `O(N)` time and
/// space, where `N` is the length of the string representation (or
/// more, if the underlying `NSString` is has unusual performance
/// characteristics).
struct String {
    init()
}

extension String : CollectionType {

    /// A character position in a `String`
    struct Index : BidirectionalIndexType, Comparable, Reflectable {

        /// Returns the next consecutive value after `self`.
        ///
        /// Requires: the next value is representable.
        func successor() -> String.Index

        /// Returns the previous consecutive value before `self`.
        ///
        /// Requires: the previous value is representable.
        func predecessor() -> String.Index

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType
    }

    /// The position of the first `Character` if the `String` is
    /// non-empty; identical to `endIndex` otherwise.
    var startIndex: String.Index { get }

    /// The `String`\ 's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: String.Index { get }
    subscript (i: String.Index) -> Character { get }

    /// Return a *generator* over the `Characters` in this `String`.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<String>
}

extension String {

    /// A collection of UTF-8 code units that encodes a `String` value.
    struct UTF8View : CollectionType, Reflectable {

        /// A position in a `String.UTF8View`
        struct Index : ForwardIndexType {

            /// Returns the next consecutive value after `self`.
            ///
            /// Requires: the next value is representable.
            func successor() -> String.UTF8View.Index
        }

        /// The position of the first code unit if the `String` is
        /// non-empty; identical to `endIndex` otherwise.
        var startIndex: String.UTF8View.Index { get }

        /// The "past the end" position.
        ///
        /// `endIndex` is not a valid argument to `subscript`, and is always
        /// reachable from `startIndex` by zero or more applications of
        /// `successor()`.
        var endIndex: String.UTF8View.Index { get }
        subscript (position: String.UTF8View.Index) -> CodeUnit { get }

        /// Return a *generator* over the code points that comprise this
        /// *sequence*.
        ///
        /// Complexity: O(1)
        func generate() -> IndexingGenerator<String.UTF8View>

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType
    }

    /// A UTF-8 encoding of `self`.
    var utf8: String.UTF8View { get }

    /// A contiguously-stored nul-terminated UTF-8 representation of
    /// `self`.
    ///
    /// To access the underlying memory, invoke
    /// `withUnsafeBufferPointer` on the `ContiguousArray`.
    var nulTerminatedUTF8: ContiguousArray<CodeUnit> { get }
}

extension String {

    /// A collection of Unicode scalar values that
    /// encode a `String` .
    struct UnicodeScalarView : Sliceable, SequenceType, Reflectable {

        /// A position in a `String.UnicodeScalarView`
        struct Index : BidirectionalIndexType, Comparable {

            /// Returns the next consecutive value after `self`.
            ///
            /// Requires: the next value is representable.
            func successor() -> String.UnicodeScalarView.Index

            /// Returns the previous consecutive value before `self`.
            ///
            /// Requires: the previous value is representable.
            func predecessor() -> String.UnicodeScalarView.Index
        }

        /// The position of the first `UnicodeScalar` if the `String` is
        /// non-empty; identical to `endIndex` otherwise.
        var startIndex: String.UnicodeScalarView.Index { get }

        /// The "past the end" position.
        ///
        /// `endIndex` is not a valid argument to `subscript`, and is always
        /// reachable from `startIndex` by zero or more applications of
        /// `successor()`.
        var endIndex: String.UnicodeScalarView.Index { get }
        subscript (position: String.UnicodeScalarView.Index) -> UnicodeScalar { get }
        subscript (r: Range<String.UnicodeScalarView.Index>) -> String.UnicodeScalarView { get }

        /// A type whose instances can produce the elements of this
        /// sequence, in order.
        struct Generator : GeneratorType {

            /// Advance to the next element and return it, or `nil` if no next
            /// element exists.
            ///
            /// Requires: no preceding call to `self.next()` has returned
            /// `nil`.
            mutating func next() -> UnicodeScalar?
        }

        /// Return a *generator* over the `UnicodeScalar`\ s that comprise
        /// this *sequence*.
        ///
        /// Complexity: O(1)
        func generate() -> String.UnicodeScalarView.Generator

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType
    }
}

extension String {

    /// Creates a new `String` by copying the nul-terminated UTF-8 data
    /// referenced by a `CString`.
    ///
    /// Returns `nil` if the `CString` is `NULL` or if it contains ill-formed
    /// UTF-8 code unit sequences.
    static func fromCString(cs: UnsafePointer<CChar>) -> String?

    /// Creates a new `String` by copying the nul-terminated UTF-8 data
    /// referenced by a `CString`.
    ///
    /// Returns `nil` if the `CString` is `NULL`.  If `CString` contains
    /// ill-formed UTF-8 code unit sequences, replaces them with replacement
    /// characters (U+FFFD).
    static func fromCStringRepairingIllFormedUTF8(cs: UnsafePointer<CChar>) -> (String?, hadError: Bool)
}

extension String {

    /// Construct an instance containing just the given `Character`.
    init(_ c: Character)
}

extension String {

    /// Invoke `f` on the contents of this string, represented as
    /// a nul-terminated array of char, ensuring that the array's
    /// lifetime extends through the execution of `f`.
    func withCString<Result>(f: (UnsafePointer<Int8>) -> Result) -> Result
}

extension String : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension String : OutputStreamType {
    mutating func write(other: String)
}

extension String : Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target)
}



extension String {
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32)
}

extension String : UnicodeScalarLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: String)
}

extension String {
    init(_builtinExtendedGraphemeClusterLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1)
}

extension String : ExtendedGraphemeClusterLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: String)
}

extension String {
    init(_builtinUTF16StringLiteral start: Builtin.RawPointer, numberOfCodeUnits: Builtin.Word)
}

extension String {
    init(_builtinStringLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1)
}

extension String : StringLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(stringLiteral value: String)
}

extension String : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}



extension String : Equatable {
}

extension String : Comparable {
}

extension String {

    /// Append the elements of `other` to `self`.
    mutating func extend(other: String)

    /// Append `x` to `self`.
    ///
    /// Complexity: amortized O(1).
    mutating func append(x: UnicodeScalar)
}

extension String : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension String : StringInterpolationConvertible {

    /// Create an instance by concatenating the elements of `strings`
    static func convertFromStringInterpolation(strings: String...) -> String

    /// Create an instance containing `expr`\ 's `print` representation
    static func convertFromStringInterpolationSegment<T>(expr: T) -> String
}

extension String : Comparable {
}



extension String : Sliceable {
    subscript (subRange: Range<String.Index>) -> String { get }
}

extension String : ExtensibleCollectionType {

    /// Reserve enough space to store `n` ASCII characters.
    ///
    /// Complexity: O(`n`)
    mutating func reserveCapacity(n: Int)

    /// Append `c` to `self`.
    ///
    /// Complexity: amortized O(1).
    mutating func append(c: Character)

    /// Append the elements of `newElements` to `self`.
    mutating func extend<S : SequenceType where Character == Character>(newElements: S)

    /// Create an instance containing `characters`.
    init<S : SequenceType where Character == Character>(_ characters: S)
}

extension String {

    /// Interpose `self` between every pair of consecutive `elements`,
    /// then concatenate the result.  For example::
    ///
    ///   "-|-".join(["foo", "bar", "baz"]) // "foo-|-bar-|-baz"
    func join<S : SequenceType where String == String>(elements: S) -> String
}

extension String : RangeReplaceableCollectionType {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if `subRange.endIndex
    /// == self.endIndex` and `isEmpty(newElements)`\ , O(N) otherwise.
    mutating func replaceRange<C : CollectionType where Character == Character>(subRange: Range<String.Index>, with newElements: C)

    /// Insert `newElement` at index `i`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func insert(newElement: Character, atIndex i: String.Index)

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self) + countElements(newElements)`\ ).
    mutating func splice<S : CollectionType where Character == Character>(newElements: S, atIndex i: String.Index)

    /// Remove and return the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeAtIndex(i: String.Index) -> Character

    /// Remove the indicated `subRange` of characters
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeRange(subRange: Range<String.Index>)

    /// Remove all characters.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// :param: `keepCapacity`, if `true`, prevents the release of
    ////   allocated storage, which can be a useful optimization
    ///    when `self` is going to be grown again.
    mutating func removeAll(keepCapacity: Bool = default)
}



extension String : StringInterpolationConvertible {
    static func convertFromStringInterpolationSegment(expr: String) -> String
    static func convertFromStringInterpolationSegment(expr: Character) -> String
    static func convertFromStringInterpolationSegment(expr: UnicodeScalar) -> String
    static func convertFromStringInterpolationSegment(expr: Bool) -> String
    static func convertFromStringInterpolationSegment(expr: Float32) -> String
    static func convertFromStringInterpolationSegment(expr: Float64) -> String
    static func convertFromStringInterpolationSegment(expr: UInt8) -> String
    static func convertFromStringInterpolationSegment(expr: Int8) -> String
    static func convertFromStringInterpolationSegment(expr: UInt16) -> String
    static func convertFromStringInterpolationSegment(expr: Int16) -> String
    static func convertFromStringInterpolationSegment(expr: UInt32) -> String
    static func convertFromStringInterpolationSegment(expr: Int32) -> String
    static func convertFromStringInterpolationSegment(expr: UInt64) -> String
    static func convertFromStringInterpolationSegment(expr: Int64) -> String
    static func convertFromStringInterpolationSegment(expr: UInt) -> String
    static func convertFromStringInterpolationSegment(expr: Int) -> String
}

extension String {

    /// Construct an instance that is the concatenation of `sz` copies
    /// of `repeatedValue`
    init(count sz: Int, repeatedValue c: Character)

    /// Construct an instance that is the concatenation of `sz` copies
    /// of `Character(repeatedValue)`
    init(count: Int, repeatedValue c: UnicodeScalar)

    /// `true` iff `self` contains no characters.
    var isEmpty: Bool { get }
}



extension String {

    /// Return `true` iff `self` begins with `prefix`
    func hasPrefix(prefix: String) -> Bool

    /// Return `true` iff `self` ends with `suffix`
    func hasSuffix(suffix: String) -> Bool
}

extension String {

    /// Create an instance representing `v` in base 10.
    init<T : _SignedIntegerType>(_ v: T)

    /// Create an instance representing `v` in base 10.
    init<T : _UnsignedIntegerType>(_ v: T)

    /// Create an instance representing `v` in the given `radix` (base).
    ///
    /// Numerals greater than 9 are represented as roman letters,
    /// starting with `a` if `uppercase` is `false` or `A` otherwise.
    init<T : _SignedIntegerType>(_ v: T, radix: Int, uppercase: Bool = default)

    /// Create an instance representing `v` in the given `radix` (base).
    ///
    /// Numerals greater than 9 are represented as roman letters,
    /// starting with `a` if `uppercase` is `false` or `A` otherwise.
    init<T : _UnsignedIntegerType>(_ v: T, radix: Int, uppercase: Bool = default)
}

extension String {

    /// If the string represents an integer that fits into an Int, returns
    /// the corresponding integer.  This accepts strings that match the regular
    /// expression "[-+]?[0-9]+" only.
    func toInt() -> Int?
}



extension String {

    /// A collection of UTF-16 code units that encodes a `String` value.
    struct UTF16View : Sliceable, Reflectable {

        /// The position of the first code unit if the `String` is
        /// non-empty; identical to `endIndex` otherwise.
        var startIndex: Int { get }

        /// The "past the end" position.
        ///
        /// `endIndex` is not a valid argument to `subscript`, and is always
        /// reachable from `startIndex` by zero or more applications of
        /// `successor()`.
        var endIndex: Int { get }

        /// A type whose instances can produce the elements of this
        /// sequence, in order.
        typealias Generator

        /// Return a *generator* over the code points that comprise this
        /// *sequence*.
        ///
        /// Complexity: O(1)
        func generate() -> Generator
        subscript (position: Int) -> UInt16 { get }
        subscript (subRange: Range<Int>) -> String.UTF16View { get }

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType
    }

    /// A UTF-16 encoding of `self`.
    var utf16: String.UTF16View { get }
}







extension String {

    /// Construct an instance given a collection of Unicode scalar
    /// values.
    init(_ view: String.UnicodeScalarView)

    /// The value of `self` as a collection of Unicode scalar values.
    var unicodeScalars: String.UnicodeScalarView
}


/// Conforming types can be initialized with string interpolations
/// containing `\(`\ ...\ `)` clauses.
protocol StringInterpolationConvertible {

    /// Create an instance by concatenating the elements of `strings`
    class func convertFromStringInterpolation(strings: Self...) -> Self

    /// Create an instance containing `expr`\ 's `print` representation
    class func convertFromStringInterpolationSegment<T>(expr: T) -> Self
}


/// Conforming types can be initialized with arbitrary string literals
protocol StringLiteralConvertible : ExtendedGraphemeClusterLiteralConvertible {
    typealias StringLiteralType

    /// Create an instance initialized to `value`.
    init(stringLiteral value: StringLiteralType)
}


/// The default type for an otherwise-unconstrained string literal
typealias StringLiteralType = String


/// A 64-bit unsigned integer value
/// type.
struct UInt : UnsignedIntegerType {
    var value: Builtin.Word

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: UInt)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: UInt)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: UInt)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: UInt)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: UInt { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: UInt { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: UInt { get }
    static var max: UInt { get }
    static var min: UInt { get }
}

extension UInt : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension UInt : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension UInt : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UInt

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UInt

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: UInt) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> UInt
}

extension UInt {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: UInt, _ rhs: UInt) -> (UInt, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: UInt, _ rhs: UInt) -> (UInt, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: UInt, _ rhs: UInt) -> (UInt, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: UInt, _ rhs: UInt) -> (UInt, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: UInt, _ rhs: UInt) -> (UInt, overflow: Bool)

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Explicitly convert to `IntMax`, trapping on overflow (except in -Ounchecked builds).
    func toIntMax() -> IntMax
}

extension UInt {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: UInt64)

    /// Construct a `UInt` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `UInt` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: Int)

    /// Construct a `UInt` having the same memory representation as
    /// the `Int` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `UInt` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: Int)
}

extension UInt : BitwiseOperationsType {

    /// The empty bitset of type UInt.
    static var allZeros: UInt { get }
}

extension UInt {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension UInt : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UInt : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 16-bit unsigned integer value
/// type.
struct UInt16 : UnsignedIntegerType {
    var value: Builtin.Int16

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: UInt16)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: UInt16)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: UInt16)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: UInt16)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: UInt16 { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: UInt16 { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: UInt16 { get }
    static var max: UInt16 { get }
    static var min: UInt16 { get }
}

extension UInt16 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension UInt16 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension UInt16 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UInt16

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UInt16

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: UInt16) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> UInt16
}

extension UInt16 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool)

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Explicitly convert to `IntMax`.
    func toIntMax() -> IntMax
}

extension UInt16 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: Int16)
    init(_ v: UInt32)

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt32)
    init(_ v: Int32)

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int32)
    init(_ v: UInt64)

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt)
    init(_ v: Int)

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int)

    /// Construct a `UInt16` having the same memory representation as
    /// the `Int16` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `UInt16` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: Int16)
}

extension UInt16 : BitwiseOperationsType {

    /// The empty bitset of type UInt16.
    static var allZeros: UInt16 { get }
}

extension UInt16 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension UInt16 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UInt16 {
}

extension UInt16 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 32-bit unsigned integer value
/// type.
struct UInt32 : UnsignedIntegerType {
    var value: Builtin.Int32

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: UInt32)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: UInt32)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: UInt32)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: UInt32)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: UInt32 { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: UInt32 { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: UInt32 { get }
    static var max: UInt32 { get }
    static var min: UInt32 { get }
}

extension UInt32 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension UInt32 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension UInt32 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UInt32

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UInt32

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: UInt32) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> UInt32
}

extension UInt32 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: UInt32, _ rhs: UInt32) -> (UInt32, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: UInt32, _ rhs: UInt32) -> (UInt32, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: UInt32, _ rhs: UInt32) -> (UInt32, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: UInt32, _ rhs: UInt32) -> (UInt32, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: UInt32, _ rhs: UInt32) -> (UInt32, overflow: Bool)

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Explicitly convert to `IntMax`.
    func toIntMax() -> IntMax
}

extension UInt32 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: Int32)
    init(_ v: UInt64)

    /// Construct a `UInt32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `UInt32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `UInt32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt)
    init(_ v: Int)

    /// Construct a `UInt32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int)

    /// Construct a `UInt32` having the same memory representation as
    /// the `Int32` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `UInt32` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: Int32)
}

extension UInt32 : BitwiseOperationsType {

    /// The empty bitset of type UInt32.
    static var allZeros: UInt32 { get }
}

extension UInt32 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension UInt32 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UInt32 {

    /// Construct with value `v.value`.
    ///
    /// Requires: `v.value` can be represented as UInt32.
    init(_ v: UnicodeScalar)
}

extension UInt32 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 64-bit unsigned integer value
/// type.
struct UInt64 : UnsignedIntegerType {
    var value: Builtin.Int64

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: UInt64)

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: UInt64)

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: UInt64)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: UInt64)

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: UInt64 { get }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: UInt64 { get }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: UInt64 { get }
    static var max: UInt64 { get }
    static var min: UInt64 { get }
}

extension UInt64 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension UInt64 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension UInt64 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UInt64

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UInt64

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: UInt64) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> UInt64
}

extension UInt64 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: UInt64, _ rhs: UInt64) -> (UInt64, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: UInt64, _ rhs: UInt64) -> (UInt64, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: UInt64, _ rhs: UInt64) -> (UInt64, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: UInt64, _ rhs: UInt64) -> (UInt64, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: UInt64, _ rhs: UInt64) -> (UInt64, overflow: Bool)

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Explicitly convert to `IntMax`, trapping on overflow (except in -Ounchecked builds).
    func toIntMax() -> IntMax
}

extension UInt64 {
    init(_ v: UInt8)
    init(_ v: Int8)
    init(_ v: UInt16)
    init(_ v: Int16)
    init(_ v: UInt32)
    init(_ v: Int32)
    init(_ v: Int64)
    init(_ v: UInt)
    init(_ v: Int)

    /// Construct a `UInt64` having the same memory representation as
    /// the `Int64` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `UInt64` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: Int64)
}

extension UInt64 : BitwiseOperationsType {

    /// The empty bitset of type UInt64.
    static var allZeros: UInt64 { get }
}

extension UInt64 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension UInt64 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UInt64 {

    /// Construct with value `v.value`.
    ///
    /// Requires: `v.value` can be represented as UInt64.
    init(_ v: UnicodeScalar)
}

extension UInt64 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// A 8-bit unsigned integer value
/// type.
struct UInt8 : UnsignedIntegerType {
    var value: Builtin.Int8

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init()

    /// Create an instance initialized to `value`.
    init(_ value: UInt8)
    init(_builtinIntegerLiteral value: Builtin.Int2048)

    /// Create an instance initialized to `value`.
    init(integerLiteral value: UInt8)
    static var max: UInt8 { get }
    static var min: UInt8 { get }
}

extension UInt8 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension UInt8 : Printable {

    /// A textual representation of `self`.
    var description: String { get }
}

extension UInt8 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UInt8

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UInt8

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: UInt8) -> Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> UInt8
}

extension UInt8 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: UInt8, _ rhs: UInt8) -> (UInt8, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: UInt8, _ rhs: UInt8) -> (UInt8, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: UInt8, _ rhs: UInt8) -> (UInt8, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: UInt8, _ rhs: UInt8) -> (UInt8, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: UInt8, _ rhs: UInt8) -> (UInt8, overflow: Bool)

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Explicitly convert to `IntMax`.
    func toIntMax() -> IntMax
}

extension UInt8 {
    init(_ v: Int8)
    init(_ v: UInt16)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt16)
    init(_ v: Int16)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int16)
    init(_ v: UInt32)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt32)
    init(_ v: Int32)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int32)
    init(_ v: UInt64)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64)
    init(_ v: Int64)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64)
    init(_ v: UInt)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt)
    init(_ v: Int)

    /// Construct a `UInt8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int)

    /// Construct a `UInt8` having the same memory representation as
    /// the `Int8` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `UInt8` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: Int8)
}

extension UInt8 : BitwiseOperationsType {

    /// The empty bitset of type UInt8.
    static var allZeros: UInt8 { get }
}

extension UInt8 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float)

    /// Construct an instance that approximates `other`.
    init(_ other: Double)

    /// Construct an instance that approximates `other`.
    init(_ other: Float80)
}

extension UInt8 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UInt8 {
}

extension UInt8 {

    /// Construct with value `v.value`.
    ///
    /// Requires: `v.value` can be represented as UInt8.
    init(_ v: UnicodeScalar)
}

extension UInt8 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// The largest native unsigned integer type
typealias UIntMax = UInt64


/// A codec for UTF-16.
struct UTF16 : UnicodeCodecType {

    /// A type that can hold code unit values for this
    /// encoding.
    typealias CodeUnit = UInt16
    init()

    /// Start or continue decoding a UTF sequence.
    ///
    /// In order to decode a code unit sequence completely, this function should
    /// be called repeatedly until it returns `UnicodeDecodingResult.EmptyInput`.
    /// Checking that the generator was exhausted is not sufficient.  The decoder
    /// can have an internal buffer that is pre-filled with data from the input
    /// generator.
    ///
    /// Because of buffering, it is impossible to find the corresponing position
    /// in the generator for a given returned `UnicodeScalar` or an error.
    ///
    /// :param: `next`: a *generator* of code units to be decoded.
    mutating func decode<G : GeneratorType where CodeUnit == CodeUnit>(inout input: G) -> UnicodeDecodingResult

    /// Encode a `UnicodeScalar` as a series of `CodeUnit`\ s by `put`\
    /// 'ing each `CodeUnit` to `output`.
    static func encode<S : SinkType where CodeUnit == CodeUnit>(input: UnicodeScalar, inout output: S)
}

extension UTF16 {

    /// Return the number of code units required to encode `x`.
    static func width(x: UnicodeScalar) -> Int

    /// Return the high surrogate code unit of a surrogate pair
    /// representing `x`.
    ///
    /// Requires: `width(x) == 2`
    static func leadSurrogate(x: UnicodeScalar) -> CodeUnit

    /// Return the low surrogate code unit of a surrogate pair
    /// representing `x`.
    ///
    /// Requires: `width(x) == 2`
    static func trailSurrogate(x: UnicodeScalar) -> CodeUnit

    /// Returns the number of UTF-16 code units required for the given code unit
    /// sequence when transcoded to UTF-16, and a bit describing if the sequence
    /// was found to contain only ASCII characters.
    ///
    /// If `repairIllFormedSequences` is `true`, the function always succeeds.
    /// If it is `false`, `nil` is returned if an ill-formed code unit sequence is
    /// found in `input`.
    static func measure<Encoding : UnicodeCodecType, Input : GeneratorType where Encoding.CodeUnit == Encoding.CodeUnit>(_: Encoding.Type, input: Input, repairIllFormedSequences: Bool) -> (Int, Bool)?
}


/// A codec for UTF-32.
struct UTF32 : UnicodeCodecType {

    /// A type that can hold code unit values for this
    /// encoding.
    typealias CodeUnit = UInt32
    init()

    /// Start or continue decoding a UTF sequence.
    ///
    /// In order to decode a code unit sequence completely, this function should
    /// be called repeatedly until it returns `UnicodeDecodingResult.EmptyInput`.
    /// Checking that the generator was exhausted is not sufficient.  The decoder
    /// can have an internal buffer that is pre-filled with data from the input
    /// generator.
    ///
    /// Because of buffering, it is impossible to find the corresponing position
    /// in the generator for a given returned `UnicodeScalar` or an error.
    ///
    /// :param: `next`: a *generator* of code units to be decoded.
    mutating func decode<G : GeneratorType where CodeUnit == CodeUnit>(inout input: G) -> UnicodeDecodingResult

    /// Encode a `UnicodeScalar` as a series of `CodeUnit`\ s by `put`\
    /// 'ing each `CodeUnit` to `output`.
    static func encode<S : SinkType where CodeUnit == CodeUnit>(input: UnicodeScalar, inout output: S)
}


/// A codec for UTF-8.
struct UTF8 : UnicodeCodecType {

    /// A type that can hold code unit values for this
    /// encoding.
    typealias CodeUnit = UInt8
    init()

    /// Start or continue decoding a UTF sequence.
    ///
    /// In order to decode a code unit sequence completely, this function should
    /// be called repeatedly until it returns `UnicodeDecodingResult.EmptyInput`.
    /// Checking that the generator was exhausted is not sufficient.  The decoder
    /// can have an internal buffer that is pre-filled with data from the input
    /// generator.
    ///
    /// Because of buffering, it is impossible to find the corresponing position
    /// in the generator for a given returned `UnicodeScalar` or an error.
    ///
    /// :param: `next`: a *generator* of code units to be decoded.
    mutating func decode<G : GeneratorType where CodeUnit == CodeUnit>(inout next: G) -> UnicodeDecodingResult

    /// Encode a `UnicodeScalar` as a series of `CodeUnit`\ s by `put`\
    /// 'ing each `CodeUnit` to `output`.
    static func encode<S : SinkType where CodeUnit == CodeUnit>(input: UnicodeScalar, inout output: S)
}


/// An unsigned integer type that occupies one machine word
typealias UWord = UInt


/// A Unicode encoding scheme.
///
/// Consists of an underlying `CodeUnit` type and functions to
/// translate between sequences of these code units and Unicode
/// scalar values.
protocol UnicodeCodecType {

    /// A type that can hold code unit values for this
    /// encoding.
    typealias CodeUnit
    init()

    /// Start or continue decoding a UTF sequence.
    ///
    /// In order to decode a code unit sequence completely, this function should
    /// be called repeatedly until it returns `UnicodeDecodingResult.EmptyInput`.
    /// Checking that the generator was exhausted is not sufficient.  The decoder
    /// can have an internal buffer that is pre-filled with data from the input
    /// generator.
    ///
    /// Because of buffering, it is impossible to find the corresponing position
    /// in the generator for a given returned `UnicodeScalar` or an error.
    ///
    /// :param: `next`: a *generator* of code units to be decoded.
    mutating func decode<G : GeneratorType where `Self`.CodeUnit == CodeUnit>(inout next: G) -> UnicodeDecodingResult

    /// Encode a `UnicodeScalar` as a series of `CodeUnit`\ s by `put`\
    /// 'ing each `CodeUnit` to `output`.
    class func encode<S : SinkType where `Self`.CodeUnit == CodeUnit>(input: UnicodeScalar, inout output: S)
}


/// The result of one Unicode decoding step
///
/// A unicode scalar value, an indication that no more unicode scalars
/// are available, or an indication of a decoding error.
enum UnicodeDecodingResult {
    case Result(UnicodeScalar)
    case EmptyInput
    case Error

    /// Return true if `self` indicates no more unicode scalars are
    /// available.
    func isEmptyInput() -> Bool
}


/// A Unicode scalar value.
struct UnicodeScalar : UnicodeScalarLiteralConvertible {

    /// A numeric representation of `self`.
    var value: UInt32 { get }
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32)

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: UnicodeScalar)

    /// Creates an instance of the NUL scalar value.
    init()

    /// Create an instance with numeric value `v`.
    ///
    /// Requires: `v` is a valid Unicode scalar value.
    init(_ v: UInt32)

    /// Create an instance with numeric value `v`.
    ///
    /// Requires: `v` is a valid Unicode scalar value.
    init(_ v: UInt16)

    /// Create an instance with numeric value `v`.
    init(_ v: UInt8)

    /// Create a duplicate of `v`.
    init(_ v: UnicodeScalar)

    /// Return a String representation of `self` .
    ///
    /// :param: `asASCII`, if `true`, forces most values into a numeric
    /// representation.
    func escape(#asASCII: Bool) -> String

    /// Returns true if this is an ASCII character (code point 0 to 127
    /// inclusive).
    func isASCII() -> Bool
}

extension UnicodeScalar : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UnicodeScalar : Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target)
}

extension UnicodeScalar : Printable, DebugPrintable {

    /// A textual representation of `self`.
    var description: String { get }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}

extension UnicodeScalar : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}

extension UnicodeScalar {

    /// Construct with value `v`.
    ///
    /// Requires: `v` is a valid unicode scalar value.
    init(_ v: Int)
}

extension UnicodeScalar : Comparable {
}

extension UnicodeScalar {
}


/// Conforming types can be initialized with string literals
/// containing a single Unicode scalar value.
protocol UnicodeScalarLiteralConvertible {
    typealias UnicodeScalarLiteralType

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: UnicodeScalarLiteralType)
}


/// The default type for an otherwise-unconstrained unicode scalar literal
typealias UnicodeScalarType = String


/// A type for propagating an unmanaged object reference.
///
/// When you use this type, you become partially responsible for
/// keeping the object alive.
struct Unmanaged<T> {

    /// Unsafely turn an opaque C pointer into an unmanaged
    /// class reference.
    ///
    /// This operation does not change reference counts.
    ///
    /// ::
    ///
    ///   let str: CFString = Unmanaged.fromOpaque(ptr).takeUnretainedValue()
    static func fromOpaque(value: COpaquePointer) -> Unmanaged<T>

    /// Unsafely turn an unmanaged class reference into an opaque
    /// C pointer.
    ///
    /// This operation does not change reference counts.
    ///
    /// ::
    ///
    ///   let str: CFString = Unmanaged.fromOpaque(ptr).takeUnretainedValue()
    func toOpaque() -> COpaquePointer

    /// Create an unmanaged reference with an unbalanced retain.
    /// The object will leak if nothing eventually balances the retain.
    ///
    /// This is useful when passing an object to an API which Swift
    /// does not know the ownership rules for, but you know that the
    /// API expects you to pass the object at +1.
    static func passRetained(value: T) -> Unmanaged<T>

    /// Create an unmanaged reference without performing an unbalanced
    /// retain.
    ///
    /// This is useful when passing a reference to an API which Swift
    /// does not know the ownership rules for, but you know that the
    /// API expects you to pass the object at +0.
    ///
    /// ::
    ///
    ///   CFArraySetValueAtIndex(.passUnretained(array), i,
    ///                          .passUnretained(object))
    static func passUnretained(value: T) -> Unmanaged<T>

    /// Get the value of this unmanaged reference as a managed
    /// reference without consuming an unbalanced retain of it.
    ///
    /// This is useful when a function returns an unmanaged reference
    /// and you know that you're not responsible for releasing the result.
    func takeUnretainedValue() -> T

    /// Get the value of this unmanaged reference as a managed
    /// reference and consume an unbalanced retain of it.
    ///
    /// This is useful when a function returns an unmanaged reference
    /// and you know that you're responsible for releasing the result.
    func takeRetainedValue() -> T

    /// Perform an unbalanced retain of the object.
    func retain() -> Unmanaged<T>

    /// Perform an unbalanced release of the object.
    func release()

    /// Perform an unbalanced autorelease of the object.
    func autorelease() -> Unmanaged<T>
}


/// A non-owning pointer to buffer of  `T`\ s stored
/// contiguously in memory, presenting a `Collection` interface to the
/// underlying elements.
struct UnsafeBufferPointer<T> : CollectionType {

    /// Always zero, which is the index of the first element in a
    /// non-empty buffer.
    var startIndex: Int { get }

    /// The "past the end" position; always identical to `count`.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Int { get }
    subscript (i: Int) -> T { get }

    /// Construct an UnsafePointer over the `count` contiguous
    /// `T` instances beginning at `start`.
    init(start: UnsafePointer<T>, count: Int)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> UnsafeBufferPointerGenerator<T>

    /// A pointer to the first element of the buffer
    var baseAddress: UnsafePointer<T> { get }

    /// The number of elements in the buffer
    var count: Int { get }
}

extension UnsafeBufferPointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// A generator for the elements in the buffer referenced by
/// `UnsafeBufferPointer` or `UnsafeMutableBufferPointer`
struct UnsafeBufferPointerGenerator<T> : GeneratorType, SequenceType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    mutating func next() -> T?

    /// `UnsafeBufferPointerGenerator` is also a `SequenceType`, so it
    /// `generate`\ 's a copy of itself
    func generate() -> UnsafeBufferPointerGenerator<T>
}


/// A non-owning pointer to buffer of mutable `T`\ s stored
/// contiguously in memory, presenting a `Collection` interface to the
/// underlying elements.
struct UnsafeMutableBufferPointer<T> : MutableCollectionType {

    /// Always zero, which is the index of the first element in a
    /// non-empty buffer.
    var startIndex: Int { get }

    /// The "past the end" position; always identical to `count`.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Int { get }
    subscript (i: Int) -> T { get nonmutating set }

    /// Construct an UnsafeMutablePointer over the `count` contiguous
    /// `T` instances beginning at `start`.
    init(start: UnsafeMutablePointer<T>, count: Int)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> UnsafeBufferPointerGenerator<T>

    /// A pointer to the first element of the buffer
    var baseAddress: UnsafeMutablePointer<T> { get }

    /// The number of elements in the buffer
    var count: Int { get }
}

extension UnsafeMutableBufferPointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}


/// A pointer to an object of type `T`.  This type provides no automated
/// memory management, and therefore the user must take care to allocate
/// and free memory appropriately.
///
/// The pointer can be in one of the following states:
///
/// - memory is not allocated (for example, pointer is null, or memory has
/// been deallocated previously);
///
/// - memory is allocated, but value has not been initialized;
///
/// - memory is allocated and value is initialized.
struct UnsafeMutablePointer<T> : RandomAccessIndexType, Hashable, NilLiteralConvertible {

    /// Construct a null pointer.
    init()

    /// Construct an `UnsafeMutablePointer` from a builtin raw pointer.
    init(_ value: Builtin.RawPointer)

    /// Convert from an opaque C pointer to a typed pointer
    ///
    /// This is a fundamentally unsafe conversion.
    init(_ other: COpaquePointer)

    /// Construct an `UnsafeMutablePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: Word)

    /// Construct an `UnsafeMutablePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: UWord)

    /// Convert from an UnsafeMutablePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafeMutablePointer<U>)

    /// Convert from a UnsafePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafePointer<U>)

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())

    /// Return a `nil` instance.
    static func null() -> UnsafeMutablePointer<T>

    /// Allocate memory for `num` objects of type `T`.
    ///
    /// Postcondition: the memory is allocated, but not initialized.
    static func alloc(num: Int) -> UnsafeMutablePointer<T>

    /// Deallocate `num` objects.
    ///
    /// :param: num number of objects to deallocate.  Should match exactly
    /// the value that was passed to `alloc()` (partial deallocations are not
    /// possible).
    ///
    /// Precondition: the memory is not initialized.
    ///
    /// Postcondition: the memory has been deallocated.
    func dealloc(num: Int)

    /// Access the underlying raw memory, getting and
    /// setting values.
    var memory: T { get nonmutating set }

    /// Initialize the value the pointer points to, to construct
    /// an object where there was no object previously stored.
    ///
    /// Precondition: the memory is not initialized.
    ///
    /// Postcondition: the memory is initalized; the value should eventually
    /// be destroyed or moved from to avoid leaks.
    func initialize(newvalue: T)

    /// Retrieve the value the pointer points to, moving it away
    /// from the location referenced in memory.
    ///
    /// Equivalent to reading `memory` property and calling `destroy()`,
    /// but more efficient.
    ///
    /// Precondition: the memory is initialized.
    ///
    /// Postcondition: the value has been destroyed and the memory must
    /// be initialized before being used again.
    func move() -> T

    /// Move count values beginning at source into uninitialized memory,
    /// transforming the source values into raw memory, proceeding from
    /// the last value to the first.  Use this for copying ranges into
    /// later memory that may overlap with the source range.
    ///
    /// Requires: either `source` precedes `self` or follows `self + count`.
    func moveInitializeBackwardFrom(source: UnsafeMutablePointer<T>, count: Int)

    /// Assign from count values beginning at source into initialized
    /// memory, transforming the source values into raw memory.
    func moveAssignFrom(source: UnsafeMutablePointer<T>, count: Int)

    /// Assign from `count` values beginning at source into initialized
    /// memory, proceeding from the first element to the last.
    func assignFrom(source: UnsafeMutablePointer<T>, count: Int)

    /// Assign from `count` values beginning at `source` into
    /// initialized memory, proceeding from the last value to the first.
    /// Use this for assigning ranges into later memory that may overlap
    /// with the source range.
    ///
    /// Requires: either `source` precedes `self` or follows `self + count`.
    func assignBackwardFrom(source: UnsafeMutablePointer<T>, count: Int)

    /// Move count values beginning at source into raw memory,
    /// transforming the source values into raw memory.
    func moveInitializeFrom(source: UnsafeMutablePointer<T>, count: Int)

    /// Copy count values beginning at source into raw memory.
    ///
    /// Precondition: the memory is not initialized.
    func initializeFrom(source: UnsafeMutablePointer<T>, count: Int)

    /// Copy the elements of `C` into raw memory.
    ///
    /// Precondition: the memory is not initialized.
    func initializeFrom<C : CollectionType where T == T>(source: C)

    /// Destroy the object the pointer points to.
    ///
    /// Precondition: the memory is initialized.
    ///
    /// Postcondition: the value has been destroyed and the memory must
    /// be initialized before being used again.
    func destroy()

    /// Destroy the `count` objects the pointer points to.
    /// Precondition: the memory is initialized.
    ///
    /// Postcondition: the value has been destroyed and the memory must
    /// be initialized before being used again.
    func destroy(count: Int)
    subscript (i: Int) -> T { get nonmutating set }

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UnsafeMutablePointer<T>

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UnsafeMutablePointer<T>

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(x: UnsafeMutablePointer<T>) -> Int

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(n: Int) -> UnsafeMutablePointer<T>
}

extension UnsafeMutablePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}

extension UnsafeMutablePointer : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}

extension UnsafeMutablePointer : SinkType {
    mutating func put(x: T)
}


/// A pointer to an object of type `T`.  This type provides no automated
/// memory management, and therefore the user must take care to allocate
/// and free memory appropriately.
///
/// The pointer can be in one of the following states:
///
/// - memory is not allocated (for example, pointer is null, or memory has
/// been deallocated previously);
///
/// - memory is allocated, but value has not been initialized;
///
/// - memory is allocated and value is initialized.
struct UnsafePointer<T> : RandomAccessIndexType, Hashable, NilLiteralConvertible {

    /// Construct a null pointer.
    init()

    /// Construct an `UnsafePointer` from a builtin raw pointer.
    init(_ value: Builtin.RawPointer)

    /// Convert from an opaque C pointer to a typed pointer
    ///
    /// This is a fundamentally unsafe conversion.
    init(_ other: COpaquePointer)

    /// Construct an `UnsafePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: Word)

    /// Construct an `UnsafePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: UWord)

    /// Convert from an UnsafeMutablePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafeMutablePointer<U>)

    /// Convert from a UnsafePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafePointer<U>)

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())

    /// Return a `nil` instance.
    static func null() -> UnsafePointer<T>

    /// Access the underlying raw memory, getting and
    /// setting values.
    var memory: T { get }
    subscript (i: Int) -> T { get }

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UnsafePointer<T>

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UnsafePointer<T>

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(x: UnsafePointer<T>) -> Int

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(n: Int) -> UnsafePointer<T>
}

extension UnsafePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}

extension UnsafePointer : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}


/// A set of common requirements for Swift's unsigned integer types.
protocol UnsignedIntegerType : _UnsignedIntegerType, IntegerType {
}


/// An object that can manage the lifetime of storage backing a
/// `CVaListPointer`
final class VaListBuilder {
}


/// The empty tuple type.
///
/// This is the default return type of functions for which no explicit
/// return type is specified.
typealias Void = ()


/// A signed integer type that occupies one machine word
typealias Word = Int


/// A sequence of pairs built out of two underlying sequences, where
/// the elements of the `i`\ th pair are the `i`\ th elements of each
/// underlying sequence.
struct Zip2<S0 : SequenceType, S1 : SequenceType> : SequenceType {
    typealias Stream1 = S0.Generator
    typealias Stream2 = S1.Generator

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = ZipGenerator2<S0.Generator, S1.Generator>

    /// Construct an instance that makes pairs of elements from `s0` and
    /// `s1`.
    init(_ s0: S0, _ s1: S1)

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> Generator
}


/// A generator for the `Zip2` sequence
struct ZipGenerator2<E0 : GeneratorType, E1 : GeneratorType> : GeneratorType {

    /// The type of element returned by `next()`.
    typealias Element = (E0.Element, E1.Element)

    /// Construct around a pair of underlying generators.
    init(_ e0: E0, _ e1: E1)

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: `next()` has not been applied to a copy of `self`
    /// since the copy was made, and no preceding call to `self.next()`
    /// has returned `nil`.
    mutating func next() -> Element?
}

func ^(lhs: Int32, rhs: Int32) -> Int32

func ^(lhs: UInt8, rhs: UInt8) -> UInt8

func ^(lhs: Bool, rhs: Bool) -> Bool

func ^<T : _RawOptionSetType>(a: T, b: T) -> T

func ^(lhs: Int8, rhs: Int8) -> Int8

func ^(lhs: UInt16, rhs: UInt16) -> UInt16

func ^(lhs: Int, rhs: Int) -> Int

func ^(lhs: UInt, rhs: UInt) -> UInt

func ^(lhs: Int64, rhs: Int64) -> Int64

func ^(lhs: UInt64, rhs: UInt64) -> UInt64

func ^(lhs: Int16, rhs: Int16) -> Int16

func ^(lhs: UInt32, rhs: UInt32) -> UInt32

func ^=(inout lhs: Bool, rhs: Bool)

func ^=(inout lhs: UInt8, rhs: UInt8)

func ^=(inout lhs: Int8, rhs: Int8)

func ^=(inout lhs: UInt64, rhs: UInt64)

func ^=(inout lhs: UInt16, rhs: UInt16)

func ^=(inout lhs: Int16, rhs: Int16)

func ^=(inout lhs: Int64, rhs: Int64)

func ^=(inout lhs: UInt, rhs: UInt)

func ^=(inout lhs: Int, rhs: Int)

func ^=(inout lhs: UInt32, rhs: UInt32)

func ^=(inout lhs: Int32, rhs: Int32)

func ^=<T : BitwiseOperationsType>(inout lhs: T, rhs: T)


/// The underlying buffer for an ArrayType conforms to _ArrayBufferType
protocol _ArrayBufferType : MutableCollectionType {

    /// The type of elements stored in the buffer
    typealias Element

    /// create an empty buffer
    init()

    /// Adopt the storage of x
    init(_ buffer: _ContiguousArrayBuffer<Element>)

    /// Copy the given subRange of this buffer into uninitialized memory
    /// starting at target.  Return a pointer past-the-end of the
    /// just-initialized memory.
    func _uninitializedCopy(subRange: Range<Int>, target: UnsafeMutablePointer<Element>) -> UnsafeMutablePointer<Element>

    /// Convert to an NSArray.
    /// Precondition: _isBridgedToObjectiveC(Element.self)
    /// O(1) if the element type is bridged verbatim, O(N) otherwise
    func _asCocoaArray() -> _SwiftNSArrayRequiredOverridesType
    subscript (index: Int) -> Element { get nonmutating set }

    /// If this buffer is backed by a uniquely-referenced mutable
    /// _ContiguousArrayBuffer that can be grown in-place to allow the self
    /// buffer store minimumCapacity elements, returns that buffer.
    /// Otherwise, returns nil.  Note: the result's baseAddress may
    /// not match ours, if we are a _SliceBuffer.
    ///
    /// Note: this function must remain mutating; otherwise the buffer
    /// may acquire spurious extra references, which will cause
    /// unnecessary reallocation.
    mutating func requestUniqueMutableBackingBuffer(minimumCapacity: Int) -> _ContiguousArrayBuffer<Element>?

    /// Returns true iff this buffer is backed by a uniquely-referenced mutable
    /// _ContiguousArrayBuffer.
    ///
    /// Note: this function must remain mutating; otherwise the buffer
    /// may acquire spurious extra references, which will cause
    /// unnecessary reallocation.
    mutating func isMutableAndUniquelyReferenced() -> Bool

    /// If this buffer is backed by a _ContiguousArrayBuffer, return it.
    /// Otherwise, return nil.  Note: the result's baseAddress may
    /// not match ours, if we are a _SliceBuffer.
    func requestNativeBuffer() -> _ContiguousArrayBuffer<Element>?

    /// Replace the given subRange with the first newCount elements of
    /// the given collection.
    ///
    /// Requires: this buffer is backed by a uniquely-referenced
    /// _ContiguousArrayBuffer
    mutating func replace<C : CollectionType where `Self`.Element == Element>(#subRange: Range<Int>, with newCount: Int, elementsOf newValues: C)
    subscript (subRange: Range<Int>) -> _SliceBuffer<Element> { get }

    /// Call `body(p)`, where `p` is an `UnsafeBufferPointer` over the
    /// underlying contiguous storage.  If no such storage exists, it is
    /// created on-demand.
    func withUnsafeBufferPointer<R>(body: (UnsafeBufferPointer<Element>) -> R) -> R

    /// Call `body(p)`, where `p` is an `UnsafeMutableBufferPointer`
    /// over the underlying contiguous storage.  Requires: such
    /// contiguous storage exists or the buffer is empty
    mutating func withUnsafeMutableBufferPointer<R>(body: (UnsafeMutableBufferPointer<Element>) -> R) -> R

    /// How many elements the buffer stores
    var count: Int { get set }

    /// How many elements the buffer can store without reallocation
    var capacity: Int { get }

    /// An object that keeps the elements stored in this buffer alive
    var owner: AnyObject? { get }

    /// If the elements are stored contiguously, a pointer to the first
    /// element. Otherwise, nil.
    var baseAddress: UnsafeMutablePointer<Element> { get }

    /// A value that identifies first mutable element, if any.  Two
    /// arrays compare === iff they are both empty, or if their buffers
    /// have the same identity and count.
    var identity: Word { get }
}


/// This protocol is an implementation detail of `BidirectionalIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `BidirectionalIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _BidirectionalIndexType : _ForwardIndexType {

    /// Return the previous consecutive value in a discrete sequence.
    ///
    /// If `self` has a well-defined successor,
    /// `self.successor().predecessor() == self`.  If `self` has a
    /// well-defined predecessor, `self.predecessor().successor() ==
    /// self`.
    ///
    /// Requires: `self` has a well-defined predecessor.
    func predecessor() -> Self
}


/// Effectively a proxy for NSString that doesn't mention it by
/// name.  NSString's conformance to this protocol is declared in
/// Foundation.
@objc protocol _CocoaStringType {
}


/// This protocol is an implementation detail of `CollectionType`; do
/// not use it directly.
///
/// Its requirements are inherited by `CollectionType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _CollectionType : _SequenceType {

    /// A type that represents a valid position in the collection.
    ///
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index : ForwardIndexType

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Index { get }
    typealias _Element
    subscript (_i: Index) -> _Element { get }
}


/// This protocol is an implementation detail of `Comparable`; do
/// not use it directly.
///
/// Its requirements are inherited by `Comparable` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Comparable {

    /// A `strict total order
    /// <http://en.wikipedia.org/wiki/Total_order#Strict_total_order>`_
    /// over instances of `Self`
    func <(lhs: Self, rhs: Self) -> Bool
}


/// This protocol is an implementation detail of `ExtensibleCollectionType`; do
/// not use it directly.
///
/// Its requirements are inherited by `ExtensibleCollectionType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _ExtensibleCollectionType : CollectionType {

    /// Create an empty instance
    init()

    /// A non-binding request to ensure `n` elements of available storage.
    ///
    /// This works as an optimization to avoid multiple reallocations of
    /// linear data structures like `Array`.  Conforming types may
    /// reserve more than `n`, exactly `n`, less than `n` elements of
    /// storage, or even ignore the request completely.
    mutating func reserveCapacity(n: Self.Index.Distance)

    /// Append `x` to `self`.
    ///
    /// Applying `successor()` to the index of the new element yields
    /// `self.endIndex`.
    ///
    /// Complexity: amortized O(1).
    mutating func append(x: Self.Generator.Element)

    /// Append the elements of `newElements` to `self`.
    ///
    /// Complexity: O(*length of result*) 
    /// 
    /// A possible implementation::
    ///
    ///   reserveCapacity(countElements(self) + underestimateCount(newElements))
    ///   for x in newElements {
    ///     newElements.append(x)
    ///   }
    mutating func extend<S : SequenceType where `Self`.Generator.Element == Self.Generator.Element>(newElements: S)
}


/// This protocol is an implementation detail of `ForwardIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `ForwardIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _ForwardIndexType : _Incrementable {

    /// A type that can represent the number of steps between pairs of
    /// `Self` values where one value is reachable from the other.
    ///
    /// Reachability is defined by the ability to produce one value from
    /// the other via zero or more applications of `successor`.
    typealias Distance : _SignedIntegerType = Int
    typealias _DisabledRangeIndex = _DisabledRangeIndex_
}


/// This protocol is an implementation detail of `ForwardIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `ForwardIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Incrementable : Equatable {

    /// Return the next consecutive value in a discrete sequence of
    /// `Self` values
    ///
    /// Requires: `self` has a well-defined successor.
    func successor() -> Self
}


/// This protocol is an implementation detail of `IntegerArithmeticType`; do
/// not use it directly.
///
/// Its requirements are inherited by `IntegerArithmeticType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _IntegerArithmeticType {

    /// Add `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func addWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func subtractWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func multiplyWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func divideWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning the remainder and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func remainderWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)
}


/// This protocol is an implementation detail of `IntegerType`; do
/// not use it directly.
///
/// Its requirements are inherited by `IntegerType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _IntegerType : IntegerLiteralConvertible, Printable, Hashable, IntegerArithmeticType, BitwiseOperationsType, _Incrementable {
}


/// A Swift Array or Dictionary of types conforming to
/// `_ObjectiveCBridgeable` can be passed to ObjectiveC as an NSArray or
/// NSDictionary, respectively.  The elements of the resulting NSArray
/// or NSDictionary will be the result of calling `_bridgeToObjectiveC`
/// on each elmeent of the source container.
protocol _ObjectiveCBridgeable {
    typealias _ObjectiveCType

    /// Return true iff instances of `Self` can be converted to
    /// Objective-C.  Even if this method returns `true`, A given
    /// instance of `Self._ObjectiveCType` may, or may not, convert
    /// successfully to `Self`; for example, an `NSArray` will only
    /// convert successfully to `[String]` if it contains only
    /// `NSString`\ s.
    class func _isBridgedToObjectiveC() -> Bool

    /// Must return `_ObjectiveCType.self`.
    class func _getObjectiveCType() -> Any.Type

    /// Convert `self` to Objective-C
    func _bridgeToObjectiveC() -> _ObjectiveCType

    /// Bridge from an Objective-C object of the bridged class type to a
    /// value of the Self type.
    ///
    /// This bridging operation is used for forced downcasting (e.g.,
    /// via as), and may defer complete checking until later. For
    /// example, when bridging from NSArray to Array<T>, we can defer
    /// the checking for the individual elements of the array.
    ///
    /// :param: result The location where the result is written. The optional
    /// will always contain a value.
    class func _forceBridgeFromObjectiveC(source: _ObjectiveCType, inout result: Self?)

    /// Try to bridge from an Objective-C object of the bridged class
    /// type to a value of the Self type.
    ///
    /// This conditional bridging operation is used for conditional
    /// downcasting (e.g., via as?) and therefore must perform a
    /// complete conversion to the value type; it cannot defer checking
    /// to a later time.
    ///
    /// :param: result The location where the result is written.
    ///
    /// :returns: true if bridging succeeded, false otherwise. This redundant
    /// information is provided for the convenience of the runtime's dynamic_cast
    /// implementation, so that it need not look into the optional representation
    /// to determine success.
    class func _conditionallyBridgeFromObjectiveC(source: _ObjectiveCType, inout result: Self?) -> Bool
}


/// This protocol is an implementation detail of `RandomAccessIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `RandomAccessIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _RandomAccessIndexType : _BidirectionalIndexType, Strideable {

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    ///
    /// Axioms::
    /// 
    ///   x.distanceTo(x.successor())) == 1
    ///   x.distanceTo(x.predecessor())) == -1
    ///   x.advancedBy(x.distanceTo(y)) == y
    func distanceTo(other: Self) -> Self.Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    ///
    /// Axioms::
    ///
    ///   x.advancedBy(0) == x
    ///   x.advancedBy(1) == x.successor()
    ///   x.advancedBy(-1) == x.predecessor()
    ///   x.distanceTo(x.advancedBy(m)) == m
    func advancedBy(n: Self.Distance) -> Self
}


/// This protocol is an implementation detail of `RawOptionSetType`; do
/// not use it directly.
///
/// Its requirements are inherited by `RawOptionSetType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _RawOptionSetType : RawRepresentable, Equatable {
    typealias RawValue : BitwiseOperationsType, Equatable
    init(rawValue: RawValue)
}


/// This protocol is an implementation detail of `SequenceType`; do
/// not use it directly.
///
/// Its requirements are inherited by `SequenceType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _SequenceType {
}


/// This protocol is an implementation detail of `SequenceType`; do
/// not use it directly.
///
/// Its requirements are inherited by `SequenceType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Sequence_Type : _SequenceType {

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator : GeneratorType

    /// Return a *generator* over the elements of this *sequence*.  The
    /// *generator*\ 's next element is the first element of the
    /// sequence.
    ///
    /// Complexity: O(1)
    func generate() -> Generator
}


/// This protocol is an implementation detail of `SignedIntegerType`;
/// do not use it directly.
///
/// Its requirements are inherited by `SignedIntegerType` and thus
/// must be satisfied by types conforming to that protocol.
protocol _SignedIntegerType : _IntegerType, SignedNumberType {

    /// Represent this number using Swift's widest native signed integer
    /// type.
    func toIntMax() -> IntMax

    /// Convert from Swift's widest signed integer type, trapping on
    /// overflow.
    init(_: IntMax)
}


/// This protocol is an implementation detail of `SignedNumberType`; do
/// not use it directly.
///
/// Its requirements are inherited by `SignedNumberType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _SignedNumberType : Comparable, IntegerLiteralConvertible {

    /// Return the difference between `lhs` and `rhs`.
    func -(lhs: Self, rhs: Self) -> Self
}


/// This protocol is an implementation detail of `Sliceable`; do
/// not use it directly.
///
/// Its requirements are inherited by `Sliceable` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Sliceable : CollectionType {
}


/// This protocol is an implementation detail of `Strideable`; do
/// not use it directly.
///
/// Its requirements are inherited by `Strideable` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Strideable {

    /// A type that can represent the distance between two values of `Self`
    typealias Stride : SignedNumberType

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    ///
    /// See also: `RandomAccessIndexType`\ 's `distanceTo`, which provides a
    /// stronger semantic guarantee.
    func distanceTo(other: Self) -> Stride

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    ///
    /// See also: `RandomAccessIndexType`\ 's `advancedBy`, which
    /// provides a stronger semantic guarantee.
    func advancedBy(n: Stride) -> Self
}

@unsafe_no_objc_tagged_pointer @objc protocol _SwiftNSArrayRequiredOverridesType : _SwiftNSCopyingType, _SwiftNSFastEnumerationType {
    func objectAtIndex(index: Int) -> AnyObject
    func getObjects(_: UnsafeMutablePointer<AnyObject>, range: _SwiftNSRange)
    func countByEnumeratingWithState(state: UnsafeMutablePointer<_SwiftNSFastEnumerationState>, objects: UnsafeMutablePointer<AnyObject>, count: Int) -> Int
    func copyWithZone(zone: _SwiftNSZone) -> AnyObject
    var count: Int { get }
}

@unsafe_no_objc_tagged_pointer @objc protocol _SwiftNSArrayType : _SwiftNSArrayRequiredOverridesType {
    func indexOfObject(anObject: AnyObject) -> Int
}

@objc protocol _SwiftNSCopyingType {
    func copyWithZone(zone: _SwiftNSZone) -> AnyObject
}

@objc protocol _SwiftNSDictionaryRequiredOverridesType : _SwiftNSCopyingType, _SwiftNSFastEnumerationType {
    init(objects: UnsafePointer<AnyObject?>, forKeys: UnsafePointer<Void>, count: Int)
    var count: Int { get }
    func objectForKey(aKey: AnyObject?) -> AnyObject?
    func keyEnumerator() -> _SwiftNSEnumeratorType?
    func copyWithZone(zone: _SwiftNSZone) -> AnyObject
    func countByEnumeratingWithState(state: UnsafeMutablePointer<_SwiftNSFastEnumerationState>, objects: UnsafeMutablePointer<AnyObject>, count: Int) -> Int
}

@unsafe_no_objc_tagged_pointer @objc protocol _SwiftNSDictionaryType : _SwiftNSDictionaryRequiredOverridesType {
    func getObjects(objects: UnsafeMutablePointer<AnyObject>, andKeys keys: UnsafeMutablePointer<AnyObject>)
}

@objc protocol _SwiftNSEnumeratorType {
    init()
    func nextObject() -> AnyObject?
}

@objc protocol _SwiftNSFastEnumerationType {
    func countByEnumeratingWithState(state: UnsafeMutablePointer<_SwiftNSFastEnumerationState>, objects: UnsafeMutablePointer<AnyObject>, count: Int) -> Int
}

@objc protocol _SwiftNSStringRequiredOverridesType : _SwiftNSCopyingType, _SwiftNSFastEnumerationType {
    func length() -> Int
    func characterAtIndex(index: Int) -> UInt16
}

@objc protocol _SwiftNSStringType : _SwiftNSStringRequiredOverridesType {
}


/// This protocol is an implementation detail of `SignedIntegerType`;
/// do not use it directly.
///
/// Its requirements are inherited by `SignedIntegerType` and thus
/// must be satisfied by types conforming to that protocol.
protocol _UnsignedIntegerType : _IntegerType {

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Convert from Swift's widest unsigned integer type, trapping on
    /// overflow.
    init(_: UIntMax)
}


/// Return the absolute value of `x`.
///
/// Concrete instances of `SignedNumberType` can specialize this
/// function by conforming to `AbsoluteValuable`.
func abs<T : SignedNumberType>(x: T) -> T


/// Return the result of advancing `start` by `n` positions.  If `T`
/// models `RandomAccessIndexType`, executes in O(1).  Otherwise,
/// executes in O(`abs(n)`).  If `T` does not model
/// `BidirectionalIndexType`, requires that `n` is non-negative.
///
/// `advance(i, n)` is a synonym for `i++n'
func advance<T : ForwardIndexType>(start: T, n: T.Distance) -> T


/// Return the result of advancing start by `n` positions, or until it
/// equals `end`.  If `T` models `RandomAccessIndexType`, executes in
/// O(1).  Otherwise, executes in O(`abs(n)`).  If `T` does not model
/// `BidirectionalIndexType`, requires that `n` is non-negative.
func advance<T : ForwardIndexType>(start: T, n: T.Distance, end: T) -> T


/// Returns the minimum memory alignment of `T`.
func alignof<T>(_: T.Type) -> Int


/// Returns the minimum memory alignment of `T`.
func alignofValue<T>(_: T) -> Int


/// User code assertions.
///
/// User code assertions and fatal errors are only enabled in debug mode. In
/// release or fast mode these checks are disabled. This means they may have no
/// effect on program semantics, depending on the assert configuration.
/// Traditional C-style assert with an optional message.
///
/// When assertions are enabled and `condition` is false, stop program
/// execution in a debuggable state after printing a message.  When
/// assertions are disabled in release and fast builds, `condition` is not even
/// evaluated.
///
/// When assertions are turned off, the optimizer can assume that the
/// `condition` is true.
func assert(condition: @autoclosure () -> Bool, _ message: @autoclosure () -> String = default, file: StaticString = default, line: UWord = default)


/// A fatal error occurred and program execution should stop in debug mode.  In
/// optimized builds this is a noop.
@noreturn func assertionFailure(_ message: @autoclosure () -> String = default, file: StaticString = default, line: UWord = default)


/// Return `true` iff `x` is in `seq`.
func contains<S : SequenceType where S.Generator.Element : Equatable>(seq: S, x: S.Generator.Element) -> Bool


/// Return `true` iff an element in `seq` satisfies `predicate`.
func contains<S : SequenceType, L : BooleanType>(seq: S, predicate: (S.Generator.Element) -> L) -> Bool


/// Equivalent to countElements(r)
func count<I : RandomAccessIndexType>(r: Range<I>) -> I.Distance


/// Return the number of elements in x.
///
/// O(1) if T.Index is RandomAccessIndexType; O(N) otherwise.
func countElements<T : _CollectionType>(x: T) -> T.Index.Distance


/// Write to `target` the textual representation of `x` most suitable
/// for debugging.
///
/// * If `T` conforms to `DebugPrintable`, write `x.debugDescription`
/// * Otherwise, if `T` conforms to `Printable`, write `x.description`
/// * Otherwise, if `T` conforms to `Stramable`, write `x`
/// * Otherwise, fall back to a default textual representation.
///
/// See also: `debugPrintln(x, &target)`
func debugPrint<T, TargetStream : OutputStreamType>(x: T, inout target: TargetStream)


/// Write to the console the textual representation of `x` most suitable
/// for debugging.
///
/// * If `T` conforms to `DebugPrintable`, write `x.debugDescription`
/// * Otherwise, if `T` conforms to `Printable`, write `x.description`
/// * Otherwise, if `T` conforms to `Stramable`, write `x`
/// * Otherwise, fall back to a default textual representation.
///
/// See also: `debugPrintln(x)`
func debugPrint<T>(x: T)


/// Write to `target` the textual representation of `x` most suitable
/// for debugging, followed by a newline.
///
/// * If `T` conforms to `DebugPrintable`, write `x.debugDescription`
/// * Otherwise, if `T` conforms to `Printable`, write `x.description`
/// * Otherwise, if `T` conforms to `Stramable`, write `x`
/// * Otherwise, fall back to a default textual representation.
///
/// See also: `debugPrint(x, &target)`
func debugPrintln<T, TargetStream : OutputStreamType>(x: T, inout target: TargetStream)


/// Write to the console the textual representation of `x` most suitable
/// for debugging, followed by a newline.
///
/// * If `T` conforms to `DebugPrintable`, write `x.debugDescription`
/// * Otherwise, if `T` conforms to `Printable`, write `x.description`
/// * Otherwise, if `T` conforms to `Stramable`, write `x`
/// * Otherwise, fall back to a default textual representation.
///
/// See also: `debugPrint(x)`
func debugPrintln<T>(x: T)


/// Measure the distance between `start` and `end`.
///
/// If `T` models `RandomAccessIndexType`, requires that `start` and `end` are
/// part of the same sequence, and executes in O(1).
///
/// Otherwise, requires that `end` is reachable from `start` by
/// incrementation, and executes in O(N), where N is the function's
/// result.
func distance<T : ForwardIndexType>(start: T, end: T) -> T.Distance


/// Return a slice containing all but the first element of `s`.
///
/// Requires: `s` is non-empty.
func dropFirst<Seq : Sliceable>(s: Seq) -> Seq.SubSlice


/// Return a slice containing all but the last element of `s`.
///
/// Requires: `s` is non-empty.
func dropLast<S : Sliceable where S.Index : BidirectionalIndexType>(s: S) -> S.SubSlice


/// Dump an object's contents using its mirror to standard output.
func dump<T>(x: T, name: String? = default, indent: Int = default, maxDepth: Int = default, maxItems: Int = default) -> T


/// Dump an object's contents using its mirror to the specified output stream.
func dump<T, TargetStream : OutputStreamType>(x: T, inout targetStream: TargetStream, name: String? = default, indent: Int = default, maxDepth: Int = default, maxItems: Int = default) -> T


/// Return a lazy `SequenceType` containing pairs (*n*, *x*), where
/// *n*\ s are consecutive `Int`\ s starting at zero, and *x*\ s are
/// the elements of `base`::
///
///   > for (n, c) in enumerate("Swift") { println("\(n): '\(c)'" )}
///   0: 'S'
///   1: 'w'
///   2: 'i'
///   3: 'f'
///   4: 't'
func enumerate<Seq : SequenceType>(base: Seq) -> EnumerateSequence<Seq>


/// Return `true` iff `a1` and `a2` contain the same elements in the
/// same order.
func equal<S1 : SequenceType, S2 : SequenceType where S1.Generator.Element == S1.Generator.Element, S1.Generator.Element : Equatable>(a1: S1, a2: S2) -> Bool


/// Return true iff `a1` and `a2` contain equivalent elements, using
/// `isEquivalent` as the equivalence test.  Requires: `isEquivalent`
/// is an `equivalence relation
/// <http://en.wikipedia.org/wiki/Equivalence_relation>`_
func equal<S1 : SequenceType, S2 : SequenceType where S1.Generator.Element == S1.Generator.Element>(a1: S1, a2: S2, isEquivalent: (S1.Generator.Element, S1.Generator.Element) -> Bool) -> Bool


/// Append elements from `newElements` to `x`.  Complexity:
/// O(N)
func extend<C : RangeReplaceableCollectionType, S : CollectionType where S.Generator.Element == S.Generator.Element>(inout x: C, newElements: S)


/// A fatal error occurred and program execution should stop in debug,
/// optimized and unchecked modes.
@noreturn func fatalError(_ message: @autoclosure () -> String = default, file: StaticString = default, line: UWord = default)


/// Return an `Array` containing the elements of `source`,
/// in order, that satisfy the predicate `includeElement`.
func filter<S : SequenceType>(source: S, includeElement: (S.Generator.Element) -> Bool) -> [S.Generator.Element]


/// Returns the first index where `value` appears in `domain` or `nil` if
/// `value` is not found.
///
/// Complexity: O(\ `countElements(domain)`\ )
func find<C : CollectionType where C.Generator.Element : Equatable>(domain: C, value: C.Generator.Element) -> C.Index?


/// Returns the first element of `x`, or `nil` if `x` is empty.
func first<C : CollectionType>(x: C) -> C.Generator.Element?


/// Returns a `CVaListPointer` built from `args` that's backed by
/// autoreleased storage.
///
/// .. Warning:: This function is best avoided in favor of
///    `withVaList`, but occasionally (i.e. in a `class` initializer) you
///    may find that the language rules don't allow you to use
///    `withVaList` as intended.
func getVaList(args: [CVarArgType]) -> CVaListPointer


/// Return the range of `x` 's valid index values.
///
/// The result's `endIndex` is the same as that of `x`.  Because
/// `Range` is half-open, iterating the values of the result produces
/// all valid subscript arguments for `x`, omitting its `endIndex`.
func indices<C : CollectionType>(x: C) -> Range<C.Index>


/// Insert `newElement` into `x` at index `i`.
///
/// Invalidates all indices with respect to `x`.
///
/// Complexity: O(\ `countElements(x)`\ ).
func insert<C : RangeReplaceableCollectionType>(inout x: C, newElement: C.Generator.Element, atIndex i: C.Index)


/// Returns `true` iff `x` is empty.
func isEmpty<C : CollectionType>(x: C) -> Bool


/// Creates and returns a collection of type `C` that is the result of
/// interposing a given separator between the elements of the sequence
/// `elements`.
///
/// For example, this code excerpt writes "``here be dragons``" to the standard
/// output::
///
///   println(join(" ", [ "here", "be", "dragons" ]))
func join<C : ExtensibleCollectionType, S : SequenceType where C == C>(separator: C, elements: S) -> C


/// Returns the last element of `x`, or `nil` if `x` is empty.
func last<C : CollectionType where C.Index : BidirectionalIndexType>(x: C) -> C.Generator.Element?


/// Augment `s` with lazy methods such as `map`, `filter`, etc.
func lazy<S : CollectionType where S.Index : ForwardIndexType>(s: S) -> LazyForwardCollection<S>


/// Augment `s` with lazy methods such as `map`, `filter`, etc.
func lazy<S : CollectionType where S.Index : BidirectionalIndexType>(s: S) -> LazyBidirectionalCollection<S>


/// Augment `s` with lazy methods such as `map`, `filter`, etc.
func lazy<S : CollectionType where S.Index : RandomAccessIndexType>(s: S) -> LazyRandomAccessCollection<S>


/// Augment `s` with lazy methods such as `map`, `filter`, etc.
func lazy<S : SequenceType>(s: S) -> LazySequence<S>


/// Return true iff `a1` precedes `a2` in a lexicographical ("dictionary")
/// ordering, using `less` as the comparison between elements.
func lexicographicalCompare<S1 : SequenceType, S2 : SequenceType where S1.Generator.Element == S1.Generator.Element>(a1: S1, a2: S2, less: (S1.Generator.Element, S1.Generator.Element) -> Bool) -> Bool


/// Return true iff a1 precedes a2 in a lexicographical ("dictionary")
/// ordering, using "<" as the comparison between elements.
func lexicographicalCompare<S1 : SequenceType, S2 : SequenceType where S1.Generator.Element == S1.Generator.Element, S1.Generator.Element : Comparable>(a1: S1, a2: S2) -> Bool


/// Haskell's fmap for Optionals.
func map<T, U>(x: T?, f: (T) -> U) -> U?


/// Return an `Array` containing the results of mapping `transform`
/// over `source`.
func map<C : CollectionType, T>(source: C, transform: (C.Generator.Element) -> T) -> [T]


/// Return an `Array` containing the results of mapping `transform`
/// over `source`.
func map<S : SequenceType, T>(source: S, transform: (S.Generator.Element) -> T) -> [T]


/// Return the greatest argument passed
func max<T : Comparable>(x: T, y: T, z: T, rest: T...) -> T


/// Return the greater of `x` and `y`
func max<T : Comparable>(x: T, y: T) -> T


/// Returns the maximum element in `elements`.  Requires:
/// `elements` is non-empty. O(countElements(elements))
func maxElement<R : SequenceType where R.Generator.Element : Comparable>(elements: R) -> R.Generator.Element


/// Return the lesser of `x` and `y`
func min<T : Comparable>(x: T, y: T) -> T


/// Return the least argument passed
func min<T : Comparable>(x: T, y: T, z: T, rest: T...) -> T


/// Returns the minimum element in `elements`.  Requires:
/// `elements` is non-empty. O(countElements(elements))
func minElement<R : SequenceType where R.Generator.Element : Comparable>(elements: R) -> R.Generator.Element


/// Convert `x` to type `U`, trapping on overflow in -Onone and -O
/// builds.
///
/// Typically used to do conversion to any contextually-deduced
/// integer type::
///
///   func f(x: Int32) {}
///   func g(x: UInt64) { f(numericCast(x)) }
func numericCast<T : _UnsignedIntegerType, U : _SignedIntegerType>(x: T) -> U


/// Convert `x` to type `U`, trapping on overflow in -Onone and -O
/// builds.
///
/// Typically used to do conversion to any contextually-deduced
/// integer type::
///
///   func f(x: UInt32) {}
///   func g(x: Int64) { f(numericCast(x)) }
func numericCast<T : _SignedIntegerType, U : _UnsignedIntegerType>(x: T) -> U


/// Convert `x` to type `U`, trapping on overflow in -Onone and -O
/// builds.
///
/// Typically used to do conversion to any contextually-deduced
/// integer type::
///
///   func f(x: UInt32) {}
///   func g(x: UInt64) { f(numericCast(x)) }
func numericCast<T : _UnsignedIntegerType, U : _UnsignedIntegerType>(x: T) -> U


/// Convert `x` to type `U`, trapping on overflow in -Onone and -O
/// builds.
///
/// Typically used to do conversion to any contextually-deduced
/// integer type::
///
///   func f(x: Int32) {}
///   func g(x: Int64) { f(numericCast(x)) }
func numericCast<T : _SignedIntegerType, U : _SignedIntegerType>(x: T) -> U


/// Returns `true` if `lhs` and `rhs` have a non-empty intersection
func overlaps<I0 : IntervalType, I1 : IntervalType where I0.Bound == I0.Bound>(lhs: I0, rhs: I1) -> Bool


/// Re-order the given `range` of `elements` and return a pivot index
/// *p*.  Postcondition: for all *i* in `range.startIndex..<`\ *p*,
/// and *j* in *p*\ `..<range.endIndex`, `less(elements[`\ *i*\ `],
/// elements[`\ *j*\ `]) && !less(elements[`\ *j*\ `], elements[`\
/// *p*\ `])`.  Only returns `range.endIndex` when `elements` is
/// empty.
/// Requires: `isOrderedBefore` is a `strict weak ordering
/// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
/// over `elements`.
func partition<C : MutableCollectionType where C.Index : RandomAccessIndexType>(inout elements: C, range: Range<C.Index>, isOrderedBefore: (C.Generator.Element, C.Generator.Element) -> Bool) -> C.Index


/// Re-order the given `range` of `elements` and return a pivot index
/// *p*.  Postcondition: for all *i* in `range.startIndex..<`\ *p*,
/// and *j* in *p*\ `..<range.endIndex`, `less(elements[`\ *i*\ `],
/// elements[`\ *j*\ `]) && !less(elements[`\ *j*\ `], elements[`\
/// *p*\ `])`.  Only returns `range.endIndex` when `elements` is
/// empty.
/// Requires: The less-than operator (`func <`) defined in the `Comparable`
/// conformance is a `strict weak ordering
/// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
/// over `elements`.
func partition<C : MutableCollectionType where C.Index : RandomAccessIndexType, C.Generator.Element : Comparable>(inout elements: C, range: Range<C.Index>) -> C.Index


/// Ensure that the `condition` is true.
///
/// If the `condition` is false, in debug and release modes the program stops.
///
/// In unchecked mode the optimizer can assume that the `condition` is true.
func precondition(condition: @autoclosure () -> Bool, _ message: @autoclosure () -> String = default, file: StaticString = default, line: UWord = default)


/// A fatal error occurred and program execution should stop in debug mode and
/// in optimized mode.  In unchecked builds this is a noop, but the
/// optimizer can still assume that the call is unreachable.
@noreturn func preconditionFailure(_ message: @autoclosure () -> String = default, file: StaticString = default, line: UWord = default)


/// Return a slice, up to `maxLength` in length, containing the
/// initial elements of `s`.
///
/// If `maxLength` exceeds `countElements(s)`, the result contains all
/// the elements of `s`.
/// 
/// Complexity: O(1)+K when `S.Index` conforms to
/// `RandomAccessIndexType` and O(N)+K otherwise, where K is the cost
/// of slicing `s`.
func prefix<S : Sliceable>(s: S, maxLength: Int) -> S.SubSlice


/// Writes the textual representation of `object` into the standard output.
///
/// The textual representation is obtained from the `object` using its protocol
/// conformances, in the following order of preference: `Streamable`,
/// `Printable`, `DebugPrintable`.
///
/// Do not overload this function for your type.  Instead, adopt one of the
/// protocols mentioned above.
func print<T>(object: T)


/// Writes the textual representation of `object` into the stream `target`.
///
/// The textual representation is obtained from the `object` using its protocol
/// conformances, in the following order of preference: `Streamable`,
/// `Printable`, `DebugPrintable`.
///
/// Do not overload this function for your type.  Instead, adopt one of the
/// protocols mentioned above.
func print<T, TargetStream : OutputStreamType>(object: T, inout target: TargetStream)


/// Writes a single newline character into the standard output.
func println()


/// Writes the textual representation of `object` and a newline character into
/// the standard output.
///
/// The textual representation is obtained from the `object` using its protocol
/// conformances, in the following order of preference: `Streamable`,
/// `Printable`, `DebugPrintable`.
///
/// Do not overload this function for your type.  Instead, adopt one of the
/// protocols mentioned above.
func println<T>(object: T)


/// Writes the textual representation of `object` and a newline character into
/// the stream `target`.
///
/// The textual representation is obtained from the `object` using its protocol
/// conformances, in the following order of preference: `Streamable`,
/// `Printable`, `DebugPrintable`.
///
/// Do not overload this function for your type.  Instead, adopt one of the
/// protocols mentioned above.
func println<T, TargetStream : OutputStreamType>(object: T, inout target: TargetStream)


/// Return the result of repeatedly calling `combine` with an
/// accumulated value initialized to `initial` and each element of
/// `sequence`, in turn.
func reduce<S : SequenceType, U>(sequence: S, initial: U, combine: (U, S.Generator.Element) -> U) -> U


/// Produce a mirror for any value. If the value's type conforms to Reflectable,
/// invoke its getMirror() method; otherwise, fall back to an implementation
/// in the runtime that structurally reflects values of any type.
func reflect<T>(x: T) -> MirrorType


/// Remove all elements from `x`
///
/// Invalidates all indices with respect to `x`.
///
/// :param: `keepCapacity`, if `true`, is a non-binding request to
///    avoid releasing storage, which can be a useful optimization
///    when `x` is going to be grown again.
///
/// Complexity: O(\ `countElements(x)`\ ).
func removeAll<C : RangeReplaceableCollectionType>(inout x: C, keepCapacity: Bool = default)


/// Remove from `x` and return the element at index `i` 
///
/// Invalidates all indices with respect to `x`.
///
/// Complexity: O(\ `countElements(x)`\ ).
func removeAtIndex<C : RangeReplaceableCollectionType>(inout x: C, index: C.Index) -> C.Generator.Element


/// Remove an element from the end of `x`  in O(1).
/// Requires: `x` is nonempty
func removeLast<C : RangeReplaceableCollectionType where C.Index : BidirectionalIndexType>(inout x: C) -> C.Generator.Element


/// Remove from `x` the indicated `subRange` of elements
///
/// Invalidates all indices with respect to `x`.
///
/// Complexity: O(\ `countElements(x)`\ ).
func removeRange<C : RangeReplaceableCollectionType>(inout x: C, subRange: Range<C.Index>)


/// Return an `Array` containing the elements of `source` in reverse
/// order.
func reverse<C : CollectionType where C.Index : BidirectionalIndexType>(source: C) -> [C.Generator.Element]


/// Returns the contiguous memory footprint of `T`.
///
/// Does not include any dynamically-allocated or "remote" storage.
/// In particular, `sizeof(X.self)`, when `X` is a class type, is the
/// same regardless of how many stored properties `X` has.
func sizeof<T>(_: T.Type) -> Int


/// Returns the contiguous memory footprint of  `T`.
///
/// Does not include any dynamically-allocated or "remote" storage.
/// In particular, `sizeof(a)`, when `a` is a class instance, is the
/// same regardless of how many stored properties `a` has.
func sizeofValue<T>(_: T) -> Int

func sort<T : Comparable>(inout array: ContiguousArray<T>)

func sort<T : Comparable>(inout array: [T])


/// Sort `collection` in-place.
///
/// The sorting algorithm is not stable (can change the relative order of
/// elements that compare equal).
///
/// Requires: The less-than operator (`func <`) defined in the `Comparable`
/// conformance is a `strict weak ordering
/// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
/// over `elements`.
func sort<C : MutableCollectionType where C.Index : RandomAccessIndexType, C.Generator.Element : Comparable>(inout collection: C)

func sort<T>(inout array: ContiguousArray<T>, isOrderedBefore: (T, T) -> Bool)

func sort<T>(inout array: [T], isOrderedBefore: (T, T) -> Bool)


/// Sort `collection` in-place according to `isOrderedBefore`.
///
/// The sorting algorithm is not stable (can change the relative order of
/// elements for which `isOrderedBefore` does not establish an order).
///
/// Requires: `isOrderedBefore` is a `strict weak ordering
/// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
/// over `elements`.
func sort<C : MutableCollectionType where C.Index : RandomAccessIndexType>(inout collection: C, isOrderedBefore: (C.Generator.Element, C.Generator.Element) -> Bool)


/// Return an `Array` containing the sorted elements of `source`{according}.
///
/// The sorting algorithm is not stable (can change the relative order of
/// elements that compare equal).
///
/// Requires: The less-than operator (`func <`) defined in the `Comparable`
/// conformance is a `strict weak ordering
/// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
/// over `elements`.
func sorted<C : SequenceType where C.Generator.Element : Comparable>(source: C) -> [C.Generator.Element]


/// Return an `Array` containing the sorted elements of `source`{according}.
///
/// The sorting algorithm is not stable (can change the relative order of
/// elements for which `isOrderedBefore` does not establish an order).
///
/// Requires: `isOrderedBefore` is a `strict weak ordering
/// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
/// over `elements`.
func sorted<C : SequenceType>(source: C, isOrderedBefore: (C.Generator.Element, C.Generator.Element) -> Bool) -> [C.Generator.Element]


/// Insert `newElements` into `x` at index `i`
///
/// Invalidates all indices with respect to `x`.
///
/// Complexity: O(\ `countElements(x) + countElements(newElements)`\ ).
func splice<C : RangeReplaceableCollectionType, S : CollectionType where S.Generator.Element == S.Generator.Element>(inout x: C, newElements: S, atIndex i: C.Index)


/// Return the result of slicing `elements` into sub-sequences that
/// don't contain elements satisfying the predicate `isSeparator`.
///
/// :param: maxSplit the maximum number of slices to return, minus 1.
/// If `maxSplit + 1` slices would otherwise be returned, the
/// algorithm stops splitting and returns a suffix of `elements`
///
/// :param: allowEmptySlices if true, an empty slice is produced in
/// the result for each pair of consecutive 
func split<S : Sliceable, R : BooleanType>(elements: S, isSeparator: (S.Generator.Element) -> R, maxSplit: Int = default, allowEmptySlices: Bool = default) -> [S.SubSlice]


/// Return true iff `s` begins with elements equivalent to those of
/// `prefix`, using `isEquivalent` as the equivalence test.  Requires:
/// `isEquivalent` is an `equivalence relation
/// <http://en.wikipedia.org/wiki/Equivalence_relation>`_
func startsWith<S0 : SequenceType, S1 : SequenceType where S0.Generator.Element == S0.Generator.Element>(s: S0, prefix: S1, isEquivalent: (S0.Generator.Element, S0.Generator.Element) -> Bool) -> Bool


/// Return true iff the the initial elements of `s` are equal to `prefix`.
func startsWith<S0 : SequenceType, S1 : SequenceType where S0.Generator.Element == S0.Generator.Element, S0.Generator.Element : Equatable>(s: S0, prefix: S1) -> Bool


/// Return the sequence of values (`start`, `start + stride`, `start +
/// stride + stride`, ... *last*) where *last* is the last value in
/// the progression less than or equal to `end`.
///
/// .. Note:: There is no guarantee that `end` is an element of the
///      sequence.
func stride<T : Strideable>(from start: T, through end: T, by stride: T.Stride) -> StrideThrough<T>


/// Return the sequence of values (`start`, `start + stride`, `start +
/// stride + stride`, ... *last*) where *last* is the last value in
/// the progression that is less than `end`.
func stride<T : Strideable>(from start: T, to end: T, by stride: T.Stride) -> StrideTo<T>


/// Returns the least possible interval between distinct instances of
/// `T` in memory.  The result is always positive.
func strideof<T>(_: T.Type) -> Int


/// Returns the least possible interval between distinct instances of
/// `T` in memory.  The result is always positive.
func strideofValue<T>(_: T) -> Int


/// Return a slice, up to `maxLength` in length, containing the
/// final elements of `s`.
///
/// If `maxLength` exceeds `countElements(s)`, the result contains all
/// the elements of `s`.
/// 
/// Complexity: O(1)+K when `S.Index` conforms to
/// `RandomAccessIndexType` and O(N)+K otherwise, where K is the cost
/// of slicing `s`.
func suffix<S : Sliceable where S.Index : BidirectionalIndexType>(s: S, maxLength: Int) -> S.SubSlice


/// Exchange the values of `a` and `b`
func swap<T>(inout a: T, inout b: T)


/// Returns the result of `debugPrint`\ 'ing `x` into a `String`
func toDebugString<T>(x: T) -> String


/// Returns the result of `print`\ 'ing `x` into a `String`
func toString<T>(x: T) -> String


/// Translate `input`, in the given `InputEncoding`, into `output`, in
/// the given `OutputEncoding`.
///
/// :param: `stopOnError` causes encoding to stop when an encoding
///   error is detected in `input`, if `true`.  Otherwise, U+FFFD
///   replacement characters are inserted for each detected error.
func transcode<Input : GeneratorType, Output : SinkType, InputEncoding : UnicodeCodecType, OutputEncoding : UnicodeCodecType where InputEncoding.CodeUnit == InputEncoding.CodeUnit, OutputEncoding.CodeUnit == OutputEncoding.CodeUnit>(inputEncoding: InputEncoding.Type, outputEncoding: OutputEncoding.Type, input: Input, output: Output, #stopOnError: Bool) -> (Bool)


/// Return an underestimate of the number of elements in the given
/// sequence, without consuming the sequence.  For Sequences that are
/// actually Collections, this will return countElements(x)
func underestimateCount<T : SequenceType>(x: T) -> Int


/// Return an UnsafePointer to the storage used for `object`.  There's
/// not much you can do with this other than use it to identify the
/// object
func unsafeAddressOf(object: AnyObject) -> UnsafePointer<Void>


/// Returns the the bits of `x`, interpreted as having type `U`.
///
/// .. Caution:: Breaks the guarantees of Swift's type system; use
///    with extreme care.  There's almost always a better way to do
///    anything.
///
func unsafeBitCast<T, U>(x: T, _: U.Type) -> U


/// Returns: `x as T`
///
/// Requires: `x is T`.  In particular, in -O builds, no test is
/// performed to ensure that `x` actually has dynamic type `T`.
///
/// .. Danger:: trades safety for performance.  Use `unsafeDowncast`
/// only when `x as T` has proven to be a performance problem and you
/// are confident that, always, `x is T`.  It is better than an
/// `unsafeBitCast` because it's more restrictive, and because
/// checking is still performed in debug builds.
func unsafeDowncast<T>(x: AnyObject) -> T


/// Evaluate `f()` and return its result, ensuring that `x` is not
/// destroyed before f returns.
func withExtendedLifetime<T, Result>(x: T, f: () -> Result) -> Result


/// Evaluate `f(x)` and return its result, ensuring that `x` is not
/// destroyed before f returns.
func withExtendedLifetime<T, Result>(x: T, f: (T) -> Result) -> Result


/// Invokes `body` with an `UnsafeMutablePointer` to `arg` and returns the
/// result. Useful for calling Objective-C APIs that take "in/out"
/// parameters (and default-constructible "out" parameters) by pointer
func withUnsafeMutablePointer<T, Result>(inout arg: T, body: (UnsafeMutablePointer<T>) -> Result) -> Result


/// Like `withUnsafeMutablePointer`, but passes pointers to `arg0`, `arg1`,
/// and `arg2`.
func withUnsafeMutablePointers<A0, A1, A2, Result>(inout arg0: A0, inout arg1: A1, inout arg2: A2, body: (UnsafeMutablePointer<A0>, UnsafeMutablePointer<A1>, UnsafeMutablePointer<A2>) -> Result) -> Result


/// Like `withUnsafeMutablePointer`, but passes pointers to `arg0` and `arg1`.
func withUnsafeMutablePointers<A0, A1, Result>(inout arg0: A0, inout arg1: A1, body: (UnsafeMutablePointer<A0>, UnsafeMutablePointer<A1>) -> Result) -> Result


/// Invokes `body` with an `UnsafePointer` to `arg` and returns the
/// result. Useful for calling Objective-C APIs that take "in/out"
/// parameters (and default-constructible "out" parameters) by pointer
func withUnsafePointer<T, Result>(inout arg: T, body: (UnsafePointer<T>) -> Result) -> Result


/// Like `withUnsafePointer`, but passes pointers to `arg0` and `arg1`.
func withUnsafePointers<A0, A1, Result>(inout arg0: A0, inout arg1: A1, body: (UnsafePointer<A0>, UnsafePointer<A1>) -> Result) -> Result


/// Like `withUnsafePointer`, but passes pointers to `arg0`, `arg1`,
/// and `arg2`.
func withUnsafePointers<A0, A1, A2, Result>(inout arg0: A0, inout arg1: A1, inout arg2: A2, body: (UnsafePointer<A0>, UnsafePointer<A1>, UnsafePointer<A2>) -> Result) -> Result


/// Invoke `f` with a C `va_list` argument derived from `builder`.
func withVaList<R>(builder: VaListBuilder, f: (CVaListPointer) -> R) -> R


/// Invoke `f` with a C `va_list` argument derived from `args`.
func withVaList<R>(args: [CVarArgType], f: (CVaListPointer) -> R) -> R

func |(lhs: Int, rhs: Int) -> Int

func |(lhs: UInt, rhs: UInt) -> UInt

func |(lhs: Int64, rhs: Int64) -> Int64

func |(lhs: UInt64, rhs: UInt64) -> UInt64

func |(lhs: Int32, rhs: Int32) -> Int32

func |(lhs: UInt32, rhs: UInt32) -> UInt32

func |(lhs: Int16, rhs: Int16) -> Int16

func |(lhs: UInt16, rhs: UInt16) -> UInt16

func |(lhs: Int8, rhs: Int8) -> Int8

func |(lhs: UInt8, rhs: UInt8) -> UInt8

func |<T : _RawOptionSetType>(a: T, b: T) -> T

func |(lhs: Bool, rhs: Bool) -> Bool

func |=(inout lhs: Int16, rhs: Int16)

func |=(inout lhs: UInt32, rhs: UInt32)

func |=(inout lhs: Int32, rhs: Int32)

func |=(inout lhs: UInt64, rhs: UInt64)

func |=(inout lhs: Int64, rhs: Int64)

func |=(inout lhs: UInt, rhs: UInt)

func |=(inout lhs: Int, rhs: Int)

func |=<T : BitwiseOperationsType>(inout lhs: T, rhs: T)

func |=(inout lhs: UInt16, rhs: UInt16)

func |=(inout lhs: Bool, rhs: Bool)

func |=(inout lhs: UInt8, rhs: UInt8)

func |=(inout lhs: Int8, rhs: Int8)


/// If `lhs` is `true`, return it.  Otherwise, evaluate `rhs` and
/// return its `boolValue`.
@inline(__always) func ||<T : BooleanType, U : BooleanType>(lhs: T, rhs: @autoclosure () -> U) -> Bool

func ||<T : BooleanType>(lhs: T, rhs: @autoclosure () -> Bool) -> Bool

prefix func ~(rhs: UInt32) -> UInt32

prefix func ~(rhs: Int64) -> Int64

prefix func ~(rhs: Int32) -> Int32

prefix func ~(rhs: UInt) -> UInt

prefix func ~(rhs: UInt64) -> UInt64

prefix func ~(rhs: Int) -> Int

prefix func ~(rhs: Int16) -> Int16

prefix func ~(rhs: UInt16) -> UInt16

prefix func ~(rhs: Int8) -> Int8

prefix func ~(rhs: UInt8) -> UInt8

prefix func ~<T : _RawOptionSetType>(a: T) -> T

prefix func ~(a: Bool) -> Bool


/// Returns `true` iff `pattern` contains `value`
func ~=<I : IntervalType>(pattern: I, value: I.Bound) -> Bool

func ~=<T : Equatable>(a: T, b: T) -> Bool

func ~=<T>(lhs: _OptionalNilComparisonType, rhs: T?) -> Bool


