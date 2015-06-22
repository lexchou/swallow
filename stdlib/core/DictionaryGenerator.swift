/// A generator over the key-value pairs of a `Dictionary<Key, Value>`
struct DictionaryGenerator<Key : Hashable, Value> : GeneratorType {

    /// Advance to the next element and return it, or `nil` if no next
    /// element exists.
    ///
    /// Requires: no preceding call to `self.next()` has returned `nil`.
    mutating func next() -> (Key, Value)? {
        return nil//TODO
    }
}
