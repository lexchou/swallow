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
};
struct Flags
{
    Flags(Parser* parser)
        :parser(parser), oldFlags(parser->flags)
    {
        parser->flags = flags = 0;
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