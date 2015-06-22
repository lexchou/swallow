/// This protocol is an implementation detail of `RandomAccessIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `RandomAccessIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _RandomAccessIndexType : _BidirectionalIndexType, Strideable {

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    ///
    /// Axioms::
    /// 
    ///   x.distanceTo(x.successor())) == 1
    ///   x.distanceTo(x.predecessor())) == -1
    ///   x.advancedBy(x.distanceTo(y)) == y
    func distanceTo(other: Self) -> Self.Distance

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    ///
    /// Axioms::
    ///
    ///   x.advancedBy(0) == x
    ///   x.advancedBy(1) == x.successor()
    ///   x.advancedBy(-1) == x.predecessor()
    ///   x.distanceTo(x.advancedBy(m)) == m
    func advancedBy(n: Self.Distance) -> Self
}
