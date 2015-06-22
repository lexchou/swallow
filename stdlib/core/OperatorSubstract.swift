
func -(lhs: UInt64, rhs: UInt64) -> UInt64 {
    return 0//TODO
}

func -(lhs: Int64, rhs: Int64) -> Int64 {
    return 0//TODO
}

func -(lhs: Int32, rhs: Int32) -> Int32 {
    return 0//TODO
}

func -(lhs: UInt, rhs: UInt) -> UInt {
    return 0//TODO
}

func -(lhs: Int, rhs: Int) -> Int {
    return 0//TODO
}

prefix func -(x: Float) -> Float {
    return 0//TODO
}

func -(lhs: UInt32, rhs: UInt32) -> UInt32 {
    return 0//TODO
}

func -(lhs: Int16, rhs: Int16) -> Int16 {
    return 0//TODO
}

func -(lhs: UInt16, rhs: UInt16) -> UInt16 {
    return 0//TODO
}

func -(lhs: Int8, rhs: Int8) -> Int8 {
    return 0//TODO
}

func -(lhs: UInt8, rhs: UInt8) -> UInt8 {
    return 0//TODO
}

func -<T>(lhs: UnsafePointer<T>, rhs: UnsafePointer<T>) -> Int {
    return 0//TODO
}

func -<T>(lhs: UnsafePointer<T>, rhs: Int) -> UnsafePointer<T> {
    return lhs//TODO
}

func -<T>(lhs: UnsafeMutablePointer<T>, rhs: UnsafeMutablePointer<T>) -> Int {
    return 0//TODO
}

func -<T>(lhs: UnsafeMutablePointer<T>, rhs: Int) -> UnsafeMutablePointer<T> {
    return lhs//TODO
}

func -<T : Strideable>(lhs: T, rhs: T) -> T.Stride {
    return 0//TODO
}

func -<T : Strideable>(lhs: T, rhs: T.Stride) -> T {
    return 0//TODO
}

prefix func -<T : _SignedNumberType>(x: T) -> T {
    return 0//TODO
}


/// Subtract `lhs` and `rhs`, returning a result and trapping in case of
/// arithmetic overflow (except in -Ounchecked builds).
func -<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T {
    return 0//TODO
}

func -(lhs: Float80, rhs: Float80) -> Float80 {
    return 0//TODO
}

prefix func -(x: Float80) -> Float80 {
    return 0//TODO
}

func -(lhs: Double, rhs: Double) -> Double {
    return 0//TODO
}

prefix func -(x: Double) -> Double {
    return 0//TODO
}

func -(lhs: Float, rhs: Float) -> Float {
    return 0//TODO
}

/// subtract `lhs` and `rhs`, silently discarding any overflow.
func &-<T : _IntegerArithmeticType>(lhs: T, rhs: T) -> T {
    return 0//TODO
}
