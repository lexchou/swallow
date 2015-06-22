
enum Optional<T> : Reflectable, NilLiteralConvertible {
    case None
    case Some(T)

    /// Construct a `nil` instance.
    init() {
        //TODO
    }

    /// Construct a non-\ `nil` instance that stores `some`.
    init(_ some: T) {
        //TODO
    }

    /// If `self == nil`, returns `nil`.  Otherwise, returns `f(self!)`.
    func map<U>(f: (T) -> U) -> U? {
        return nil//TODO
    }

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        //TODO
    }

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ()) {
        //TODO
    }
}

extension Optional : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}
