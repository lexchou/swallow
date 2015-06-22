/// Conforming types can be initialized with floating point literals
protocol FloatLiteralConvertible {
    typealias FloatLiteralType

    /// Create an instance initialized to `value`.
    init(floatLiteral value: FloatLiteralType)
}
