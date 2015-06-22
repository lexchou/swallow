/// This protocol is an implementation detail of `Strideable`; do
/// not use it directly.
///
/// Its requirements are inherited by `Strideable` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Strideable {

    /// A type that can represent the distance between two values of `Self`
    typealias Stride : SignedNumberType

    /// Returns a stride `x` such that `self.advancedBy(x)` approximates
    /// `other`.
    ///
    /// Complexity: O(1).
    ///
    /// See also: `RandomAccessIndexType`\ 's `distanceTo`, which provides a
    /// stronger semantic guarantee.
    func distanceTo(other: Self) -> Stride

    /// Returns a `Self` `x` such that `self.distanceTo(x)` approximates
    /// `n`.
    ///
    /// Complexity: O(1).
    ///
    /// See also: `RandomAccessIndexType`\ 's `advancedBy`, which
    /// provides a stronger semantic guarantee.
    func advancedBy(n: Stride) -> Self
}
