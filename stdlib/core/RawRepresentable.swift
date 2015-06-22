/// A type that can be converted to an associated "raw" type, then
/// converted back to produce an instance equivalent to the original.
protocol RawRepresentable {

    /// The "raw" type that can be used to represent all values of `Self`.
    ///
    /// Every distinct value of `self` has a corresponding unique
    /// value of `RawValue`, but `RawValue` may have representations
    /// that do not correspond to an value of `Self`.
    typealias RawValue

    /// Convert from a value of `RawValue`, yielding `nil` iff
    /// `rawValue` does not correspond to a value of `Self`.
    init?(rawValue: RawValue)

    /// The corresponding value of the "raw" type.
    ///
    /// `Self(rawValue: self.rawValue)!` is equivalent to `self`.
    var rawValue: RawValue { get }
}
