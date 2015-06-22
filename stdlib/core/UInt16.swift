/// A 16-bit unsigned integer value
/// type.
struct UInt16 : UnsignedIntegerType {
    var value: Builtin.Int16

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init() {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(_ value: UInt16) {
        //TODO
    }

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: UInt16) {
        //TODO
    }

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: UInt16) {
        //TODO
    }
    init(_builtinIntegerLiteral value: Builtin.Int2048) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(integerLiteral value: UInt16) {
        //TODO
    }

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: UInt16 {
        get {
            return 0//TODO
        }
    }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: UInt16 {
        get {
            return 0//TODO
        }
     }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: UInt16 {
        get {
            return 0//TODO
        }
    }
    static var max: UInt16 {
        get {
            return 0//TODO
        }
    }
    static var min: UInt16 {
        get {
            return 0//TODO
        }
    }
}

extension UInt16 : Hashable {

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

extension UInt16 : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension UInt16 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UInt16 {
        return 0//TODO
    }

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UInt16 {
        return 0//TODO
    }

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: UInt16) -> Distance {
        return 0//TODO
    }

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> UInt16 {
        return 0//TODO
    }
}

extension UInt16 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: UInt16, _ rhs: UInt16) -> (UInt16, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax {
        return 0//TODO
    }

    /// Explicitly convert to `IntMax`.
    func toIntMax() -> IntMax {
        return 0//TODO
    }
}

extension UInt16 {
    init(_ v: UInt8) {
        //TODO
    }
    init(_ v: Int8) {
        //TODO
    }
    init(_ v: Int16) {
        //TODO
    }
    init(_ v: UInt32) {
        //TODO
    }

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt32) {
        //TODO
    }
    init(_ v: Int32) {
        //TODO
    }

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int32) {
        //TODO
    }
    init(_ v: UInt64) {
        //TODO
    }

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64) {
        //TODO
    }
    init(_ v: Int64) {
        //TODO
    }

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64) {
        //TODO
    }
    init(_ v: UInt) {
        //TODO
    }

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt) {
        //TODO
    }
    init(_ v: Int) {
        //TODO
    }

    /// Construct a `UInt16` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int) {
        //TODO
    }

    /// Construct a `UInt16` having the same memory representation as
    /// the `Int16` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `UInt16` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: Int16) {
        //TODO
    }
}

extension UInt16 : BitwiseOperationsType {

    /// The empty bitset of type UInt16.
    static var allZeros: UInt16 {
        get {
            return 0//TODO
        }
    }
}

extension UInt16 {

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

extension UInt16 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}

extension UInt16 {
}

extension UInt16 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word] {
        return []//TODO
    }
}
