/// A *collection* from which a sub-range of elements (a "slice")
/// can be efficiently extracted.
protocol Sliceable : _Sliceable {

    /// The *collection* type that represents a sub-range of elements.
    ///
    /// Though it can't currently be enforced by the type system, the
    /// `SubSlice` type in a concrete implementation of `Sliceable`
    /// should also be `Sliceable`.
    typealias SubSlice : _Sliceable
    subscript (bounds: Range<Self.Index>) -> SubSlice { get }
}
