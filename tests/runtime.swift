
protocol Equatable {
    //func ==(lhs: Self, rhs: Self) -> Bool
}
protocol Hashable : Equatable {
    //var hashValue: Int { get }
}
protocol _Comparable {
    //func <(lhs: Self, rhs: Self) -> Bool
}

protocol Comparable : _Comparable, Equatable {
    //func <=(lhs: Self, rhs: Self) -> Bool
    //func >=(lhs: Self, rhs: Self) -> Bool
    //func >(lhs: Self, rhs: Self) -> Bool
}


typealias Void = ()

struct String
{
}


/// Conforming types can be initialized with integer literals
protocol IntegerLiteralConvertible {
    typealias IntegerLiteralType
//    init(integerLiteral value: IntegerLiteralType)
}

protocol Printable
{
}

/// This protocol is an implementation detail of `IntegerType`; do
/// not use it directly.
///
/// Its requirements are inherited by `IntegerType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _IntegerType : IntegerLiteralConvertible, Printable, Hashable {
}
protocol IntegerType : _IntegerType {
}
protocol SignedIntegerType : IntegerType {
}
protocol UnsignedIntegerType : IntegerType {
}




protocol SignedNumberType : IntegerLiteralConvertible {
    //prefix func -(x: Self) -> Self
}

struct Int : SignedIntegerType {}
struct Int8 : SignedIntegerType {}
struct Int16 : SignedIntegerType {}
struct Int32 : SignedIntegerType {}
struct Int64 : SignedIntegerType {}


typealias IntegerLiteralType = Int

struct UInt : UnsignedIntegerType {
}
struct UInt8 : SignedIntegerType {}
struct UInt16 : SignedIntegerType {}
struct UInt32 : SignedIntegerType {}
struct UInt64 : SignedIntegerType {}


protocol NilLiteralConvertible {}

protocol StringInterpolationConvertible {
}
protocol BooleanLiteralConvertible {
}
protocol StringLiteralConvertible {
}
protocol FloatLiteralConvertible {
}

protocol FloatingPointType : Comparable
{
}

struct Float : FloatingPointType, FloatLiteralConvertible
{
}

struct Double : FloatingPointType, FloatLiteralConvertible
{
}


protocol BooleanType
{
}

struct Bool : BooleanType, BooleanLiteralConvertible
{
}

protocol RawRepresentable {
    //typealias RawValue
    //init?(rawValue: RawValue)
    //var rawValue: RawValue { get }
}



protocol UnicodeScalarLiteralConvertible {
    typealias UnicodeScalarLiteralType

    /// Create an instance initialized to `value`.
    //init(unicodeScalarLiteral value: UnicodeScalarLiteralType)
}


protocol ExtendedGraphemeClusterLiteralConvertible : UnicodeScalarLiteralConvertible {
    typealias ExtendedGraphemeClusterLiteralType

    /// Create an instance initialized to `value`.
//    init(extendedGraphemeClusterLiteral value: ExtendedGraphemeClusterLiteralType)
}


struct Character : ExtendedGraphemeClusterLiteralConvertible, Equatable, Hashable, Comparable {
    //case LargeRepresentation(OnHeap<String>)
    //case SmallRepresentation
}


protocol SequenceType {}
protocol CollectionType : SequenceType {}
protocol ArrayLiteralConvertible {}

struct Array<T> : CollectionType, ArrayLiteralConvertible
{
   func append(item : T) {}

}
protocol DictionaryLiteralConvertible {}
struct Dictionary<Key : Hashable, Value> : CollectionType, DictionaryLiteralConvertible {
}

enum Optional<T> : NilLiteralConvertible {
    case None
    case Some(T)
}
struct _OptionalNilComparisonType : NilLiteralConvertible {
}
