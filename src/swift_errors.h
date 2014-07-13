#ifndef SWIFT_ERRORS_H
#define SWIFT_ERRORS_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

struct Errors
{
    enum
    {
        E_UNEXPECTED_EOF = 0x1000
        //grammar errors
        
        //semantic errors
    };
    
};

SWIFT_NS_END

#endif//SWIFT_ERRORS_H