/// Conforming types can be initialized with dictionary literals
protocol DictionaryLiteralConvertible {
    typealias Key
    typealias Value

    /// Create an instance initialized with `elements`.
    init(dictionaryLiteral elements: (Key, Value)...)
}
