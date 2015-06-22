/// The common requirements for types that support integer arithmetic.
protocol IntegerArithmeticType : _IntegerArithmeticType, Comparable {

    /// Add `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func +(lhs: Self, rhs: Self) -> Self

    /// Subtract `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func -(lhs: Self, rhs: Self) -> Self

    /// Multiply `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func *(lhs: Self, rhs: Self) -> Self

    /// Divide `lhs` and `rhs`, returning a result and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func /(lhs: Self, rhs: Self) -> Self

    /// Divide `lhs` and `rhs`, returning the remainder and trapping in case of
    /// arithmetic overflow (except in -Ounchecked builds).
    func %(lhs: Self, rhs: Self) -> Self

    /// Explicitly convert to `IntMax`, trapping on overflow (except in
    /// -Ounchecked builds).
    func toIntMax() -> IntMax
}
