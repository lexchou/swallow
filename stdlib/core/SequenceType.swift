/// A type that can be iterated with a `for`\ ...\ `in` loop.
///
/// `SequenceType` makes no requirement on conforming types regarding
/// whether they will be destructively "consumed" by iteration.  To
/// ensure non-destructive iteration, constrain your *sequence* to
/// `CollectionType`.
protocol SequenceType : _Sequence_Type {

    /// A type that provides the *sequence*\ 's iteration interface and
    /// encapsulates its iteration state.
    typealias Generator : GeneratorType

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> Generator
}
