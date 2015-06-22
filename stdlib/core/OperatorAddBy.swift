



/// Extend `lhs` with the elements of `rhs`
func += <T, S : SequenceType where T == T>(inout lhs: ContiguousArray<T>, rhs: S) {
    //TODO
}

func +=(inout lhs: UInt8, rhs: UInt8) {
    //TODO
}


/// Extend `lhs` with the elements of `rhs`
func += <T, C : CollectionType where T == T>(inout lhs: ContiguousArray<T>, rhs: C) {
    //TODO
}

func += <T>(inout lhs: UnsafeMutablePointer<T>, rhs: Int) {
    //TODO
}

func +=(inout lhs: String, rhs: String) {
    //TODO
}

func += <T : Strideable>(inout lhs: T, rhs: T.Stride) {
    //TODO
}


/// add `lhs` and `rhs` and store the result in `lhs`, trapping in
/// case of arithmetic overflow (except in -Ounchecked builds).
func += <T : _IntegerArithmeticType>(inout lhs: T, rhs: T) {
    //TODO
}

func +=(inout lhs: Float80, rhs: Float80) {
    //TODO
}

func +=(inout lhs: Double, rhs: Double) {
    //TODO
}

func +=(inout lhs: Float, rhs: Float) {
    //TODO
}

func +=(inout lhs: Int, rhs: Int) {
    //TODO
}

func +=(inout lhs: UInt, rhs: UInt) {
    //TODO
}

func +=(inout lhs: Int64, rhs: Int64) {
    //TODO
}


/// Extend `lhs` with the elements of `rhs`
func += <T, S : SequenceType where T == T>(inout lhs: Slice<T>, rhs: S) {
    //TODO
}


/// Extend `lhs` with the elements of `rhs`
func += <T, C : CollectionType where T == T>(inout lhs: Slice<T>, rhs: C) {
    //TODO
}

func +=(inout lhs: UInt64, rhs: UInt64) {
    //TODO
}

func +=(inout lhs: Int32, rhs: Int32) {
    //TODO
}

func +=(inout lhs: UInt32, rhs: UInt32) {
    //TODO
}

func +=(inout lhs: Int16, rhs: Int16) {
    //TODO
}


/// Extend `lhs` with the elements of `rhs`
func += <T, S : SequenceType where T == T>(inout lhs: [T], rhs: S) {
    //TODO
}


/// Extend `lhs` with the elements of `rhs`
func += <T, C : CollectionType where T == T>(inout lhs: [T], rhs: C) {
    //TODO
}


/// Append the elements of rhs to lhs
func += <T, C : CollectionType where T == T>(inout lhs: _ContiguousArrayBuffer<T>, rhs: C) {
    //TODO
}


/// Append rhs to lhs
func += <T>(inout lhs: _ContiguousArrayBuffer<T>, rhs: T) {
    //TODO
}

func +=(inout lhs: UInt16, rhs: UInt16) {
    //TODO
}

func +=(inout lhs: Int8, rhs: Int8) {
    //TODO
}

func += <T>(inout lhs: UnsafePointer<T>, rhs: Int) {
    //TODO
}
