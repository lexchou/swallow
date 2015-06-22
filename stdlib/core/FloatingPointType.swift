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

