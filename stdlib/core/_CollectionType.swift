/// This protocol is an implementation detail of `CollectionType`; do
/// not use it directly.
///
/// Its requirements are inherited by `CollectionType` and thus must
/// be satisfied by types conforming to that protocol.
protocol _CollectionType : _SequenceType {

    /// A type that represents a valid position in the collection.
    ///
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index : ForwardIndexType

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: Index { get }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: Index { get }
    typealias _Element
    subscript (_i: Index) -> _Element { get }
}
