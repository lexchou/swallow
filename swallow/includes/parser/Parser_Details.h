/* Parser_Details.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PARSER_DETAILS_H
#define PARSER_DETAILS_H
#include "swallow_conf.h"
#include "common/ScopedValue.h"
SWALLOW_NS_BEGIN

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

#define ENTER_CONTEXT(ctx) TokenizerState& state_##__LINE__ = tokenizer->save(); SCOPED_SET(state_##__LINE__.context, ctx);


SWALLOW_NS_END
#endif//PARSER_DETAILS_H
