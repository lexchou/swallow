/// An interval over a `Comparable` type.
protocol IntervalType {

    /// The type of the `Interval`\ 's endpoints
    typealias Bound : Comparable

    /// Returns `true` iff the interval contains `value`
    func contains(value: Bound) -> Bool

    /// Return `rhs` clamped to `self`.  The bounds of the result, even
    /// if it is empty, are always within the bounds of `self`
    func clamp(intervalToClamp: Self) -> Self

    /// True iff `self` is empty
    var isEmpty: Bool { get }

    /// The `Interval`\ 's lower bound. Invariant: `start` <= `end`
    var start: Bound { get }

    /// The `Interval`\ 's upper bound. Invariant: `start` <= `end`
    var end: Bound { get }
}
