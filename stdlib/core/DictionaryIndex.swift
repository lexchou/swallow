/// Used to access the key-value pairs in an instance of
/// `Dictionary<Key,Value>`.
///
/// Remember that Dictionary has two subscripting interfaces:
///
/// 1. Subscripting with a key, yielding an optional value::
///
///      v = d[k]!
///
/// 2. Subscripting with an index, yielding a key-value pair:
///
///      (k,v) = d[i]
struct DictionaryIndex<Key : Hashable, Value> : BidirectionalIndexType, Comparable {

    /// Identical to `self.dynamicType`
    typealias Index = DictionaryIndex<Key, Value>

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> DictionaryIndex<Key, Value> {
        //TODO
    }

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> DictionaryIndex<Key, Value> {
        //TODO
    }
}
