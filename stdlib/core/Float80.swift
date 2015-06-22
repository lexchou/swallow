struct Float80 {

    /// Create an instance initialized to zero.
    init() {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(_ value: Float80) {
        //TODO
    }
}

extension Float80 : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension Float80 : IntegerLiteralConvertible {
    init(_builtinIntegerLiteral value: Builtin.Int2048) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int64) {
        //TODO
    }
}

extension Float80 {
    init(_builtinFloatLiteral value: Builtin.FPIEEE80) {
        //TODO
    }
}

extension Float80 : FloatLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(floatLiteral value: Float80) {
        //TODO
    }
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
    var hashValue: Int {
        get {
            return 0//TODO
        }
    }
}

extension Float80 : AbsoluteValuable {

    /// Returns the absolute value of `x`
    static func abs(x: Float80) -> Float80 {
        return 0//TODO
    }
}

extension Float80 {
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

extension Float80 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float) {
        //TODO
    }

    /// Construct an instance that approximates `other`.
    init(_ other: Double) {
        //TODO
    }
}

extension Float80 : Strideable {

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Float80) -> Float80 {
        return 0//TODO
    }

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    func advancedBy(amount: Float80) -> Float80 {
        return 0//TODO
    }
}
