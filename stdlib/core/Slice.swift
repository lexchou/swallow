/// The `Array`-like type that represents a sub-sequence of any
/// `Array`, `ContiguousArray`, or other `Slice`.
///
/// `Slice` always uses contiguous storage and does not bridge to
/// Objective-C.
///
/// .. Warning:: Long-term storage of `Slice` instances is discouraged
///
///    Because a `Slice` presents a *view* onto the storage of some
///    larger array even after the original array's lifetime ends,
///    storing the slice may prolong the lifetime of elements that are
///    no longer accessible, which can manifest as apparent memory and
///    object leakage.  To prevent this effect, use `Slice` only for
///    transient computation.
struct Slice<T> : MutableCollectionType, Sliceable {

    /// The type of element stored by this `Slice`
    typealias Element = T

    /// Always zero, which is the index of the first element when non-empty.
    var startIndex: Int {
        get {
            return 0//TODO
        }
    }

    /// A "past-the-end" element index; the successor of the last valid
    /// subscript argument.
    var endIndex: Int {
        get {
            return 0//TODO
        }
    }
    subscript (index: Int) -> T {
        return T()//TODO
    }

    /// Return a *generator* over the elements.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<Slice<T>> {
        return IndexingGenerator<Slice<T>>()//TODO
    }

    /// A type that can represent a sub-range of a `Slice` 
    typealias SubSlice = Slice<T>
    subscript (subRange: Range<Int>) -> Slice<T> {
        return Slice<T>()//TODO
    }

    /// Initialization from an existing buffer does not have "array.init"
    /// semantics because the caller may retain an alias to buffer.
    init(_ buffer: _SliceBuffer<T>) {
        //TODO
    }
}

extension Slice : ArrayLiteralConvertible {

    /// Create an instance containing `elements`.
    init(arrayLiteral elements: T...) {
        //TODO
    }
}



extension Slice {

    /// Construct an empty Slice
    init() {
        //TODO
    }

    /// Construct from an arbitrary sequence with elements of type `T`
    init<S : SequenceType where T == T>(_ s: S) {
        //TODO
    }

    /// Construct a Slice of `count` elements, each initialized to
    /// `repeatedValue`.
    init(count: Int, repeatedValue: T) {
        //TODO
    }

    /// How many elements the Slice stores
    var count: Int {
        get {
            return 0//TODO
        }
    }

    /// How many elements the `Slice` can store without reallocation
    var capacity: Int {
        get {
            return 0//TODO
        }
    }

    /// `true` if and only if the `Slice` is empty
    var isEmpty: Bool {
        get {
            return 0//TODO
        }
    }

    /// The first element, or `nil` if the array is empty
    var first: T? {
        get {
            return nil//TODO
        }
    }

    /// The last element, or `nil` if the array is empty
    var last: T? {
        get {
            return nil//TODO
        }
    }

    /// Reserve enough space to store minimumCapacity elements.
    ///
    /// PostCondition: `capacity >= minimumCapacity` and the array has
    /// mutable contiguous storage.
    ///
    /// Complexity: O(`count`)
    mutating func reserveCapacity(minimumCapacity: Int) {
        //TODO
    }

    /// Append newElement to the Slice
    ///
    /// Complexity: amortized O(1) unless `self`'s storage is shared with another live array; O(`count`) otherwise.
    mutating func append(newElement: T) {
        //TODO
    }

    /// Append the elements of `newElements` to `self`.
    ///
    /// Complexity: O(*length of result*) 
    /// 
    mutating func extend<S : SequenceType where T == T>(newElements: S) {
        //TODO
    }

    /// Remove an element from the end of the Slice in O(1).
    /// Requires: count > 0
    mutating func removeLast() -> T {
        //TODO
    }

    /// Insert `newElement` at index `i`.
    ///
    /// Requires: `i <= count`
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func insert(newElement: T, atIndex i: Int) {
        //TODO
    }

    /// Remove and return the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAtIndex(index: Int) -> T {
        //TODO
    }

    /// Remove all elements.
    ///
    /// Postcondition: `capacity == 0` iff `keepCapacity` is `false`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeAll(keepCapacity: Bool = default) {
        //TODO
    }

    /// Interpose `self` between each consecutive pair of `elements`,
    /// and concatenate the elements of the resulting sequence.  For
    /// example, `[-1, -2].join([[1, 2, 3], [4, 5, 6], [7, 8, 9]])`
    /// yields `[1, 2, 3, -1, -2, 4, 5, 6, -1, -2, 7, 8, 9]`
    func join<S : SequenceType where Slice<T> == Slice<T>>(elements: S) -> Slice<T> {
        //TODO
    }

    /// Return the result of repeatedly calling `combine` with an
    /// accumulated value initialized to `initial` and each element of
    /// `self`, in turn, i.e. return
    /// `combine(combine(...combine(combine(initial, self[0]),
    /// self[1]),...self[count-2]), self[count-1])`.
    func reduce<U>(initial: U, combine: (U, T) -> U) -> U {
        //TODO
    }

    /// Sort `self` in-place according to `isOrderedBefore`.  Requires:
    /// `isOrderedBefore` induces a `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    mutating func sort(isOrderedBefore: (T, T) -> Bool) {
        //TODO
    }

    /// Return a copy of `self` that has been sorted according to
    /// `isOrderedBefore`.  Requires: `isOrderedBefore` induces a
    /// `strict weak ordering
    /// <http://en.wikipedia.org/wiki/Strict_weak_order#Strict_weak_orderings>`__
    /// over the elements.
    func sorted(isOrderedBefore: (T, T) -> Bool) -> Slice<T> {
        //TODO
    }

    /// Return a `Slice` containing the results of calling
    /// `transform(x)` on each element `x` of `self`
    func map<U>(transform: (T) -> U) -> Slice<U> {
        //TODO
    }

    /// A Slice containing the elements of `self` in reverse order
    func reverse() -> Slice<T> {
        //TODO
    }

    /// Return a `Slice` containing the elements `x` of `self` for which
    /// `includeElement(x)` is `true`
    func filter(includeElement: (T) -> Bool) -> Slice<T> {
        //TODO
    }
}

extension Slice : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}

extension Slice : Printable, DebugPrintable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//todo
        }
    }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}



extension Slice {

    /// Call `body(p)`, where `p` is a pointer to the `Slice`\ 's
    /// contiguous storage.
    ///
    /// Often, the optimizer can eliminate bounds checks within an
    /// array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    func withUnsafeBufferPointer<R>(body: (UnsafeBufferPointer<T>) -> R) -> R {
        //TODO
    }

    /// Call `body(p)`, where `p` is a pointer to the `Slice`\ 's
    /// mutable contiguous storage.
    ///
    /// Often, the optimizer can eliminate bounds- and uniqueness-checks
    /// within an array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    mutating func withUnsafeMutableBufferPointer<R>(body: (inout UnsafeMutableBufferPointer<T>) -> R) -> R {
        //TODO
    }
}

extension Slice {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if `subRange.endIndex
    /// == self.endIndex` and `isEmpty(newElements)`\ , O(N) otherwise.
    mutating func replaceRange<C : CollectionType where T == T>(subRange: Range<Int>, with newElements: C) {
        //TODO
    }

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count + countElements(newElements)`\ ).
    mutating func splice<S : CollectionType where T == T>(newElements: S, atIndex i: Int) {
        //TODO
    }

    /// Remove the indicated `subRange` of elements
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeRange(subRange: Range<Int>) {
        //TODO
    }
}
