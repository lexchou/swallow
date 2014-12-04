/* Symbol.h --
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
#ifndef SYMBOL_H
#define SYMBOL_H
#include "swallow_conf.h"
#include <string>

SWALLOW_NS_BEGIN
typedef std::shared_ptr<class Type> TypePtr;

enum SymbolFlags
{
    //The symbol is still being initialized, used to detect uninitialized symbol used by another symbol's initialization
    SymbolFlagInitializing = 1,
    //The symbol is already initialized
    SymbolFlagInitialized = 2,
    //The symbol is a member symbol, which belongs to a type or an instance
    SymbolFlagMember = 4,
    //The symbol is writable, e.g. computed property declared with a setter, or variable declared by *var*
    SymbolFlagWritable = 8,
    //The symbol is readable
    SymbolFlagReadable = 0x10,
    //A temporary variable used for some intermediate works.
    SymbolFlagTemporary = 0x20,
    //The symbol is declared with a initializer
    SymbolFlagHasInitializer = 0x40,
    //Static members can be accessed directly from type
    SymbolFlagStatic = 0x80,
    //The symbol is a type's init, init function cannot be hold by a value binding, and cannot have return value
    SymbolFlagInit = 0x100,
    //The symbol is a type's deinit, deinit cannot have return value
    SymbolFlagDeinit = 0x200,
};

class SWALLOW_EXPORT Symbol
{
public:
    Symbol();
    virtual ~Symbol(){}
public:
    virtual const std::wstring& getName() const = 0;
    virtual TypePtr getType() {return nullptr;}
    void setFlags(int flags);
    int getFlags()const;
    bool hasFlags(SymbolFlags flags)const;
    void setFlags(SymbolFlags flags, bool set);

protected:
    int flags;
};
class SWALLOW_EXPORT SymbolPlaceHolder : public Symbol
{
public:
    enum Role
    {
        R_LOCAL_VARIABLE,
        R_PARAMETER,
        R_UPVALUE,
        R_PROPERTY
    };
public:
    SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, Role role, int flags);
public:
    virtual const std::wstring& getName() const override {return name;}
    virtual TypePtr getType() override {return type;}
    void setType(const TypePtr& type) {this->type = type;}
    Role getRole()const {return role;}
private:
    std::wstring name;
    TypePtr type;
    Role role;
};

SWALLOW_NS_END


#endif//SYMBOL_H
