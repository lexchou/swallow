/// This protocol is an implementation detail of `BidirectionalIndexType`; do
/// not use it directly.
///
/// Its requirements are inherited by `BidirectionalIndexType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _BidirectionalIndexType : _ForwardIndexType {

    /// Return the previous consecutive value in a discrete sequence.
    ///
    /// If `self` has a well-defined successor,
    /// `self.successor().predecessor() == self`.  If `self` has a
    /// well-defined predecessor, `self.predecessor().successor() ==
    /// self`.
    ///
    /// Requires: `self` has a well-defined predecessor.
    func predecessor() -> Self
}
