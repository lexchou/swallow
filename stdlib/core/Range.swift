/// A collection of consecutive discrete index values.
///
/// :param: `T` is both the element type and the index type of the
///   collection.
///
/// Like other collections, a range containing one element has an
/// `endIndex` that is the successor of its `startIndex`; and an empty
/// range has `startIndex == endIndex`.
///
/// Axiom: for any `Range` `r`, `r[i] == i`.
///
/// Therefore, if `T` has a maximal value, it can serve as an
/// `endIndex`, but can never be contained in a `Range<T>`.
///
/// It also follows from the axiom above that `(-99..<100)[0] == 0`.
/// To prevent confusion (because some expect the result to be `-99`),
/// in a context where `T` is known to be an integer type,
/// subscripting with `T` is a compile-time error::
///
///   // error: could not find an overload for 'subscript'...
///   println( Range<Int>(start:-99, end:100)[0] )
///
/// However, subscripting that range still works in a generic context::
///
///   func brackets<T:ForwardIndexType>(x: Range<T>, i: T) -> T {
///     return x[i] // Just forward to subscript
///   }
///   println(brackets(Range<Int>(start:-99, end:100), 0)) // prints 0
struct Range<T : ForwardIndexType> : Equatable, CollectionType, Printable, DebugPrintable {

    /// Construct a copy of `x`
    init(_ x: Range<T>) {
        //TODO
    }

    /// Construct a range with `startIndex == start` and `endIndex ==
    /// end`.
    init(start: T, end: T) {
        //TODO
    }

    /// `true` iff the range is empty, i.e. `startIndex == endIndex`
    var isEmpty: Bool {
        get {
            return false//TODO
        }
    }

    /// A type that represents a valid position in the collection.
    /// 
    /// Valid indices consist of the position of every element and a
    /// "past the end" position that's not valid for use as a subscript.
    typealias Index = T
    typealias Slice = Range<T>
    subscript (position: T) -> T {
        get {
            return T()//TODO
        }
    }
    subscript (_: T._DisabledRangeIndex) -> T {
        get {
            return T()
        }
    }

    /// A type whose instances can produce the elements of this
    /// sequence, in order.
    typealias Generator = RangeGenerator<T>

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> RangeGenerator<T> {
        //TODO
    }

    /// The range's lower bound
    ///
    /// Identical to `endIndex` in an empty range.
    var startIndex: T

    /// The range's upper bound
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: T

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}

extension Range {

    /// Return an array containing the results of calling
    /// `transform(x)` on each element `x` of `self`.
    func map<U>(transform: (T) -> U) -> [U] {
        return []//TODO
    }
}

extension Range : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}
