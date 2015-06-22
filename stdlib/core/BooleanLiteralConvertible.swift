/// Conforming types can be initialized with the boolean literals
/// `true` and `false`.
protocol BooleanLiteralConvertible {
    typealias BooleanLiteralType

    /// Create an instance initialized to `value`.
    init(booleanLiteral value: BooleanLiteralType)
}


/// The default type for an otherwise-unconstrained boolean literal
typealias BooleanLiteralType = Bool
