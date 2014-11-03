/* Symbol.h --
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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
#ifndef SYMBOL_H
#define SYMBOL_H
#include "swift_conf.h"
#include <string>

SWIFT_NS_BEGIN
typedef std::shared_ptr<class Type> TypePtr;
class Symbol
{
public:
    Symbol(){}
    virtual ~Symbol(){}
public:
    virtual const std::wstring& getName() const = 0;
    virtual TypePtr getType() {return nullptr;}
};
class SymbolPlaceHolder : public Symbol
{
public:
    enum Role
    {
        R_LOCAL_VARIABLE,
        R_PARAMETER,
        R_UPVALUE,
        R_PROPERTY
    };
    enum Flags
    {
        F_INITIALIZING = 1,
        F_INITIALIZED = 2,
        F_MEMBER= 4,
        F_WRITABLE = 8,
        F_READABLE = 0x10


    };
public:
    SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, Role role, int flags)
    :name(name), type(type), role(role), flags(flags){}
public:
    virtual const std::wstring& getName() const override {return name;}
    virtual TypePtr getType() override {return type;}
    void setType(const TypePtr& type) {this->type = type;}
    Role getRole()const {return role;}
private:
    std::wstring name;
    TypePtr type;
    Role role;
public:
    int flags;

};

SWIFT_NS_END


#endif//SYMBOL_H