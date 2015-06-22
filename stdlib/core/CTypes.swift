/// The C '_Bool' and C++ 'bool' type.
typealias CBool = Bool


/// The C 'char' type.
///
/// This will be the same as either `CSignedChar` (in the common
/// case) or `CUnsignedChar`, depending on the platform.
typealias CChar = Int8


/// The C++11 'char16_t' type, which has UTF-16 encoding.
typealias CChar16 = UInt16


/// The C++11 'char32_t' type, which has UTF-32 encoding.
typealias CChar32 = UnicodeScalar


/// The C 'double' type.
typealias CDouble = Double


/// The C 'float' type.
typealias CFloat = Float


/// The family of C function pointer types.
///
/// In imported APIs, `T` is a Swift function type such as
/// `(Int)->String`.
/// 
/// Though not directly useful in Swift, `CFunctionPointer<T>` can be
/// used to safely pass a C function pointer, received from one C or
/// Objective-C API, to another C or Objective-C API.
struct CFunctionPointer<T> : Equatable, Hashable, NilLiteralConvertible {

    /// Create a `nil` instance.
    init() {
        //TODO
    }

    /// Reinterpret the bits of `value` as `CFunctionPointer<T>`.
    ///
    /// .. Warning:: This is a fundamentally unsafe operation, equivalent to
    ///      `unsafeBitCast(value, CFunctionPointer<T>.self)`
    init(_ value: COpaquePointer) {
        //TODO
    }

    /// Returns `nil`
    static func null() -> CFunctionPointer<T> {
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

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ()) {
        //TODO
    }
}

extension CFunctionPointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}


/// The C 'int' type.
typealias CInt = Int32


/// The C 'long' type.
typealias CLong = Int


/// The C 'long long' type.
typealias CLongLong = Int64


/// A wrapper around an opaque C pointer.
///
/// Opaque pointers are used to represent C pointers to types that
/// cannot be represented in Swift, such as incomplete struct types.
struct COpaquePointer : Equatable, Hashable, NilLiteralConvertible {

    /// Construct a `nil` instance.
    init() {
        //TODO
    }

    /// Construct a `COpaquePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: Word) {
        //TODO
    }

    /// Construct a `COpaquePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: UWord) {
        //TODO
    }

    /// Convert a typed `UnsafePointer` to an opaque C pointer.
    init<T>(_ value: UnsafePointer<T>) {
        //TODO
    }

    /// Convert a typed `UnsafeMutablePointer` to an opaque C pointer.
    init<T>(_ value: UnsafeMutablePointer<T>) {
        //TODO
    }

    /// Returns `nil`
    static func null() -> COpaquePointer {
        return COpaquePointer() //TODO
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

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ()) {
        //TODO
    }
}

extension COpaquePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }
}

extension COpaquePointer {

    /// Reinterpret the bits of `value` as `COpaquePointer`.
    ///
    /// .. Warning:: This is a fundamentally unsafe operation, equivalent to
    ///      `unsafeBitCast(value, COpaquePointer.self)`
    init<T>(_ value: CFunctionPointer<T>) {
        //TODO
    }
}

extension COpaquePointer : CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word] {
        return []//TODO
    }
}


/// The C 'short' type.
typealias CShort = Int16


/// The C 'signed char' type.
typealias CSignedChar = Int8


/// The C 'unsigned char' type.
typealias CUnsignedChar = UInt8


/// The C 'unsigned int' type.
typealias CUnsignedInt = UInt32


/// The C 'unsigned long' type.
typealias CUnsignedLong = UInt


/// The C 'unsigned long long' type.
typealias CUnsignedLongLong = UInt64


/// The C 'unsigned short' type.
typealias CUnsignedShort = UInt16


/// The corresponding Swift type to `va_list` in imported C APIs.
struct CVaListPointer {
}

extension CVaListPointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""
        }
    }
}


/// Instances of conforming types can be encoded, and appropriately
/// passed, as elements of a C `va_list`.
///
/// This protocol is useful in presenting C "varargs" APIs natively in
/// Swift.  It only works for APIs that have a `va_list` variant, so
/// for example, it isn't much use if all you have is::
///
///   int f(int n, ...)
///
/// Given a version like this, though, ::
///
///   int f(int, va_list arguments)
///
/// you can write::
///
///   func swiftF(x: Int, arguments: CVarArgType...) -> Int {
///     return withVaList(arguments) { f(x, $0) }
///   }
protocol CVarArgType {

    /// Transform `self` into a series of machine words that can be
    /// appropriately interpreted by C varargs
    func encode() -> [Word]
}


/// The C++ 'wchar_t' type.
typealias CWideChar = UnicodeScalar


/// Access to the raw argc value from C.
//TODO: uncomment it: var C_ARGC: CInt


/// Access to the raw argv value from C. Accessing the argument vector
/// through this pointer is unsafe.
//TODO: uncomment it: var C_ARGV: UnsafeMutablePointer<UnsafeMutablePointer<Int8>>
