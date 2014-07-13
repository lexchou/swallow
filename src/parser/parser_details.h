#ifndef PARSER_DETAILS_H
#define PARSER_DETAILS_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

enum
{
    ENABLE_GENERIC      = 1,
    UNDER_SWITCH_CASE   = 2,
    UNDER_PROTOCOL      = 4,
    UNDER_CLASS         = 8,
    UNDER_STRUCT        = 0x10,
    UNDER_ENUM          = 0x20,
    UNDER_LET           = 0x40,
    UNDER_VAR           = 0x80,
    UNDER_CASE          = 0x100,
    UNDER_IF            = 0x200,
    UNDER_FOR_LOOP      = 0x400,
    UNDER_WHILE_LOOP    = 0x800,
    UNDER_DO_LOOP       = 0x1000,
    SUPPRESS_TRAILING_CLOSURE = 0x2000,
    UNDER_EXTENSION     = 0x4000
};
struct Flags
{
    Flags(Parser* parser, int f = 0)
        :parser(parser), oldFlags(parser->flags)
    {
        this->flags = parser->flags;
        if(f)
            *this += f;
    }
    Flags(const Flags& flags)
        :oldFlags(flags.parser->flags)
    {
        this->parser = flags.parser;
        parser->flags = this->flags = flags.flags;
    }
    ~Flags()
    {
        parser->flags = oldFlags;
    }
    
    
    Flags& operator += (int flag)
    {
        flags |= flag;
        parser->flags = flags;
        return *this;
    }
    Flags& operator -= (int flag)
    {
        flags &= ~flag;
        parser->flags = flags;
        return *this;
    }
    
    
    Parser* parser;
    int oldFlags;
    int flags;
};


SWIFT_NS_END
#endif//PARSER_DETAILS_H