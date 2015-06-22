/// This protocol is an implementation detail of `IntegerType`; do
/// not use it directly.
///
/// Its requirements are inherited by `IntegerType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _IntegerType : IntegerLiteralConvertible, Printable, Hashable, IntegerArithmeticType, BitwiseOperationsType, _Incrementable {
}
