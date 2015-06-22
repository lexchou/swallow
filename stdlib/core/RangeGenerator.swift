/// A generator over the elements of `Range<T>`
struct RangeGenerator<T : ForwardIndexType> : GeneratorType, SequenceType {

    /// The type of element returned by `next()`.
    typealias Element = T

    /// Construct an instance that traverses the elements of `bounds`
    init(_ bounds: Range<T>) {
        //TODO
    }

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    mutating func next() -> T? {
        return nil//TODO
    }

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = RangeGenerator<T>

    /// `RangeGenerator` is also a `SequenceType`, so it
    /// `generate`\ 's a copy of itself
    func generate() -> RangeGenerator<T> {
        return RangeGenerator<T>()//TODO
    }

    /// The lower bound of the remaining range.
    var startIndex: T

    /// The upper bound of the remaining range; not included in the
    /// generated sequence.
    var endIndex: T
}

