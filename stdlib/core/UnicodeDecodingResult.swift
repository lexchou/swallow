/// The result of one Unicode decoding step
///
/// A unicode scalar value, an indication that no more unicode scalars
/// are available, or an indication of a decoding error.
enum UnicodeDecodingResult {
    case Result(UnicodeScalar)
    case EmptyInput
    case Error

    /// Return true if `self` indicates no more unicode scalars are
    /// available.
    func isEmptyInput() -> Bool {
		return false//TODO
	}
}
