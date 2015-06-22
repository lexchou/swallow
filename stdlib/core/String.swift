/// An arbitrary Unicode string value.
///
/// Unicode-Correct
/// ===============
///
/// Swift strings are designed to be Unicode-correct.  In particular,
/// the APIs make it easy to write code that works correctly, and does
/// not surprise end-users, regardless of where you venture in the
/// Unicode character space.  For example,
///
/// * The `==` operator checks for Unicode canonical equivalence,
///   so two different representations of the same string will always
///   compare equal.
///
/// * String elements are `Characters` (Unicode extended grapheme
///   clusters), a unit of text that is meaningful to most humans.
///
/// Locale-Insensitive
/// ==================
///
/// The fundamental operations on Swift strings are not sensitive to
/// locale settings.  That's because, for example, the validity of a
/// `Dictionary<String, T>` in a running program depends on a given
/// string comparison having a single, stable result.  Therefore,
/// Swift always uses the default, un-tailored Unicode algorithms
/// for basic string operations.
///
/// Importing `Foundation` endows swift strings with the full power of
/// the `NSString` API, which allows you to choose more complex
/// locale-sensitive operations explicitly.
///
/// Value Semantics
/// ===============
///
/// Each string variable, `let` binding, or stored property has an
/// independent value, so mutations to the string are not observable
/// through its copies::
///
///   var a = "foo"
///   var b = a
///   b[b.endIndex.predecessor()] = "x"
///   println("a=\(a), b=\(b)")     // a=foo, b=fox
///
/// Strings use Copy-on-Write so that their data is only copied
/// lazily, upon mutation, when more than one string instance is using
/// the same buffer.  Therefore, the first in any sequence of mutating
/// operations may cost `O(N)` time and space, where `N` is the length
/// of the string's (unspecified) underlying representation,.
///
/// Growth and Capacity
/// ===================
///
/// When a string's contiguous storage fills up, new storage must be
/// allocated and characters must be moved to the new storage.
/// `String` uses an exponential growth strategy that makes `append` a
/// constant time operation *when amortized over many invocations*.
///
/// Objective-C Bridge
/// ==================
///
/// `String` is bridged to Objective-C as `NSString`, and a `String`
/// that originated in Objective-C may store its characters in an
/// `NSString`.  Since any arbitrary subclass of `NSSString` can
/// become a `String`, there are no guarantees about representation or
/// efficiency in this case.  Since `NSString` is immutable, it is
/// just as though the storage was shared by some copy: the first in
/// any sequence of mutating operations causes elements to be copied
/// into unique, contiguous storage which may cost `O(N)` time and
/// space, where `N` is the length of the string representation (or
/// more, if the underlying `NSString` is has unusual performance
/// characteristics).
struct String {
    init() {
        //TODO
    }
}

extension String : CollectionType {

    /// A character position in a `String`
    struct Index : BidirectionalIndexType, Comparable, Reflectable {

        /// Returns the next consecutive value after `self`.
        ///
        /// Requires: the next value is representable.
        func successor() -> String.Index {
            return String.Index()//TODO
        }

        /// Returns the previous consecutive value before `self`.
        ///
        /// Requires: the previous value is representable.
        func predecessor() -> String.Index {
            return String.Index()//TODO
        }

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType {
            //TODO
        }
    }

    /// The position of the first `Character` if the `String` is
    /// non-empty; identical to `endIndex` otherwise.
    var startIndex: String.Index {
        get {
            return String.Index()//TODO
        }
    }

    /// The `String`\ 's "past the end" position.
    ///
    /// `endIndex` is not a valid argument to `subscript`, and is always
    /// reachable from `startIndex` by zero or more applications of
    /// `successor()`.
    var endIndex: String.Index {
        get {
            return String.Index()//TODO
        }
    }
    subscript (i: String.Index) -> Character {
        get {
            return Character()//TODO
        }
    }

    /// Return a *generator* over the `Characters` in this `String`.
    ///
    /// Complexity: O(1)
    func generate() -> IndexingGenerator<String> {
        return IndexingGenerator<String>()/TODO
    }
}

extension String {

    /// A collection of UTF-8 code units that encodes a `String` value.
    struct UTF8View : CollectionType, Reflectable {

        /// A position in a `String.UTF8View`
        struct Index : ForwardIndexType {

            /// Returns the next consecutive value after `self`.
            ///
            /// Requires: the next value is representable.
            func successor() -> String.UTF8View.Index {
                return String.UTF8View.Index()//TODO
            }
        }

        /// The position of the first code unit if the `String` is
        /// non-empty; identical to `endIndex` otherwise.
        var startIndex: String.UTF8View.Index {
            get {
                return String.UTF8View.Index()//TODO
            }
        }

        /// The "past the end" position.
        ///
        /// `endIndex` is not a valid argument to `subscript`, and is always
        /// reachable from `startIndex` by zero or more applications of
        /// `successor()`.
        var endIndex: String.UTF8View.Index {
            get {
                return String.UTF8View.Index()//TODO
            }
        }
        subscript (position: String.UTF8View.Index) -> CodeUnit {
            get {
                return CodeUnit()//TODO
            }
        }

        /// Return a *generator* over the code points that comprise this
        /// *sequence*.
        ///
        /// Complexity: O(1)
        func generate() -> IndexingGenerator<String.UTF8View> {
            //TODO
        }

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType {
            //TODO
        }
    }

    /// A UTF-8 encoding of `self`.
    var utf8: String.UTF8View {
        get {
            //TODO
        }
    }

    /// A contiguously-stored nul-terminated UTF-8 representation of
    /// `self`.
    ///
    /// To access the underlying memory, invoke
    /// `withUnsafeBufferPointer` on the `ContiguousArray`.
    var nulTerminatedUTF8: ContiguousArray<CodeUnit> {
        get {
            return ContiguousArray<CodeUnit>()//TODO
        }
    }
}

extension String {

    /// A collection of Unicode scalar values that
    /// encode a `String` .
    struct UnicodeScalarView : Sliceable, SequenceType, Reflectable {

        /// A position in a `String.UnicodeScalarView`
        struct Index : BidirectionalIndexType, Comparable {

            /// Returns the next consecutive value after `self`.
            ///
            /// Requires: the next value is representable.
            func successor() -> String.UnicodeScalarView.Index {
                //TODO
            }

            /// Returns the previous consecutive value before `self`.
            ///
            /// Requires: the previous value is representable.
            func predecessor() -> String.UnicodeScalarView.Index {
                //TODO
            }
        }

        /// The position of the first `UnicodeScalar` if the `String` is
        /// non-empty; identical to `endIndex` otherwise.
        var startIndex: String.UnicodeScalarView.Index {
            get {
                //TODO
            }
        }

        /// The "past the end" position.
        ///
        /// `endIndex` is not a valid argument to `subscript`, and is always
        /// reachable from `startIndex` by zero or more applications of
        /// `successor()`.
        var endIndex: String.UnicodeScalarView.Index {
            get {
                //TODO
            }
        }
        subscript (position: String.UnicodeScalarView.Index) -> UnicodeScalar {
            get {
                //TODO
            }
        }
        subscript (r: Range<String.UnicodeScalarView.Index>) -> String.UnicodeScalarView {
            get {
                //TODO
            }
        }

        /// A type whose instances can produce the elements of this
        /// sequence, in order.
        struct Generator : GeneratorType {

            /// Advance to the next element and return it, or `nil` if no next
            /// element exists.
            ///
            /// Requires: no preceding call to `self.next()` has returned
            /// `nil`.
            mutating func next() -> UnicodeScalar? {
                return nil//TODO
            }
        }

        /// Return a *generator* over the `UnicodeScalar`\ s that comprise
        /// this *sequence*.
        ///
        /// Complexity: O(1)
        func generate() -> String.UnicodeScalarView.Generator {
            return String.UnicodeScalarView.Generator()//TODO
        }

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType {
            //TODO
        }
    }
}

extension String {

    /// Creates a new `String` by copying the nul-terminated UTF-8 data
    /// referenced by a `CString`.
    ///
    /// Returns `nil` if the `CString` is `NULL` or if it contains ill-formed
    /// UTF-8 code unit sequences.
    static func fromCString(cs: UnsafePointer<CChar>) -> String? {
        return nil//TODO
    }

    /// Creates a new `String` by copying the nul-terminated UTF-8 data
    /// referenced by a `CString`.
    ///
    /// Returns `nil` if the `CString` is `NULL`.  If `CString` contains
    /// ill-formed UTF-8 code unit sequences, replaces them with replacement
    /// characters (U+FFFD).
    static func fromCStringRepairingIllFormedUTF8(cs: UnsafePointer<CChar>) -> (String?, hadError: Bool) {
        return (nil, false) //TODO
    }
}

extension String {

    /// Construct an instance containing just the given `Character`.
    init(_ c: Character) {
        //TODO
    }
}

extension String {

    /// Invoke `f` on the contents of this string, represented as
    /// a nul-terminated array of char, ensuring that the array's
    /// lifetime extends through the execution of `f`.
    func withCString<Result>(f: (UnsafePointer<Int8>) -> Result) -> Result {
        //TODO
    }
}

extension String : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }
}

extension String : OutputStreamType {
    mutating func write(other: String) {
        //TODO
    }
}

extension String : Streamable {

    /// Write a textual representation of `self` into `target`
    func writeTo<Target : OutputStreamType>(inout target: Target) {
        //TODO
    }
}



extension String {
    init(_builtinUnicodeScalarLiteral value: Builtin.Int32) {
        //TODO
    }
}

extension String : UnicodeScalarLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(unicodeScalarLiteral value: String) {
        //TODO
    }
}

extension String {
    init(_builtinExtendedGraphemeClusterLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1) {
        //TODO
    }
}

extension String : ExtendedGraphemeClusterLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(extendedGraphemeClusterLiteral value: String) {
        //TODO
    }
}

extension String {
    init(_builtinUTF16StringLiteral start: Builtin.RawPointer, numberOfCodeUnits: Builtin.Word) {
        //TODO
    }
}

extension String {
    init(_builtinStringLiteral start: Builtin.RawPointer, byteSize: Builtin.Word, isASCII: Builtin.Int1) {
        //TODO
    }
}

extension String : StringLiteralConvertible {

    /// Create an instance initialized to `value`.
    init(stringLiteral value: String) {
        //TODO
    }
}

extension String : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}



extension String : Equatable {
}

extension String : Comparable {
}

extension String {

    /// Append the elements of `other` to `self`.
    mutating func extend(other: String) {
        //TODO
    }

    /// Append `x` to `self`.
    ///
    /// Complexity: amortized O(1).
    mutating func append(x: UnicodeScalar) {
        //TODO
    }
}

extension String : Hashable {

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

extension String : StringInterpolationConvertible {

    /// Create an instance by concatenating the elements of `strings`
    static func convertFromStringInterpolation(strings: String...) -> String {
        return ""//TODO
    }

    /// Create an instance containing `expr`\ 's `print` representation
    static func convertFromStringInterpolationSegment<T>(expr: T) -> String {
        return ""//TODO
    }
}

extension String : Comparable {
}



extension String : Sliceable {
    subscript (subRange: Range<String.Index>) -> String {
        get {
            return ""//TODO
        }
    }
}

extension String : ExtensibleCollectionType {

    /// Reserve enough space to store `n` ASCII characters.
    ///
    /// Complexity: O(`n`)
    mutating func reserveCapacity(n: Int) {
        //TODO
    }

    /// Append `c` to `self`.
    ///
    /// Complexity: amortized O(1).
    mutating func append(c: Character) {
        //TODO
    }

    /// Append the elements of `newElements` to `self`.
    mutating func extend<S : SequenceType where Character == Character>(newElements: S) {
        //TODO
    }

    /// Create an instance containing `characters`.
    init<S : SequenceType where Character == Character>(_ characters: S) {
        //TODO
    }
}

extension String {

    /// Interpose `self` between every pair of consecutive `elements`,
    /// then concatenate the result.  For example::
    ///
    ///   "-|-".join(["foo", "bar", "baz"]) // "foo-|-bar-|-baz"
    func join<S : SequenceType where String == String>(elements: S) -> String {
        //TODO
    }
}

extension String : RangeReplaceableCollectionType {

    /// Replace the given `subRange` of elements with `newElements`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(subRange)`\ ) if `subRange.endIndex
    /// == self.endIndex` and `isEmpty(newElements)`\ , O(N) otherwise.
    mutating func replaceRange<C : CollectionType where Character == Character>(subRange: Range<String.Index>, with newElements: C) {
        //TODO
    }

    /// Insert `newElement` at index `i`.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func insert(newElement: Character, atIndex i: String.Index) {
        //TODO
    }

    /// Insert `newElements` at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self) + countElements(newElements)`\ ).
    mutating func splice<S : CollectionType where Character == Character>(newElements: S, atIndex i: String.Index) {
        //TODO
    }

    /// Remove and return the element at index `i`
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeAtIndex(i: String.Index) -> Character {
        //TODO
    }

    /// Remove the indicated `subRange` of characters
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// Complexity: O(\ `countElements(self)`\ ).
    mutating func removeRange(subRange: Range<String.Index>) {
        //TODO
    }

    /// Remove all characters.
    ///
    /// Invalidates all indices with respect to `self`.
    ///
    /// :param: `keepCapacity`, if `true`, prevents the release of
    ////   allocated storage, which can be a useful optimization
    ///    when `self` is going to be grown again.
    mutating func removeAll(keepCapacity: Bool = default) {
        //TODO
    }
}



extension String : StringInterpolationConvertible {
    static func convertFromStringInterpolationSegment(expr: String) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Character) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: UnicodeScalar) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Bool) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Float32) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Float64) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: UInt8) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Int8) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: UInt16) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Int16) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: UInt32) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Int32) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: UInt64) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Int64) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: UInt) -> String {
        return ""//TODO
    }
    static func convertFromStringInterpolationSegment(expr: Int) -> String {
        return ""//TODO
    }
}

extension String {

    /// Construct an instance that is the concatenation of `sz` copies
    /// of `repeatedValue`
    init(count sz: Int, repeatedValue c: Character) {
        //TODO
    }

    /// Construct an instance that is the concatenation of `sz` copies
    /// of `Character(repeatedValue)`
    init(count: Int, repeatedValue c: UnicodeScalar) {
        //TODO
    }

    /// `true` iff `self` contains no characters.
    var isEmpty: Bool {
        get {
            return false//TODO
        }
    }
}



extension String {

    /// Return `true` iff `self` begins with `prefix`
    func hasPrefix(prefix: String) -> Bool {
        return false//TODO
    }

    /// Return `true` iff `self` ends with `suffix`
    func hasSuffix(suffix: String) -> Bool {
        return false//TODO
    }
}

extension String {

    /// Create an instance representing `v` in base 10.
    init<T : _SignedIntegerType>(_ v: T) {
        //TODO
    }

    /// Create an instance representing `v` in base 10.
    init<T : _UnsignedIntegerType>(_ v: T) {
        //TODO
    }

    /// Create an instance representing `v` in the given `radix` (base).
    ///
    /// Numerals greater than 9 are represented as roman letters,
    /// starting with `a` if `uppercase` is `false` or `A` otherwise.
    init<T : _SignedIntegerType>(_ v: T, radix: Int, uppercase: Bool = default) {
        //TODO
    }

    /// Create an instance representing `v` in the given `radix` (base).
    ///
    /// Numerals greater than 9 are represented as roman letters,
    /// starting with `a` if `uppercase` is `false` or `A` otherwise.
    init<T : _UnsignedIntegerType>(_ v: T, radix: Int, uppercase: Bool = default) {
        //TODO
    }
}

extension String {

    /// If the string represents an integer that fits into an Int, returns
    /// the corresponding integer.  This accepts strings that match the regular
    /// expression "[-+]?[0-9]+" only.
    func toInt() -> Int? {
        return nil//TODO
    }
}



extension String {

    /// A collection of UTF-16 code units that encodes a `String` value.
    struct UTF16View : Sliceable, Reflectable {

        /// The position of the first code unit if the `String` is
        /// non-empty; identical to `endIndex` otherwise.
        var startIndex: Int {
            get {
                return 0//TODO
            }
        }

        /// The "past the end" position.
        ///
        /// `endIndex` is not a valid argument to `subscript`, and is always
        /// reachable from `startIndex` by zero or more applications of
        /// `successor()`.
        var endIndex: Int {
            get {
                return 0//TODO
            }
        }

        /// A type whose instances can produce the elements of this
        /// sequence, in order.
        struct Generator : GeneratorType {

            /// Advance to the next element and return it, or `nil` if no next
            /// element exists.
            ///
            /// Requires: no preceding call to `self.next()` has returned
            /// `nil`.
            mutating func next() -> UnicodeScalar? {
                return nil//TODO
            }
        }

        /// Return a *generator* over the code points that comprise this
        /// *sequence*.
        ///
        /// Complexity: O(1)
        func generate() -> Generator {
            return Generator()//TODO
        }
        subscript (position: Int) -> UInt16 {
            get {
                return 0//TODO
            }
        }
        subscript (subRange: Range<Int>) -> String.UTF16View {
            get {
                return 0//TODO
            }
        }

        /// Returns a mirror that reflects `self`.
        func getMirror() -> MirrorType {
            return//TODO
        }
    }

    /// A UTF-16 encoding of `self`.
    var utf16: String.UTF16View {
        get {
            return String.UTF16View()//TODO
        }
    }
}







extension String {

    /// Construct an instance given a collection of Unicode scalar
    /// values.
    init(_ view: String.UnicodeScalarView) {
        //TODO
    }

    /// The value of `self` as a collection of Unicode scalar values.
    var unicodeScalars: String.UnicodeScalarView
}
