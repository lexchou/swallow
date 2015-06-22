/// A *generator* for an arbitrary *collection*.  Provided `C`
/// conforms to the other requirements of *CollectionType*,
/// `IndexingGenerator<C>` can be used as the result of `C`\ 's
/// `generate()` method.  For example:
///
/// .. parsed-literal::
///
///    struct MyCollection : CollectionType {
///      struct Index : ForwardIndexType { *implementation hidden* }
///      subscript(i: Index) -> MyElement { *implementation hidden* }
///      func generate() -> **IndexingGenerator<MyCollection>** {
///        return IndexingGenerator(self)
///      }
///    }
struct IndexingGenerator<C : _CollectionType> : GeneratorType, SequenceType {

    /// Create a *generator* over the given collection
    init(_ seq: C) {
        //TODO
    }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<C> {
        return self;//TODO
    }

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: no preceding call to `self.next()` has returned `nil`.
    mutating func next() -> C._Element? {
        return nil//TODO
    }
}
