///
/// For example,
///
/// .. parsed-literal:
///
///      x[i..<j] = *someExpression*
///      x[i..<j].\ *mutatingMethod*\ ()
protocol MutableSliceable : Sliceable, MutableCollectionType {
    subscript (_: Range<Self.Index>) -> Self.SubSlice { get set }
}
