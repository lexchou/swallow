/// A pointer to an object of type `T`.  This type provides no automated
/// memory management, and therefore the user must take care to allocate
/// and free memory appropriately.
///
/// The pointer can be in one of the following states:
///
/// - memory is not allocated (for example, pointer is null, or memory has
/// been deallocated previously);
///
/// - memory is allocated, but value has not been initialized;
///
/// - memory is allocated and value is initialized.
struct UnsafePointer<T> : RandomAccessIndexType, Hashable, NilLiteralConvertible {

    /// Construct a null pointer.
    init() {
        //TODO
    }

    /// Construct an `UnsafePointer` from a builtin raw pointer.
    init(_ value: Builtin.RawPointer) {
        //TODO
    }

    /// Convert from an opaque C pointer to a typed pointer
    ///
    /// This is a fundamentally unsafe conversion.
    init(_ other: COpaquePointer) {
        //TODO
    }

    /// Construct an `UnsafePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: Word) {
        //TODO
    }

    /// Construct an `UnsafePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: UWord) {
        //TODO
    }

    /// Convert from an UnsafeMutablePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafeMutablePointer<U>) {
        //TODO
    }

    /// Convert from a UnsafePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafePointer<U>) {
        //TODO
    }

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ()) {
        //TODO
    }

    /// Return a `nil` instance.
    static func null() -> UnsafePointer<T> {
        return UnsafePointer<T>()//TODO
    }

    /// Access the underlying raw memory, getting and
    /// setting values.
    var memory: T {
        get {
            return T()//TODO
        }
    }
    subscript (i: Int) -> T {
        get {
            return T()//TODO
        }
    }

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

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UnsafePointer<T> {
        return UnsafePointer<T>()//TODO
    }

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UnsafePointer<T> {
        return UnsafePointer<T>()//TODO
    }

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(x: UnsafePointer<T>) -> Int {
        return UnsafePointer<T>()//TODO
    }

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(n: Int) -> UnsafePointer<T> {
        return UnsafePointer<T>()//TODO
    }
}

extension UnsafePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}

extension UnsafePointer : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return //TODO
    }
}
