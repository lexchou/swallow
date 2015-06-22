/// A 32-bit signed integer value
/// type.
struct Int32 : SignedIntegerType {
    var value: Builtin.Int32

    /// A type that can represent the number of steps between pairs of
    /// values.
    typealias Distance = Int

    /// Create an instance initialized to zero.
    init() {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(_ value: Int32) {
        //TODO
    }

    /// Creates an integer from its big-endian representation, changing the
    /// byte order if necessary.
    init(bigEndian value: Int32) {
        //TODO
    }

    /// Creates an integer from its little-endian representation, changing the
    /// byte order if necessary.
    init(littleEndian value: Int32) {
        //TODO
    }
    init(_builtinIntegerLiteral value: Builtin.Int2048) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(integerLiteral value: Int32) {
        //TODO
    }

    /// Returns the big-endian representation of the integer, changing the
    /// byte order if necessary.
    var bigEndian: Int32 {
        get {
            return 0//TODO
        }
    }

    /// Returns the little-endian representation of the integer, changing the
    /// byte order if necessary.
    var littleEndian: Int32 {
        get {
            return 0//TODO
        }
    }

    /// Returns the current integer with the byte order swapped.
    var byteSwapped: Int32 {
        get {
            return 0//TODO
        }
    }
    static var max: Int32 {
        get {
            return 0//TODO
        }
    }
    static var min: Int32 {
        get {
            return 0//TODO
        }
    }
}

extension Int32 : Hashable {

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

extension Int32 : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        } 
    }
}

extension Int32 : RandomAccessIndexType {

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> Int32 {
        return 0//TODO
    }

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> Int32 {
        return 0//TODO
    }

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(other: Int32) -> Distance {
        return 0//TODO
    }

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(amount: Distance) -> Int32 {
        return 0//TODO
    }
}

extension Int32 {

    /// Add `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func addWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Subtract `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func subtractWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Multiply `lhs` and `rhs`, returning a result and a
    /// `Bool` that is true iff the operation caused an arithmetic
    /// overflow.
    static func multiplyWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Divide `lhs` and `rhs`, returning
    /// a result and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func divideWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Divide `lhs` and `rhs`, returning
    /// the remainder and a `Bool`
    /// that is true iff the operation caused an arithmetic overflow.
    static func remainderWithOverflow(lhs: Int32, _ rhs: Int32) -> (Int32, overflow: Bool) {
        return (0, false)//TODO
    }

    /// Represent this number using Swift's widest native signed
    /// integer type.
    func toIntMax() -> IntMax {
        return 0//TODO
    }
}

extension Int32 : SignedNumberType {
}

extension Int32 {
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
    init(_ v: UInt64) {
        //TODO
    }

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt64) {
        //TODO
    }
    init(_ v: Int64) {
        //TODO
    }

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int64) {
        //TODO
    }
    init(_ v: UInt) {
        //TODO
    }

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: UInt) {
        //TODO
    }
    init(_ v: Int) {
        //TODO
    }

    /// Construct a `Int32` having the same bitwise representation as
    /// the least significant bits of the provided bit pattern.
    ///
    /// No range or overflow checking occurs.
    init(truncatingBitPattern: Int) {
        //TODO
    }

    /// Construct a `Int32` having the same memory representation as
    /// the `UInt32` `bitPattern`.  No range or overflow checking
    /// occurs, and the resulting `Int32` may not have the same numeric
    /// value as `bitPattern`--it is only guaranteed to use the same
    /// pattern of bits.
    init(bitPattern: UInt32) {
        //TODO
    }
}

extension Int32 : BitwiseOperationsType {

    /// The empty bitset of type Int32.
    static var allZeros: Int32 {
        get {
            return 0//TODO
        }
    }
}

extension Int32 {

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

extension Int32 : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}

extension Int32 : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word] {
        return []//TODO
    }
}
