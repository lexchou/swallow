/// Conforming types can be initialized with arbitrary string literals
protocol StringLiteralConvertible : ExtendedGraphemeClusterLiteralConvertible {
    typealias StringLiteralType

    /// Create an instance initialized to `value`.
    init(stringLiteral value: StringLiteralType)
}


/// The default type for an otherwise-unconstrained string literal
typealias StringLiteralType = String
