/// Instances of conforming types can be subtracted, arithmetically
/// negated, and initialized from `0`.
///
/// Axioms:
///
/// - `x - 0 == x`
/// - `-x == 0 - x`
/// - `-(-x) == x`
protocol SignedNumberType : _SignedNumberType {

    /// Return the result of negating `x`.
    prefix func -(x: Self) -> Self
}
