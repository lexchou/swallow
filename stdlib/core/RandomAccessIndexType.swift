/// An *index* that can be offset by an arbitrary number of positions,
/// and can measure the distance to any reachable value, in O(1).
protocol RandomAccessIndexType : BidirectionalIndexType, _RandomAccessIndexType {
}
