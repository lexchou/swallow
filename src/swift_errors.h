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


        //linking errors
    };
    
};

SWIFT_NS_END

#endif//SWIFT_ERRORS_H