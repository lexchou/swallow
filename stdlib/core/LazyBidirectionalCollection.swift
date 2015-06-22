/// A collection that forwards its implementation to an underlying
/// collection instance while exposing lazy computations as methods.
struct LazyBidirectionalCollection<S : CollectionType where S.Index : BidirectionalIndexType> : CollectionType {

    /// Construct an instance with `base` as its underlying collection
    /// instance.
    init(_ base: S) {
        //TODO
    }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> S.Generator {
        return S.Generator() //TODO
    }

    /// The position of the first element in a non-empty collection.
    ///
    /// Identical to `endIndex` in an empty collection.
    var startIndex: S.Index {
        get {
            return S.Index()//TODO
        }
    }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: S.Index {
        get {
            return S.Index()//TODO
        }
    }

    /// True if and only if the collection is empty
    var isEmpty: Bool {
        get {
            return false//TODO
        }
    }

    /// The first element, or `nil` if `self` is empty
    var first: S.Generator.Element? {
        get {
            return nil//TODO
        }
    }

    /// The last element, or `nil` if `self` is empty
    var last: S.Generator.Element? {
        get {
            return nil//TODO
        }
    }
    subscript (position: S.Index) -> S.Generator.Element {
        get {
            return S.Generator.Element()//TODO
        }
    }

    /// an Array, created on-demand, containing the elements of this
    /// lazy CollectionType.
    var array: [S.Generator.Element] {
        get {
            return []//TODO
        }
    }
}

extension LazyBidirectionalCollection {

    /// Return a lazy SequenceType containing the elements `x` of `source` for
    /// which `includeElement(x)` is `true`
    func filter(includeElement: (S.Generator.Element) -> Bool) -> LazySequence<FilterSequenceView<S>> {
        //TODO
    }
}

extension LazyBidirectionalCollection {

    /// Return a `MapCollectionView` over this `LazyBidirectionalCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func map<U>(transform: (S.Generator.Element) -> U) -> LazyBidirectionalCollection<MapCollectionView<S, U>> {
        //TODO
    }
}

extension LazyBidirectionalCollection {

    /// Return a `BidirectionalReverseView` over this `LazyBidirectionalCollection`.  The elements of
    /// the result are computed lazily, each time they are read, by
    /// calling `transform` function on a base element.
    func reverse() -> LazyBidirectionalCollection<BidirectionalReverseView<S>> {
        //TODO
    }
}
