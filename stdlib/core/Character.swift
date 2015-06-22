/// `Character` represents some Unicode grapheme cluster as
/// defined by a canonical, localized, or otherwise tailored
/// segmentation algorithm.
enum Character : ExtendedGraphemeClusterLiteralConvertible, Equatable, Hashable, Comparable {
    case LargeRepresentation(OnHeap<String>)
    case SmallRepresentation

    /// Construct a `Character` containing just the given `scalar`.
    init(_ scalar: UnicodeScalar) {
        //TODO
    }
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: Character) {
        //TODO
    }
    init(_builtinExtendedGraphemeClusterLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1) {
        //TODO
    }

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: Character) {
        //TODO
    }

    /// Create an instance from a single-character `String`.
    ///
    /// Requires: `s` contains exactly one extended grapheme cluster.
    init(_ s: String) {
        //TODO
    }

    /// The hash value.
    ///
    /// **Axiom:** `x == y` implies `x.hashValue == y.hashValue`
    ///
    /// **Note:** the hash value is not guaranteed to be stable across
    /// different invocations of the same program.  Do not persist the
    /// hash value across program runs.
    var hashValue: Int {
        get {
            return 0//TODO
        }
    }
}

extension Character : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return Character.Type;//TODO
    }
}

extension Character : Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target) {
        //TODO
    }
}
