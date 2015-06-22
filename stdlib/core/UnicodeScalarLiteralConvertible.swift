/// Conforming types can be initialized with string literals
/// containing a single Unicode scalar value.
protocol UnicodeScalarLiteralConvertible {
    typealias UnicodeScalarLiteralType

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: UnicodeScalarLiteralType)
}
/// The default type for an otherwise-unconstrained unicode scalar literal
typealias UnicodeScalarType = String
