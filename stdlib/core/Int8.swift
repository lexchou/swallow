/// A 8-bit signed integer value
/// type.
struct Int8 : SignedIntegerType {
    var value: Builtin.Int8

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init() {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(_ value: Int8) {
        //TODO
    }
    init(_builtinIntegerLiteral value: Builtin.Int2048) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int8) {
        //TODO
    }
    static var max: Int8 {
        get {
            return 0//TODO
        }
    }
    static var min: Int8 {
        get  {
            return 0//TODO
        }
    }
}

extension Int8 : Hashable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int {
        get  {
            return 0//TODO
        }
    }
}

extension Int8 : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension Int8 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int8 {
        return 0//TODO
    }

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int8 {
        return 0//TODO
    }

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int8) -> Distance {
        return 0//TODO
    }

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int8 {
        return 0//TODO
    }
}

extension Int8 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int8, _ rhs: Int8) -> (Int8, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax {
        return 0//TODO
    }
}

extension Int8 : SignedNumberType {
}

extension Int8 {
    init(_ v: UInt8) {
        //TODO
    }
    init(_ v: UInt16) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt16) {
        //TODO
    }
    init(_ v: Int16) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int16) {
        //TODO
    }
    init(_ v: UInt32) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt32) {
        //TODO
    }
    init(_ v: Int32) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int32) {
        //TODO
    }
    init(_ v: UInt64) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64) {
        //TODO
    }
    init(_ v: Int64) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64) {
        //TODO
    }
    init(_ v: UInt) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt) {
        //TODO
    }
    init(_ v: Int) {
        //TODO
    }

    /// Construct a `Int8` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int) {
        //TODO
    }

    /// Construct a `Int8` having the same memory representation as
    /// the `UInt8` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int8` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt8) {
        //TODO
    }
}

extension Int8 : BitwiseOperationsType {

    /// The empty bitset of type Int8.
    static var allZeros: Int8 {
        get  {
            return 0//TODO
        }
    }
}

extension Int8 {

    /// Construct an instance that approximates `other`.
    init(_ other: Float) {
        //TODO
    }

    /// Construct an instance that approximates `other`.
    init(_ other: Double) {
        //TODO
    }

    /// Construct an instance that approximates `other`.
    init(_ other: Float80) {
        //TODO
    }
}

extension Int8 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}

extension Int8 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word] {
        return []//TODO
    }
}
