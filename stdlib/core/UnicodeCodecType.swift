/// A Unicode encoding scheme.
///
/// Consists of an underlying `CodeUnit` type and functions to
/// translate between sequences of these code units and Unicode
/// scalar values.
protocol UnicodeCodecType {

    /// A type that can hold code unit values for this
    /// encoding.
    typealias CodeUnit
    init()

    /// Start or continue decoding a UTF sequence.
    ///
    /// In order to decode a code unit sequence completely, this function should
    /// be called repeatedly until it returns `UnicodeDecodingResult.EmptyInput`.
    /// Checking that the generator was exhausted is not sufficient.  The decoder
    /// can have an internal buffer that is pre-filled with data from the input
    /// generator.
    ///
    /// Because of buffering, it is impossible to find the corresponing position
    /// in the generator for a given returned `UnicodeScalar` or an error.
    ///
    /// :param: `next`: a *generator* of code units to be decoded.
    mutating func decode<G : GeneratorType where `Self`.CodeUnit == CodeUnit>(inout next: G) -> UnicodeDecodingResult

    /// Encode a `UnicodeScalar` as a series of `CodeUnit`\ s by `put`\
    /// 'ing each `CodeUnit` to `output`.
    class func encode<S : SinkType where `Self`.CodeUnit == CodeUnit>(input: UnicodeScalar, inout output: S)
}
