

/// How children of this value should be presented in the IDE.
enum MirrorDisposition {
    case Struct
    case Class
    case Enum
    case Tuple
    case Aggregate
    case IndexContainer
    case KeyContainer
    case MembershipContainer
    case Container
    case Optional
    case ObjCObject
}


/// The type returned by `reflect(x)`; supplies an API for runtime
/// reflection on `x`
protocol MirrorType {

    /// The instance being reflected
    var value: Any { get }

    /// Identical to `value.dynamicType`
    var valueType: Any.Type { get }

    /// A unique identifier for `value` if it is a class instance; `nil`
    /// otherwise.
    var objectIdentifier: ObjectIdentifier? { get }

    /// The count of `value`\ 's logical children 
    var count: Int { get }
    subscript (i: Int) -> (String, MirrorType) { get }

    /// A string description of `value`.
    var summary: String { get }

    /// A rich representation of `value` for an IDE, or `nil` if none is supplied.
    var quickLookObject: QuickLookObject? { get }

    /// How `value` should be presented in an IDE.
    var disposition: MirrorDisposition { get }
}
