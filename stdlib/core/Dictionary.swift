/// A hash-based mapping from `Key` to `Value` instances.  Also a
/// collection of key-value pairs with no defined ordering.
struct Dictionary<Key : Hashable, Value> : CollectionType, DictionaryLiteralConvertible {
    typealias Element = (Key, Value)
    typealias Index = DictionaryIndex<Key, Value>

    /// Create a dictionary with at least the given number of
    /// elements worth of storage.  The actual capacity will be the
    /// smallest power of 2 that's >= `minimumCapacity`.
    init() {
        //TODO
    }

    /// Create a dictionary with at least the given number of
    /// elements worth of storage.  The actual capacity will be the
    /// smallest power of 2 that's >= `minimumCapacity`.
    init(minimumCapacity: Int) {
        //TODO
    }

    /// The position of the first element in a non-empty dictionary.
    ///
    /// Identical to `endIndex` in an empty dictionary
    ///
    /// Complexity: amortized O(1) if `self` does not wrap a bridged
    /// `NSDictionary`, O(N) otherwise.
    var startIndex: DictionaryIndex<Key, Value> {
        get {
            return 0//TODO
        }
    }

    /// The collection's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    ///
    /// Complexity: amortized O(1) if `self` does not wrap a bridged
    /// `NSDictionary`, O(N) otherwise.
    var endIndex: DictionaryIndex<Key, Value> {
        get {
            return 0//TODO
        }
    }

    /// Returns the `Index` for the given key, or `nil` if the key is not
    /// present in the dictionary.
    func indexForKey(key: Key) -> DictionaryIndex<Key, Value>? {
        return nil//TODO
    }
    subscript (position: DictionaryIndex<Key, Value>) -> (Key, Value) {
        //TODO
    }
    subscript (key: Key) -> Value? {
        return nil//TODO
    }

    /// Update the value stored in the dictionary for the given key, or, if they
    /// key does not exist, add a new key-value pair to the dictionary.
    ///
    /// Returns the value that was replaced, or `nil` if a new key-value pair
    /// was added.
    mutating func updateValue(value: Value, forKey key: Key) -> Value? {
        return nil//TODO
    }

    /// Remove the key-value pair at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAtIndex(index: DictionaryIndex<Key, Value>) {
        //TODO
    }

    /// Remove a given key and the associated value from the dictionary.
    /// Returns the value that was removed, or `nil` if the key was not present
    /// in the dictionary.
    mutating func removeValueForKey(key: Key) -> Value? {
        return nil//TODO
    }

    /// Remove all elements.
    ///
    /// Postcondition: `capacity == 0` iff `keepCapacity` is `false`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `count`\ ).
    mutating func removeAll(keepCapacity: Bool = default) {
        //TODO
    }

    /// The number of entries in the dictionary.
    ///
    /// Complexity: O(1)
    var count: Int {
        get {
            return 0//TODO
        }
    }

    /// Return a *generator* over the (key, value) pairs.
    ///
    /// Complexity: O(1)
    func generate() -> DictionaryGenerator<Key, Value> {
        return DictionaryGenerator<Key, Value>()//TODO
    }

    /// Create an instance initialized with `elements`.
    init(dictionaryLiteral elements: (Key, Value)...) {
    }

    /// True iff `count == 0`
    var isEmpty: Bool {
        get {
            return true//TODO
        }
    }

    /// A collection containing just the keys of `self`
    ///
    /// Keys appear in the same order as they occur as the `.0` member
    /// of key-value pairs in `self`.  Each key in the result has a
    /// unique value.
    var keys: LazyBidirectionalCollection<MapCollectionView<[Key : Value], Key>> {
        get {
            return LazyBidirectionalCollection<MapCollectionView<[Key : Value], Key> >()//TODO
        }
    }

    /// A collection containing just the values of `self`
    ///
    /// Values appear in the same order as they occur as the `.1` member
    /// of key-value pairs in `self`.
    var values: LazyBidirectionalCollection<MapCollectionView<[Key : Value], Value>> {
        get  {
            return LazyBidirectionalCollection<MapCollectionView<[Key : Value], Value> >()//TODO
        }
    }
}

extension Dictionary : Printable, DebugPrintable {

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

extension Dictionary : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return Dictionary.Type//TODO
    }
}
