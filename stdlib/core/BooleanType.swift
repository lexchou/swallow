/// A type that represents a boolean value.
///
/// Types that conform to the `BooleanType` protocol can be used as
/// the condition in control statements (`if`, `while`, C-style `for`)
/// and other logical value contexts (e.g., `case` statement guards).
///
/// Only two types provided by Swift, `Bool` and `ObjCBool`, conform
/// to `BooleanType`. Expanding this set to include types that
/// represent more than simple boolean values is discouraged.
protocol BooleanType {

    /// The value of `self`, expressed as a `Bool`.
    var boolValue: Bool { get }
}
