/// This protocol is an implementation detail of `SequenceType`; do
/// not use it directly.
///
/// Its requirements are inherited by `SequenceType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Sequence_Type : _SequenceType {

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator : GeneratorType

    /// Return a *generator* over the elements of this *sequence*.  The
    /// *generator*\ 's next element is the first element of the
    /// sequence.
    ///
    /// Complexity: O(1)
    func generate() -> Generator
}
