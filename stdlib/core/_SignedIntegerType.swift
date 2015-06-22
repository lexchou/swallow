/// This protocol is an implementation detail of `SignedIntegerType`;
/// do not use it directly.
///
/// Its requirements are inherited by `SignedIntegerType` and thus
/// must be satisfied by types conforming to that protocol.
protocol _SignedIntegerType : _IntegerType, SignedNumberType {

    /// Represent this number using Swift's widest native signed integer
    /// type.
    func toIntMax() -> IntMax

    /// Convert from Swift's widest signed integer type, trapping on
    /// overflow.
    init(_: IntMax)
}
