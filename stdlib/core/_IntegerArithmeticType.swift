/// This protocol is an implementation detail of `IntegerArithmeticType`; do
/// not use it directly.
///
/// Its requirements are inherited by `IntegerArithmeticType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _IntegerArithmeticType {

    /// Add `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func addWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Subtract `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func subtractWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Multiply `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func multiplyWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning a result and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func divideWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)

    /// Divide `lhs` and `rhs`, returning the remainder and a `Bool` that is
    /// true iff the operation caused an arithmetic overflow.
    class func remainderWithOverflow(lhs: Self, _ rhs: Self) -> (Self, overflow: Bool)
}
