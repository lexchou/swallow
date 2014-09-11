#ifndef SWIFT_ERRORS_H
#define SWIFT_ERRORS_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

struct Errors
{
    enum
    {
        E_UNEXPECTED_EOF = 0x1000,
        //grammar errors
        E_EXPECT,// % is expected
        E_UNEXPECTED,// Unexpected %
        E_EXPECT_IDENTIFIER,// Identifier expected, but % found.
        E_EXPECT_KEYWORD,// Keyword %s expected
        E_EXPECT_EXPRESSION, // Expression expected but % found.
        E_EXPECT_OPERATOR, // Operator expected but % found.
        E_EXPECT_INTEGER_PRECEDENCE,// Operator's precedence must be an integer
        E_INVALID_OPERATOR_PRECEDENCE, // Operator's precedence must be between [0, 255]
        E_EXPECT_INIT_SELF_DYNAMICTYPE_IDENTIFIER, // init/self/dynamicType or member field is expected, but % found.
        E_UNDEFINED_INFIX_OPERATOR, // Undefined infix operator %
        E_EXPECT_CAPTURE_SPECIFIER, // The capture specifier is not specified.
        E_EXPECT_CASE, // case/default is expected in switch/case statement
        E_GETTER_SETTER_CAN_ONLY_BE_DEFINED_FOR_A_SINGLE_VARIABLE, // Getter/setter can only be defined for a single variable
        
        //semantic errors
        E_INVALID_REDECLARATION, // Invalid redeclaration of type %
        E_USE_OF_UNDECLARED_TYPE, // Use of undeclared type %
        E_CANNOT_ASSIGN, // cannot assign to the result of this expression
        E_USE_OF_UNRESOLVED_IDENTIFIER, //use of unresolved identifier '%'
        E_USE_OF_UNINITIALIZED_VARIABLE, //use of local variable '%' before its declaration
        E_USE_OF_INITIALIZING_VARIABLE, //variable used within its own initial value
        E_DEFINITION_CONFLICT, //definition conflicts with previous value
        E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, //use of function local variable inside type declaration

        E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE,//tuple pattern cannot match values of the non-tuple type '%'
        E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT, //tuple types '($T0, $T1)' and '($T2, $T3, $T4)' have a different number of elements (2 vs. 3)
        E_OPERATOR_REDECLARED,//operator redeclared
        E_OPERATOR_PRECEDENCE_OUT_OF_RANGE,//'precedence' must be in the range of 0 to 255
        E_UNKNOWN_BINARY_OPERATOR, //operator is not a known binary operator
        E_IS_NOT_BINARY_OPERATOR, //'%' is not a binary operator
        E_NO_OVERLOAD_ACCEPTS_ARGUMENTS,//could not find an overload for '%' that accepts the supplied arguments
        E_INVALID_CALL_OF_NON_FUNCTION_TYPE,//invalid use of '()' to call a value of non-function type '%'
        E_UNMATCHED_PARAMETERS, // Unmatched number of supplied parameters
        E_UNMATCHED_PARAMETER, // Unmatched type of parameter #%
        E_NO_MATCHED_OVERLOAD, // No matched overload found
        E_MISSING_ARGUMENT_LABEL_IN_CALL,// Missing argument label '%:' in call
        E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL, //Extraneous argument label '%:' in call
        E_EXTRANEOUS_ARGUMENT, //Extraneous argument in call
        E_AMBIGUOUS_USE,    // Ambiguous use of '%'
        E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC, // Inout arguments cannot be variadic
        E_DOES_NOT_HAVE_A_MEMBER, // 'XXX' does not have a member named '%'

        E_CANNOT_CONVERT_EXPRESSION_TYPE, //Cannot convert expression's type '%' to type '%'
        E_LET_REQUIRES_INITIALIZER, //'let' declarations require an initializer expression
        E_CANNOT_DEFINE_EMPTY_ARRAY_WITHOUT_TYPE, // cannot define an empty array without type declaration.
        E_ARRAY_CONTAINS_DIFFERENT_TYPES, // Array contains different types
        E_TYPE_DOES_NOT_CONFORM_TO_ARRAY_PROTOCOL, // Type '%' does not conform to ArrayLiteralConvertible protocol
        E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE, // Superclass '%' must appear first in the inheritance clause
        E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE, //Inheritance from none-protocol, non-class type '%'
        E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE, //Inheritance from none-protocol type '%'
        E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD, //Default argument not permitted in a protocol method
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION,// Type % does not conform to protocol %, unimplemeneted function %
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE, //Type % does not conform to protocol %, unimplemented type %
        E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT, // Protocol % cannot define let constant
        E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY, // Protocol %'s variable must be a computed property
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY,
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY,
        E_TYPE_ANNOTATION_MISSING_IN_PATTERN, // Type annotation missing in pattern
        E_NESTED_TYPE_IS_NOT_ALLOWED_HERE,


        W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY, // '% %' can be expressed more succinctly as '#%'
        W_EXTRANEOUS_SHARTP_IN_PARAMETER, //Extraneous '#' in parameter: '%' is already the keyword argument name
        //linking errors
    };
    
};


SWIFT_NS_END

#endif//SWIFT_ERRORS_H