/// An optional type that allows implicit member access (via compiler
/// magic).
///
/// The compiler has special knowledge of the existence of
/// ImplicitlyUnwrappedOptional<T>, but always interacts with it using the
/// library intrinsics below.
enum ImplicitlyUnwrappedOptional<T> : Reflectable, NilLiteralConvertible {
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

    /// Construct an instance from an explicitly unwrapped optional
    /// (`T?`).
    init(_ v: T?) {
        //TODO
    }

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ()) {
        //TODO
    }

    /// If `self == nil`, returns `nil`.  Otherwise, returns `f(self!)`.
    func map<U>(f: (T) -> U) -> U! {
        return nil //TODO
    }

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return ImplicitlyUnwrappedOptional<T>.Type//TODO
    }
}

extension ImplicitlyUnwrappedOptional : Printable {

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }
}

extension ImplicitlyUnwrappedOptional : _ObjectiveCBridgeable {
}
