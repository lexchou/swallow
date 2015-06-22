/// A Unicode scalar value.
struct UnicodeScalar : UnicodeScalarLiteralConvertible {

    /// A numeric representation of `self`.
    var value: UInt32 { 
		get {
			return 0//TODO
		}
	}
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32) {
		//TODO
	}

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: UnicodeScalar) {
		//TODO
	}

    /// Creates an instance of the NUL scalar value.
    init() {
		//TODO
	}

    /// Create an instance with numeric value `v`.
    ///
    /// Requires: `v` is a valid Unicode scalar value.
    init(_ v: UInt32) {
		//TODO
	}

    /// Create an instance with numeric value `v`.
    ///
    /// Requires: `v` is a valid Unicode scalar value.
    init(_ v: UInt16) {
		//TODO
	}

    /// Create an instance with numeric value `v`.
    init(_ v: UInt8) {
		//TODO
	}

    /// Create a duplicate of `v`.
    init(_ v: UnicodeScalar) {
		//TODO
	}

    /// Return a String representation of `self` .
    ///
    /// :param: `asASCII`, if `true`, forces most values into a numeric
    /// representation.
    func escape(#asASCII: Bool) -> String {
		return ""//TODO
	}

    /// Returns true if this is an ASCII character (code point 0 to 127
    /// inclusive).
    func isASCII() -> Bool {
		return false//TODO
	}
}

extension UnicodeScalar : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
		//TODO
	}
}

extension UnicodeScalar : Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target) {
		//TODO
	}
}

extension UnicodeScalar : Printable, DebugPrintable {

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

extension UnicodeScalar : Hashable {

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

extension UnicodeScalar {

    /// Construct with value `v`.
    ///
    /// Requires: `v` is a valid unicode scalar value.
    init(_ v: Int) {
		//TODO
	}
}

extension UnicodeScalar : Comparable {
}

extension UnicodeScalar {
}
