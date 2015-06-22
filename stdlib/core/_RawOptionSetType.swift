/// This protocol is an implementation detail of `RawOptionSetType`; do
/// not use it directly.
///
/// Its requirements are inherited by `RawOptionSetType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _RawOptionSetType : RawRepresentable, Equatable {
    typealias RawValue : BitwiseOperationsType, Equatable
    init(rawValue: RawValue)
}
