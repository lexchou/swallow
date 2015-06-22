
/// Returns a closed interval from `start` through `end`
func ...<T : Comparable>(start: T, end: T) -> ClosedInterval<T> {
    return ClosedInterval<T>()//TODO
}


/// Forms a closed range that contains both `start` and `end`.
/// Requres: `start <= end`
func ...<Pos : ForwardIndexType where Pos : Comparable>(start: Pos, end: Pos) -> Range<Pos> {
    return Range<Pos>()//TODO
}


/// Forms a closed range that contains both `minimum` and `maximum`.
func ...<Pos : ForwardIndexType>(minimum: Pos, maximum: Pos) -> Range<Pos> {
    return Range<Pos>()//TODO
}


/// Forms a half-open range that contains `start`, but not
/// `end`.  Requires: `start <= end`
func ..<<Pos : ForwardIndexType where Pos : Comparable>(start: Pos, end: Pos) -> Range<Pos> {
    return Range<Pos>()//TODO
}


/// Forms a half-open range that contains `minimum`, but not
/// `maximum`.
func ..<<Pos : ForwardIndexType>(minimum: Pos, maximum: Pos) -> Range<Pos> {
    return Range<Pos>()//TODO
}


/// Returns a half-open interval from `start` to `end`
func ..<<T : Comparable>(start: T, end: T) -> HalfOpenInterval<T> {
    return HalfOpenInterval<Pos>()//TODO
}
