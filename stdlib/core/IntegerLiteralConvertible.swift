/// Conforming types can be initialized with integer literals
protocol IntegerLiteralConvertible {
    typealias IntegerLiteralType

    /// Create an instance initialized to `value`.
    init(integerLiteral value: IntegerLiteralType)
}

/// The default type for an otherwise-unconstrained integer literal
typealias IntegerLiteralType = Int
