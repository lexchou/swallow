/// This protocol is an implementation detail of `SignedIntegerType`;
/// do not use it directly.
///
/// Its requirements are inherited by `SignedIntegerType` and thus
/// must be satisfied by types conforming to that protocol.
protocol _UnsignedIntegerType : _IntegerType {

    /// Represent this number using Swift's widest native unsigned
    /// integer type.
    func toUIntMax() -> UIntMax

    /// Convert from Swift's widest unsigned integer type, trapping on
    /// overflow.
    init(_: UIntMax)
}
