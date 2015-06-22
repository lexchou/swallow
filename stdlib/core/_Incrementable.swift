/// This protocol is an implementation detail of `ForwardIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `ForwardIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Incrementable : Equatable {

    /// Return the next consecutive value in a discrete sequence of
    /// `Self` values
    ///
    /// Requires: `self` has a well-defined successor.
    func successor() -> Self
}
