/// A *collection* that supports subscript assignment.
///
/// For any instance `a` of a type conforming to
/// `MutableCollectionType`, ::
///
///   a[i] = x
///   let y = a[i]
///
/// is equivalent to ::
///
///   a[i] = x
///   let y = x
///
protocol MutableCollectionType : CollectionType {
    subscript (position: Self.Index) -> Self.Generator.Element { get set }
}
