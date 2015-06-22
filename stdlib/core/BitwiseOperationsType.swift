/// A set type with O(1) standard bitwise operators.
///
/// Each instance is a subset of `~Self.allZeros`
///
/// Axioms, where `x` is an instance of `Self`::
///
///   x | Self.allZeros == x
///   x ^ Self.allZeros == x
///   x & Self.allZeros == .allZeros
///   x & ~Self.allZeros == x
///   ~x == x ^ ~Self.allZeros
protocol BitwiseOperationsType {

    /// Returns the intersection of bits set in `lhs` and `rhs`.
    ///
    /// Complexity: O(1)
    func &(lhs: Self, rhs: Self) -> Self

    /// Returns the union of bits set in `lhs` and `rhs`
    ///
    /// Complexity: O(1)
    func |(lhs: Self, rhs: Self) -> Self

    /// Returns the bits that are set in exactly one of `lhs` and `rhs`
    ///
    /// Complexity: O(1)
    func ^(lhs: Self, rhs: Self) -> Self

    /// Returns `x ^ ~Self.allZeros`
    ///
    /// Complexity: O(1)
    prefix func ~(x: Self) -> Self

    /// The empty bitset.
    ///
    /// Also the `identity element
    /// <http://en.wikipedia.org/wiki/Identity_element>`_ for `|` and
    /// `^`, and the `fixed point
    /// <http://en.wikipedia.org/wiki/Fixed_point_(mathematics)>`_ for
    /// `&`.
    class var allZeros: Self { get }
}
