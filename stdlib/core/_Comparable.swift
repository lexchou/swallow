/// This protocol is an implementation detail of `Comparable`; do
/// not use it directly.
///
/// Its requirements are inherited by `Comparable` and thus must
/// be satisfied by types conforming to that protocol.
protocol _Comparable {

    /// A `strict total order
    /// <http://en.wikipedia.org/wiki/Total_order#Strict_total_order>`_
    /// over instances of `Self`
    func <(lhs: Self, rhs: Self) -> Bool
}
