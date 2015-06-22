
/// The protocol to which all types implicitly conform
typealias Any = protocol<>



/// The protocol to which all classes implicitly conform.
///
/// When used as a concrete type, all known `@objc` methods and
/// properties are available, as implicitly-unwrapped-optional methods
/// and properties respectively, on each instance of `AnyObject`.  For
/// example:
///
/// .. parsed-literal:
///
///   class C {
///     @objc func getCValue() -> Int { return 42 }
///   }
///
///   // If x has a method @objc getValue()->Int, call it and
///   // return the result.  Otherwise, return nil.
///   func getCValue1(x: AnyObject) -> Int? {
///     if let f: ()->Int = **x.getCValue** {
///       return f()
///     }
///     return nil
///   }
///
///   // A more idiomatic implementation using "optional chaining"
///   func getCValue2(x: AnyObject) -> Int? {
///     return **x.getCValue?()**
///   }
///
///   // An implementation that assumes the required method is present
///   func getCValue3(x: AnyObject) -> **Int** {
///     return **x.getCValue()** // x.getCValue is implicitly unwrapped.
///   }
///
/// See also: `AnyClass`
@objc protocol AnyObject {
}


/// The protocol to which all class types implicitly conform.
///
/// When used as a concrete type, all known `@objc` `class` methods and
/// properties are available, as implicitly-unwrapped-optional methods
/// and properties respectively, on each instance of `AnyClass`. For
/// example:
///
/// .. parsed-literal:
///
///   class C {
///     @objc class var cValue: Int { return 42 }
///   }
///
///   // If x has an @objc cValue: Int, return its value.  
///   // Otherwise, return nil.
///   func getCValue(x: AnyClass) -> Int? {
///     return **x.cValue**
///   }
///
/// See also: `AnyObject`
typealias AnyClass = AnyObject.Type
