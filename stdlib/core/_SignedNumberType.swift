/// This protocol is an implementation detail of `SignedNumberType`; do
/// not use it directly.
///
/// Its requirements are inherited by `SignedNumberType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _SignedNumberType : Comparable, IntegerLiteralConvertible {

    /// Return the difference between `lhs` and `rhs`.
    func -(lhs: Self, rhs: Self) -> Self
}