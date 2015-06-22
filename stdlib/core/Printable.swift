/// A type with a customized textual representation.
///
/// This textual representation is used when objects are written to an
/// *output stream*, for example, by `print` and `println`.
protocol Printable {

    /// A textual representation of `self`.
    var description: String { get }
}
