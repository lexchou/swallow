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
        //linking errors
    };
    
};

SWIFT_NS_END

#endif//SWIFT_ERRORS_H