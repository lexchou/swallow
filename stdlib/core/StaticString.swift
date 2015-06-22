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
    var utf8Start: UnsafePointer<UInt8> {
        get {
            return nil//TODO
        }
    }

    /// The stored Unicode scalar value
    ///
    /// Requires: `self` stores a single Unicode scalar value.
    var unicodeScalar: UnicodeScalar {
        get {
            return nil//TODO
        }
    }

    /// If `self` stores a pointer to ASCII or UTF-8 code units, the
    /// length in bytes of that data.
    ///
    /// If `self` stores a single Unicode scalar value, the value of
    /// `byteSize` is unspecified.
    var byteSize: Word {
        get {
            return nil//TODO
        }
    }

    /// `true` iff `self` stores a pointer to ASCII or UTF-8 code units
    var hasPointerRepresentation: Bool {
        get {
            return false//TODO
        }
    }

    /// `true` if `self` stores a pointer to ASCII code units.
    ///
    /// If `self` stores a single Unicode scalar value, the value of
    /// `isASCII` is unspecified.
    var isASCII: Bool {
        get {
            return false//TODO
        }
    }

    /// Invoke `body` with a buffer containing the UTF-8 code units of
    /// `self`.
    ///
    /// This method works regardless of what `self` stores.
    func withUTF8Buffer<R>(body: (UnsafeBufferPointer<UInt8>) -> R) -> R {
        return R()//TODO
    }

    /// Return a `String` representing the same sequence of Unicode
    /// scalar values as `self` does.
    var stringValue: String {
        get {
            return ""//TODO
        }
    }

    /// Create an empty instance.
    init() {
        //TODO
    }
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: StaticString) {
        //TODO
    }
    init(_builtinExtendedGraphemeClusterLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: StaticString) {
        //TODO
    }
    init(_builtinStringLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(stringLiteral value: StaticString) {
        //TODO
    }

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}
