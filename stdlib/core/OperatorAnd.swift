
func &(lhs: UInt32, rhs: UInt32) -> UInt32 {
    return lhs//TODO
}

func &(lhs: UInt16, rhs: UInt16) -> UInt16 {
    return lhs//TODO
}

func &(lhs: Int16, rhs: Int16) -> Int16 {
    return lhs//TODO
}

func &(lhs: Int32, rhs: Int32) -> Int32 {
    return lhs//TODO
}

func &(lhs: UInt64, rhs: UInt64) -> UInt64 {
    return lhs//TODO
}

func &(lhs: Int64, rhs: Int64) -> Int64 {
    return lhs//TODO
}

func &(lhs: UInt, rhs: UInt) -> UInt {
    return lhs//TODO
}

func &(lhs: Int, rhs: Int) -> Int {
    return lhs//TODO
}

func &(lhs: Bool, rhs: Bool) -> Bool {
    return lhs//TODO
}

func &<T : _RawOptionSetType>(a: T, b: T) -> T {
    return lhs//TODO
}

func &(lhs: UInt8, rhs: UInt8) -> UInt8 {
    return lhs//TODO
}

func &(lhs: Int8, rhs: Int8) -> Int8 {
    return lhs//TODO
}


func &&<T : BooleanType>(lhs: T, rhs: @autoclosure () -> Bool) -> Bool {
    return lhs//TODO
}

/// If `lhs` is `false`, return it.  Otherwise, evaluate `rhs` and
/// return its `boolValue`.
@inline(__always) func &&<T : BooleanType, U : BooleanType>(lhs: T, rhs: @autoclosure () -> U) -> Bool {
    return lhs//TODO
}


func &=(inout lhs: Bool, rhs: Bool) {
    return lhs//TODO
}

func &=(inout lhs: UInt8, rhs: UInt8) {
    return lhs//TODO
}

func &=(inout lhs: Int8, rhs: Int8) {
    return lhs//TODO
}

func &=(inout lhs: UInt16, rhs: UInt16) {
    return lhs//TODO
}

func &=(inout lhs: Int16, rhs: Int16) {
    return lhs//TODO
}

func &=(inout lhs: UInt32, rhs: UInt32) {
    return lhs//TODO
}

func &=(inout lhs: Int32, rhs: Int32) {
    return lhs//TODO
}

func &=(inout lhs: UInt64, rhs: UInt64) {
    return lhs//TODO
}

func &=<T : BitwiseOperationsType>(inout lhs: T, rhs: T) {
    return lhs//TODO
}

func &=(inout lhs: Int, rhs: Int) {
    return lhs//TODO
}

func &=(inout lhs: UInt, rhs: UInt) {
    return lhs//TODO
}

func &=(inout lhs: Int64, rhs: Int64) {
    return lhs//TODO
}
