/// Conforming types can be initialized with `nil`.
protocol NilLiteralConvertible {

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ())
}
