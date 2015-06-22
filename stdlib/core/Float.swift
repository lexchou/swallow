struct Float {

    /// Create an instance initialized to zero.
    init() {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(_ value: Float) {
        //TODO
    }

}

extension Float : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension Float : FloatingPointType {

    /// The positive infinity.
    static var infinity: Float {
        get {
            return 0//TODO
        }
    }

    /// A quiet NaN.
    static var NaN: Float {
        get {
            return 0//TODO
        }
    }

    /// A quiet NaN.
    static var quietNaN: Float {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is negative
    var isSignMinus: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is normal (not zero, subnormal, infinity, or
    /// NaN).
    var isNormal: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is zero, subnormal, or normal (not infinity
    /// or NaN).
    var isFinite: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is +0.0 or -0.0.
    var isZero: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is subnormal.
    var isSubnormal: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is infinity.
    var isInfinite: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is NaN.
    var isNaN: Bool {
        get {
            return 0//TODO
        }
    }

    /// `true` iff `self` is a signaling NaN.
    var isSignaling: Bool {
        get {
            return 0//TODO
        }
    }
}

extension Float {

    /// The IEEE 754 "class" of this type.
    var floatingPointClass: FloatingPointClassification {
        get {
            return .SignalingNaN//TODO
        }
    }
}

extension Float : IntegerLiteralConvertible {
    init(_builtinIntegerLiteral value: Builtin.Int2048) {
        //TODO
    }


    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64)  {
        //TODO
    }


}

extension Float {
    init(_builtinFloatLiteral value: Builtin.FPIEEE80)  {
        //TODO
    }

}

extension Float : FloatLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(floatLiteral value: Float) {
        //TODO
    }

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
    var hashValue: Int {
        get {
            return 0//TODO
        }
    }
}

extension Float : AbsoluteValuable {

    /// Returns the absolute value of `x`
    static func abs(x: Float) -> Float {
        return 0//TODO
    }
}

extension Float {
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

extension Float {

    /// Construct an instance that approximates `other`.
    init(_ other: Double) {
        //TODO
    }


    /// Construct an instance that approximates `other`.
    init(_ other: Float80) {
        //TODO
    }

}

extension Float : Strideable {

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Float) -> Float {
        return 0//TODO
    }

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    func advancedBy(amount: Float) -> Float {
        return 0//TODO
    }
}

extension Float : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return Float.Type//TODO
    }

}

extension Float : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word] {
        return []
    }
}


/// A 32-bit floating point type
typealias Float32 = Float
