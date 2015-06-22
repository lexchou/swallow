/// Instances of conforming types provide an integer `hashValue` and
/// can be used as `Dictionary` keys.
protocol Hashable : Equatable {

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int { get }
}
