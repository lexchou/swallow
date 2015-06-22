/// A sequence that forwards its implementation to an underlying
/// sequence instance while exposing lazy computations as methods.
struct LazySequence<S : SequenceType> : SequenceType {

    /// Construct an instance with `base` as its underlying sequence
    /// instance.
    init(_ base: S) {
        //TODO
    }

    /// Return a *generator* over the elements of this *sequence*.
    ///
    /// Complexity: O(1)
    func generate() -> S.Generator {
        return S.Generator()//TODO
    }

    /// an Array, created on-demand, containing the elements of this
    /// lazy SequenceType.
    var array: [S.Generator.Element] {
        get {
            return []//TODO
        }
    }
}
