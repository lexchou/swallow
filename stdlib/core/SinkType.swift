/// Instances of conforming types are effectively functions with the
/// signature `(Element) -> Void`.
///
/// Useful mainly when the optimizer's ability to specialize generics
/// outstrips its ability to specialize ordinary closures.  For
/// example, you may find that instead of::
///
///   func f(g: (X)->Void) { ... g(a) ...}
///
/// the following generates better code::
///
///   func f<T: Sink where T.Element == X>(g: T) { ... g.put(a) ...}
protocol SinkType {

    /// The type of element to be written to this sink.
    typealias Element

    /// Write `x` to this sink.
    mutating func put(x: Element)
}
