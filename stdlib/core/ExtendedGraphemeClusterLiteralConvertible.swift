/// Conforming types can be initialized with string literals
/// containing a single Unicode extended grapheme cluster.
protocol ExtendedGraphemeClusterLiteralConvertible : UnicodeScalarLiteralConvertible {
    typealias ExtendedGraphemeClusterLiteralType

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: ExtendedGraphemeClusterLiteralType)
}


/// The default type for an otherwise-unconstrained unicode extended
/// grapheme cluster literal
typealias ExtendedGraphemeClusterType = String
