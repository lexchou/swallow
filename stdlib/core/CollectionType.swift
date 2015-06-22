/// A multi-pass *sequence* with addressable positions.
///
/// Positions are represented by an associated `Index` type.  Whereas
/// an arbitrary *sequence* may be consumed as it is traversed, a
/// *collection* is multi-pass: any element may be revisited merely by
/// saving its index.
///
/// The sequence view of the elements is identical to the collection
/// view.  In other words, the following code binds the same series of
/// values to `x` as does `for x in self {}`::
///
///   for i in startIndex..<endIndex {
///     let x = self[i]
///   }
protocol CollectionType : _CollectionType, SequenceType {
    subscript (position: Self.Index) -> Self.Generator.Element { get }
}
