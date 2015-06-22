/// A value type whose instances are either `true` or `false`.
struct Bool {

    /// Default-initialize Boolean value to `false`.
    init() {
        //TODO
    }
}

extension Bool : BooleanLiteralConvertible {
    init(_builtinBooleanLiteral value: Builtin.Int1) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(booleanLiteral value: Bool) {
        //TODO
    }
}

extension Bool : BooleanType {

    /// Identical to `self`.
    var boolValue: Bool {
        get {
            return false//TODO
        }
    }

    /// Construct an instance representing the same logical value as
    /// `value`
    init<T : BooleanType>(_ value: T) {
        //TODO
    }
}

extension Bool : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension Bool : Equatable, Hashable {

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

extension Bool : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return Bool.Type//TODO
    }
}

