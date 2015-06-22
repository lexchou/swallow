/// A type with a customized textual representation for debugging
/// purposes.
///
/// This textual representation is used when objects are written to an
/// *output stream* by `debugPrint` and `debugPrintln`, and is
/// typically more verbose than the text provided by a `Printable`\ 's
/// `description` property.
protocol DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { get }
}
