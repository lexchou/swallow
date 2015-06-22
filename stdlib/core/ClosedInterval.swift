struct ClosedInterval<T : Comparable> : IntervalType, Equatable, Printable, DebugPrintable, Reflectable {

    /// The type of the `Interval`\ 's endpoints
    typealias Bound = T

    /// Construct a copy of `x`
    init(_ x: ClosedInterval<T>) {
        //TODO
    }

    /// Construct an interval with the given bounds.  Requires: `start`
    /// <= `end`.
    init(_ start: T, _ end: T) {
        //TODO
    }

    /// The `Interval`\ 's lower bound. Invariant: `start` <= `end`
    var start: T {
        get {
            return 0//TODO
        }
    }

    /// The `Interval`\ 's upper bound. Invariant: `start` <= `end`
    var end: T {
        get {
            return 0//TODO
        }
    }

    /// A textual representation of `self`.
    var description: String {
        get {
            return ""//TODO
        }
    }

    /// A textual representation of `self`, suitable for debugging.
    var debugDescription: String {
        get {
            return ""//TODO
        }
    }

    /// Returns `true` iff the `Interval` contains `x`
    func contains(x: T) -> Bool {
        get {
            return false//TODO
        }
    }

    /// Return `intervalToClamp` clamped to `self`.  The bounds of the
    /// result, even if it is empty, are always limited to the bounds of
    /// `self`
    func clamp(intervalToClamp: ClosedInterval<T>) -> ClosedInterval<T> {
        return self//TODO
    }

    /// Returns a mirror that reflects `self`.
    func getMirror() -> MirrorType {
        return ClosedInterval<T>.Type//TODO
    }
}

extension ClosedInterval {

    /// `true` iff the `Interval` is empty.  In the case of
    /// `ClosedInterval`, always returns `false`
    var isEmpty: Bool {
        get {
            return false//TODO
        }
    }
}
