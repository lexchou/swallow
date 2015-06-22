/// Conceptually_, `Array` is an efficient, tail-growable random-access
/// collection of arbitrary elements.
///
/// Common Properties of Array Types
/// ================================
///
/// The information in this section applies to all three of Swift's
/// array types, `Array<T>`, `ContiguousArray<T>`, and `Slice<T>`.
/// When you read the word "array" here in a normal typeface, it
/// applies to all three of them.
///
/// Value Semantics
/// ---------------
///
/// Each array variable, `let` binding, or stored property has an
/// independent value that includes the values of all of its elements.
/// Therefore, mutations to the array are not observable through its
/// copies::
///
///   var a = [1, 2, 3]
///   var b = a
///   b[0] = 4
///   println("a=\(a), b=\(b)")     // a=[1, 2, 3], b=[4, 2, 3]
///
/// (Of course, if the array stores `class` references, the objects
/// are shared; only the values of the references are independent)
///
/// Arrays use Copy-on-Write so that their storage and elements are
/// only copied lazily, upon mutation, when more than one array
/// instance is using the same buffer.  Therefore, the first in any
/// sequence of mutating operations may cost `O(N)` time and space,
/// where `N` is the length of the array.
///
/// Growth and Capacity
/// -------------------
///
/// When an array's contiguous storage fills up, new storage must be
/// allocated and elements must be moved to the new storage.  `Array`,
/// `ContiguousArray`, and `Slice` share an exponential growth
/// strategy that makes `append` a constant time operation *when
/// amortized over many invocations*.  In addition to a `count`
/// property, these array types have a `capacity` that reflects their
/// potential to store elements without reallocation, and when you
/// know how many elements you'll store, you can call
/// `reserveCapacity` to pre-emptively reallocate and prevent
/// intermediate reallocations.
///
/// .. _Conceptually:
///
/// Objective-C Bridge
/// ==================
///
/// The main distinction between `Array` and the other array types is
/// that it interoperates seamlessly and efficiently with Objective-C.
///
/// `Array<T>` is considered bridged to Objective-C iff `T` is bridged
/// to Objective-C.
///
/// When `T` is a `class` or `@objc` protocol type, `Array` may store
/// its elements in an `NSArray`.  Since any arbitrary subclass of
/// `NSArray` can become an `Array`, there are no guarantees about
/// representation or efficiency in this case (see also
/// `ContiguousArray`).  Since `NSArray` is immutable, it is just as
/// though the storage was shared by some copy: the first in any
/// sequence of mutating operations causes elements to be copied into
/// unique, contiguous storage which may cost `O(N)` time and space,
/// where `N` is the length of the array (or more, if the underlying
/// `NSArray` is has unusual performance characteristics).
///
/// Bridging to Objective-C
/// -----------------------
///
/// Any bridged `Array` can be implicitly converted to an `NSArray`.
/// When `T` is a `class` or `@objc` protocol, bridging takes O(1)
/// time and O(1) space.  Other `Array`\ s must be bridged
/// element-by-element, allocating a new object for each element, at a
/// cost of at least O(`count`) time and space.
///
/// Bridging from Objective-C
/// -------------------------
///
/// An `NSArray` can be implicitly or explicitly converted to any
/// bridged `Array<T>`.  This conversion calls `copyWithZone` on the
/// `NSArray`, to ensure it won't be modified, and stores the result
/// in the `Array`.  Type-checking, to ensure the `NSArray`\ 's
/// elements match or can be bridged to `T`, is deferred until the
/// first element access.
struct Array<T> : MutableCollectionType, Sliceable {

    /// The type of element stored by this `Array`
    typealias Element = T

    /// Always zero, which is the index of the first element when non-empty.
    var startIndex: Int {
        get {
            return 0;//TODO
        }
    }

    /// A "past-the-end" element index; the successor of the last valid
    /// subscript argument.
    var endIndex: Int {
        get {
            return 0;//TODO
        }
    }
    subscript (index: Int) -> T {
        return T(); //TODO
    }

    /// Return a *generator* over the elements.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<[T]> {
        return IndexingGenerator<[T]>();//TODO
    }

    /// A type that can represent a sub-range of an `Array` 
    typealias SubSlice = Slice<T>
    subscript (subRange: Range<Int>) -> Slice<T> {
        return Slice<T>();//TODO
    }

    /// Initialization from an existing buffer does not have "array.init"
    /// semantics because the caller may retain an alias to buffer.
    init(_ buffer: _ArrayBuffer<T>) {
        //TODO
    }
}

extension Array : ArrayLiteralConvertible {

    /// Create an instance containing `elements`.
    init(arrayLiteral elements: T...) {
        //TODO
    }
}



extension Array {

    /// Construct an empty Array
    init() {
        //TODO
    }

    /// Construct from an arbitrary sequence with elements of type `T`
    init<S : SequenceType where T == T>(_ s: S) {
        //TODO
    }

    /// Construct a Array of `count` elements, each initialized to
    /// `repeatedValue`.
    init(count: Int, repeatedValue: T) {
        //TODO
    }

    /// How many elements the Array stores
    var count: Int {
        get {
            return 0//TODO
        }
    }

    /// How many elements the `Array` can store without reallocation
    var capacity: Int {
        get {
            return 0//TODO
        }
    }

    /// `true` if and only if the `Array` is empty
    var isEmpty: Bool {
        get {
            return true//TODO
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

    /// Append newElement to the Array
    ///
    /// Complexity: amortized O(1) unless `self`'s storage is shared with another live array; O(`count`) if `self` does not wrap a bridged `NSArray`; otherwise the efficiency is unspecified.
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

    /// Remove an element from the end of the Array in O(1).
    /// Requires: count > 0
    mutating func removeLast() -> T {
        return T()//TODO
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
        return T()//TODO
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
    func join<S : SequenceType where S.Generator.Element == T>(elements: S) -> [T] {
        return []//TODO
    }

    /// Return the result of repeatedly calling `combine` with an
    /// accumulated value initialized to `initial` and each element of
    /// `self`, in turn, i.e. return
    /// `combine(combine(...combine(combine(initial, self[0]),
    /// self[1]),...self[count-2]), self[count-1])`.
    func reduce<U>(initial: U, combine: (U, T) -> U) -> U {
        return U()//TODO
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
    func sorted(isOrderedBefore: (T, T) -> Bool) -> [T] {
        return []//TODO
    }

    /// Return an `Array` containing the results of calling
    /// `transform(x)` on each element `x` of `self`
    func map<U>(transform: (T) -> U) -> [U] {
        return []//TODO
    }

    /// A Array containing the elements of `self` in reverse order
    func reverse() -> [T] {
        return []//TODO
    }

    /// Return an `Array` containing the elements `x` of `self` for which
    /// `includeElement(x)` is `true`
    func filter(includeElement: (T) -> Bool) -> [T] {
        return []//TODO
    }
}

extension Array : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}

extension Array : Printable, DebugPrintable {

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



extension Array {

    /// Call `body(p)`, where `p` is a pointer to the `Array`\ 's
    /// contiguous storage. If no such storage exists, it is first created.
    ///
    /// Often, the optimizer can eliminate bounds checks within an
    /// array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    func withUnsafeBufferPointer<R>(body: (UnsafeBufferPointer<T>) -> R) -> R {
        return R()//TODO
    }

    /// Call `body(p)`, where `p` is a pointer to the `Array`\ 's
    /// mutable contiguous storage. If no such storage exists, it is first created.
    ///
    /// Often, the optimizer can eliminate bounds- and uniqueness-checks
    /// within an array algorithm, but when that fails, invoking the
    /// same algorithm on `body`\ 's argument lets you trade safety for
    /// speed.
    mutating func withUnsafeMutableBufferPointer<R>(body: (inout UnsafeMutableBufferPointer<T>) -> R) -> R {
        return R()//TODO
    }
}

extension Array {

    /// This function "seeds" the ArrayLiteralConvertible protocol
    static func convertFromHeapArray(base: Builtin.RawPointer, owner: Builtin.NativeObject, count: Builtin.Word) -> [T] {
        return []//TODO
    }
}

extension Array {

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

extension Array {

    /// Construct from the given `_SwiftNSArrayRequiredOverridesType`.
    ///
    /// If `noCopy` is `true`, either `source` must be known to be immutable,
    /// or the resulting / `Array` must not survive across code that could mutate
    /// `source`.
    init(_fromCocoaArray source: _SwiftNSArrayRequiredOverridesType, noCopy: Bool = default) {
        //TODO
    }
}


/// Conforming types can be initialized with array literals
protocol ArrayLiteralConvertible {
    typealias Element

    /// Create an instance initialized with `elements`.
    init(arrayLiteral elements: Element...)
}

