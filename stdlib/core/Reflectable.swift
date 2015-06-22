/// Customizes the result of `reflect(x)`, where `x` is a conforming
/// type.
protocol Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType
}
