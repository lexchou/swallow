/// This protocol is an implementation detail of `ForwardIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `ForwardIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _ForwardIndexType : _Incrementable {

    /// A type that can represent the number of steps between pairs of
    /// `Self` values where one value is reachable from the other.
    ///
    /// Reachability is defined by the ability to produce one value from
    /// the other via zero or more applications of `successor`.
    typealias Distance : _SignedIntegerType = Int
    typealias _DisabledRangeIndex = _DisabledRangeIndex_
}
