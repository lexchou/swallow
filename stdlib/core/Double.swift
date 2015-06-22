struct Double {

    /// Create an instance initialized to zero.
    init() {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(_ value: Double) {
        //TODO
    }
}

extension Double : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension Double : FloatingPointType {

    /// The positive infinity.
    static var infinity: Double {
        get {
            return 0//TODO
        }
    }

    /// A quiet NaN.
    static var NaN: Double {
        get {
            return 0//TODO
        }
    }

    /// A quiet NaN.
    static var quietNaN: Double {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is negative
    var isSignMinus: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` iff `self` is normal (not zero, subnormal, infinity, or
    /// NaN).
    var isNormal: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` iff `self` is zero, subnormal, or normal (not infinity
    /// or NaN).
    var isFinite: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` iff `self` is +0.0 or -0.0.
    var isZero: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` iff `self` is subnormal.
    var isSubnormal: Bool {
        get  {
            return false//TODO
        }
    }

    /// `true` iff `self` is infinity.
    var isInfinite: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` iff `self` is NaN.
    var isNaN: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` iff `self` is a signaling NaN.
    var isSignaling: Bool {
        get {
            return false//TODO
        }
    }
}

extension Double {

    /// The IEEE 754 "class" of this type.
    var floatingPointClass: FloatingPointClassification {
        get {
            return .SignalingNaN//TODO
        }
    }
}

extension Double : IntegerLiteralConvertible {
    init(_builtinIntegerLiteral value: Builtin.Int2048) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64) {
        //TODO
    }
}

extension Double {
    init(_builtinFloatLiteral value: Builtin.FPIEEE80) {
        //TODO
    }
}

extension Double : FloatLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(floatLiteral value: Double) {
        //TODO
    }
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
    var hashValue: Int {
        get {
            return 0 //TODO
        }
    }
}

extension Double : AbsoluteValuable {

    /// Returns the absolute value of `x`
    static func abs(x: Double) -> Double {
        return self //TODO
    }
}

extension Double {
    init(_ v: UInt8) {
        //TODO
    }
    init(_ v: Int8) {
        //TODO
    }
    init(_ v: UInt16) {
        //TODO
    }
    init(_ v: Int16) {
        //TODO
    }
    init(_ v: UInt32) {
        //TODO
    }
    init(_ v: Int32) {
        //TODO
    }
    init(_ v: UInt64) {
        //TODO
    }
    init(_ v: Int64) {
        //TODO
    }
    init(_ v: UInt) {
        //TODO
    }
    init(_ v: Int) {
        //TODO
    }
}

extension Double {

    /// Construct an instance that approximates `other`.
    init(_ other: Float) {
        //TODO
    }

    /// Construct an instance that approximates `other`.
    init(_ other: Float80) {
        //TODO
    }
}

extension Double : Strideable {

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Double) -> Double {
        return self//TODO
    }

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    func advancedBy(amount: Double) -> Double {
        return self//TODO
    }

}

extension Double : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return Double.Type//TODO
    }
}

extension Double : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word] {
        return []//TODO
    }
}


/// A 64-bit floating point type
typealias Float64 = Double
