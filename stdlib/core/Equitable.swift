/// Instances of conforming types can be compared for value equality
/// using operators `==` and `!=`.
///
/// When adopting `Equatable`, only the `==` operator is required to be
/// implemented.  The standard library provides an implementation for `!=`.
protocol Equatable {

    /// Return true if `lhs` is equal to `rhs`.
    ///
    /// **Equality implies substitutability**.  When `x == y`, `x` and
    /// `y` are interchangeable in any code that only depends on their
    /// values.
    ///
    /// Class instance identity as distinguished by triple-equals `===`
    /// is notably not part of an instance's value.  Exposing other
    /// non-value aspects of `Equatable` types is discouraged, and any
    /// that *are* exposed should be explicitly pointed out in
    /// documentation.
    /// 
    /// **Equality is an equivalence relation**
    ///
    /// - `x == x` is `true`
    /// - `x == y` implies `y == x`
    /// - `x == y` and `y == z` implies `x == z`
    ///
    /// **Inequality is the inverse of equality**, i.e. `!(x == y)` iff
    /// `x != y`
    func ==(lhs: Self, rhs: Self) -> Bool
}
