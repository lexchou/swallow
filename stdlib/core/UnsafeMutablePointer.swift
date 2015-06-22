/// A pointer to an object of type `T`.  This type provides no automated
/// memory management, and therefore the user must take care to allocate
/// and free memory appropriately.
///
/// The pointer can be in one of the following states:
///
/// - memory is not allocated (for example, pointer is null, or memory has
/// been deallocated previously);
///
/// - memory is allocated, but value has not been initialized;
///
/// - memory is allocated and value is initialized.
struct UnsafeMutablePointer<T> : RandomAccessIndexType, Hashable, NilLiteralConvertible {

    /// Construct a null pointer.
    init() {
		//TODO
	}

    /// Construct an `UnsafeMutablePointer` from a builtin raw pointer.
    init(_ value: Builtin.RawPointer) {
		//TODO
	}

    /// Convert from an opaque C pointer to a typed pointer
    ///
    /// This is a fundamentally unsafe conversion.
    init(_ other: COpaquePointer) {
		//TODO
	}

    /// Construct an `UnsafeMutablePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: Word) {
		//TODO
	}

    /// Construct an `UnsafeMutablePointer` from a given address in memory.
    ///
    /// This is a fundamentally unsafe conversion.
    init(bitPattern: UWord) {
		//TODO
	}

    /// Convert from an UnsafeMutablePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafeMutablePointer<U>) {
		//TODO
	}

    /// Convert from a UnsafePointer of a different type.
    ///
    /// This is a fundamentally unsafe conversion.
    init<U>(_ from: UnsafePointer<U>) {
		//TODO
	}

    /// Create an instance initialized with `nil`.
    init(nilLiteral: ()) {
		//TODO
	}

    /// Return a `nil` instance.
    static func null() -> UnsafeMutablePointer<T> {
		return UnsafeMutablePointer<T>()//TODO
	}

    /// Allocate memory for `num` objects of type `T`.
    ///
    /// Postcondition: the memory is allocated, but not initialized.
    static func alloc(num: Int) -> UnsafeMutablePointer<T> {
		return UnsafeMutablePointer<T>()//TODO
	}

    /// Deallocate `num` objects.
    ///
    /// :param: num number of objects to deallocate.  Should match exactly
    /// the value that was passed to `alloc()` (partial deallocations are not
    /// possible).
    ///
    /// Precondition: the memory is not initialized.
    ///
    /// Postcondition: the memory has been deallocated.
    func dealloc(num: Int) {
		//TODO
	}

    /// Access the underlying raw memory, getting and
    /// setting values.
    var memory: T { 
		get {
			return T()//TODO
		}
		nonmutating set {
		}
	}

    /// Initialize the value the pointer points to, to construct
    /// an object where there was no object previously stored.
    ///
    /// Precondition: the memory is not initialized.
    ///
    /// Postcondition: the memory is initalized; the value should eventually
    /// be destroyed or moved from to avoid leaks.
    func initialize(newvalue: T) {
		//TODO
	}

    /// Retrieve the value the pointer points to, moving it away
    /// from the location referenced in memory.
    ///
    /// Equivalent to reading `memory` property and calling `destroy()`,
    /// but more efficient.
    ///
    /// Precondition: the memory is initialized.
    ///
    /// Postcondition: the value has been destroyed and the memory must
    /// be initialized before being used again.
    func move() -> T {
		//TODO
	}

    /// Move count values beginning at source into uninitialized memory,
    /// transforming the source values into raw memory, proceeding from
    /// the last value to the first.  Use this for copying ranges into
    /// later memory that may overlap with the source range.
    ///
    /// Requires: either `source` precedes `self` or follows `self + count`.
    func moveInitializeBackwardFrom(source: UnsafeMutablePointer<T>, count: Int) {
		//TODO
	}

    /// Assign from count values beginning at source into initialized
    /// memory, transforming the source values into raw memory.
    func moveAssignFrom(source: UnsafeMutablePointer<T>, count: Int) {
		//TODO
	}

    /// Assign from `count` values beginning at source into initialized
    /// memory, proceeding from the first element to the last.
    func assignFrom(source: UnsafeMutablePointer<T>, count: Int) {
		//TODO
	}

    /// Assign from `count` values beginning at `source` into
    /// initialized memory, proceeding from the last value to the first.
    /// Use this for assigning ranges into later memory that may overlap
    /// with the source range.
    ///
    /// Requires: either `source` precedes `self` or follows `self + count`.
    func assignBackwardFrom(source: UnsafeMutablePointer<T>, count: Int) {
		//TODO
	}

    /// Move count values beginning at source into raw memory,
    /// transforming the source values into raw memory.
    func moveInitializeFrom(source: UnsafeMutablePointer<T>, count: Int) {
		//TODO
	}

    /// Copy count values beginning at source into raw memory.
    ///
    /// Precondition: the memory is not initialized.
    func initializeFrom(source: UnsafeMutablePointer<T>, count: Int) {
		//TODO
	}

    /// Copy the elements of `C` into raw memory.
    ///
    /// Precondition: the memory is not initialized.
    func initializeFrom<C : CollectionType where T == T>(source: C) {
		//TODO
	}

    /// Destroy the object the pointer points to.
    ///
    /// Precondition: the memory is initialized.
    ///
    /// Postcondition: the value has been destroyed and the memory must
    /// be initialized before being used again.
    func destroy() {
		//TODO
	}

    /// Destroy the `count` objects the pointer points to.
    /// Precondition: the memory is initialized.
    ///
    /// Postcondition: the value has been destroyed and the memory must
    /// be initialized before being used again.
    func destroy(count: Int) {
		//TODO
	}
    subscript (i: Int) -> T { 
		get {
			return T()//TODO
		}
		nonmutating set {
		}
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

    /// Returns the next consecutive value after `self`.
    ///
    /// Requires: the next value is representable.
    func successor() -> UnsafeMutablePointer<T> {
		return UnsafeMutablePointer<T>()//TODO
	}

    /// Returns the previous consecutive value before `self`.
    ///
    /// Requires: the previous value is representable.
    func predecessor() -> UnsafeMutablePointer<T> {
		return UnsafeMutablePointer<T>()//TODO
	}

    /// Return the minimum number of applications of `successor` or
    /// `predecessor` required to reach `other` from `self`.
    ///
    /// Complexity: O(1).
    func distanceTo(x: UnsafeMutablePointer<T>) -> Int {
		return 0//TODO
	}

    /// Return `self` offset by `n` steps.
    ///
    /// :returns: If `n > 0`, the result of applying `successor` to
    /// `self` `n` times.  If `n < 0`, the result of applying
    /// `predecessor` to `self` `-n` times. Otherwise, `self`.
    ///
    /// Complexity: O(1)
    func advancedBy(n: Int) -> UnsafeMutablePointer<T> {
		return UnsafeMutablePointer<T>()//TODO
	}
}

extension UnsafeMutablePointer : DebugPrintable {

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String { 
		get {
			return ""//TODO
		}
	}
}

extension UnsafeMutablePointer : Reflectable {

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
		//TODO
	}
}

extension UnsafeMutablePointer : SinkType {
    mutating func put(x: T) {
		//TODO
	}
}

