/// A unique identifier for a class instance or metatype. This can be used by
/// reflection clients to recognize cycles in the object graph.
///
/// In Swift, only class instances and metatypes have unique identities. There
/// is no notion of identity for structs, enums, functions, or tuples.
struct ObjectIdentifier : Hashable, Comparable {

    /// Convert to a `UInt` that captures the full value of `self`.
    ///
    /// Axiom: `a.uintValue == b.uintValue` iff `a == b`
    var uintValue: UInt {
        get {
            return 0//TODO
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

    /// Construct an instance that uniquely identifies the class instance `x`.
    init(_ x: AnyObject) {
        //TODO
    }

    /// Construct an instance that uniquely identifies the metatype `x`.
    init(_ x: Any.Type) {
        //TODO
    }
}
