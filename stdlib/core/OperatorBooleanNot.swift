

prefix func !(a: Bool) -> Bool {
    return false//TODO
}


/// Return the result of inverting `a`\ 's logic value
prefix func !<T : BooleanType>(a: T) -> Bool {
    return false//TODO
}

func !=(lhs: Int, rhs: Int) -> Bool {
    return false//TODO
}

func !=<T : Equatable>(lhs: T?, rhs: T?) -> Bool {
    return false//TODO
}

func !=<T>(lhs: T?, rhs: _OptionalNilComparisonType) -> Bool {
    return false//TODO
}

func !=(lhs: UInt, rhs: UInt) -> Bool {
    return false//TODO
}

func !=(lhs: Int64, rhs: Int64) -> Bool {
    return false//TODO
}

func !=(lhs: UInt64, rhs: UInt64) -> Bool {
    return false//TODO
}

func !=(lhs: Int32, rhs: Int32) -> Bool {
    return false//TODO
}

func !=(lhs: UInt32, rhs: UInt32) -> Bool {
    return false//TODO
}

func !=(lhs: Int16, rhs: Int16) -> Bool {
    return false//TODO
}

func !=(lhs: UInt16, rhs: UInt16) -> Bool {
    return false//TODO
}

func !=(lhs: Int8, rhs: Int8) -> Bool {
    return false//TODO
}

func !=(lhs: UInt8, rhs: UInt8) -> Bool {
    return false//TODO
}

func !=<T>(lhs: _OptionalNilComparisonType, rhs: T?) -> Bool {
    return false//TODO
}

func !=<T : Equatable>(lhs: T, rhs: T) -> Bool {
    return false//TODO
}


/// Returns true if the arrays do not contain the same elements.
func !=<T : Equatable>(lhs: ContiguousArray<T>, rhs: ContiguousArray<T>) -> Bool {
    return false//TODO
}


/// Returns true if the arrays do not contain the same elements.
func !=<T : Equatable>(lhs: Slice<T>, rhs: Slice<T>) -> Bool {
    return false//TODO
}


/// Returns true if the arrays do not contain the same elements.
func !=<T : Equatable>(lhs: [T], rhs: [T]) -> Bool {
    return false//TODO
}

func !=<Key : Equatable, Value : Equatable>(lhs: [Key : Value], rhs: [Key : Value]) -> Bool {
    return false//TODO
}

func !==(lhs: AnyObject?, rhs: AnyObject?) -> Bool {
    return false//TODO
}
