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
    SymbolFlagInitializing = 1 << 0,
    //The symbol is already initialized
    SymbolFlagInitialized = 1 << 1,
    //The symbol is a member symbol, which belongs to a type or an instance
    SymbolFlagMember = 1 << 2,
    //The symbol is writable, e.g. computed property declared with a setter, or variable declared by *var*
    SymbolFlagWritable = 1 << 3,
    //This symbol is nonmutating, different to SymbolFlagWritable, it means all its member are also not changable.
    //e.g. self in a class has no SymbolFlagWritable and no SymbolFlagNonmutating, self in a struct has no SymbolFlagWritable but has SymbolFlagNonmutating
    SymbolFlagNonmutating = 1 << 4,
    //The function is mutating
    SymbolFlagMutating = 1 << 5,
    //The symbol is readable
    SymbolFlagReadable = 1 << 6,
    //A temporary variable used for some intermediate works.
    SymbolFlagTemporary = 1 << 7,
    //The symbol is declared with a initializer
    SymbolFlagHasInitializer = 1 << 8,
    //Static members can be accessed directly from type
    SymbolFlagStatic = 1 << 9,
    //The symbol is a type's init, init function cannot be hold by a value binding, and cannot have return value
    SymbolFlagInit = 1 << 10,
    //The symbol is a type's convenience init, it's not allowed to call another convenience init inside a convenience initializer
    //A convenience initializer can have SymbolFlagInit and SymbolFlagConvenienceInit both.
    SymbolFlagConvenienceInit = 1 << 11,
    //The symbol is a type's deinit, deinit cannot have return value
    SymbolFlagDeinit = 1 << 12,
    SymbolFlagPrefix = 1 << 13,
    SymbolFlagInfix = 1 << 14,
    SymbolFlagPostfix = 1 << 15,
    //The symbol is declared through extension
    SymbolFlagExtension = 1 << 16,
    //The variable is lazy
    SymbolFlagLazy = 1 << 17,
    //This is a stored property declared on a type
    SymbolFlagStoredProperty = 1 << 18,
    //This is a computed property declared on a type
    SymbolFlagComputedProperty = 1 << 19,
    //This is a final symbol, not allowed to be overridden
    SymbolFlagFinal = 1 << 20,
    //It's a failable initializer, allowed to return nil
    SymbolFlagFailableInitializer = 1 << 21,
    //It's a failable initializer, but it returns an ImplicitlyUnwrappedOptional<T>
    SymbolFlagImplicitFailableInitializer = 1 << 22,
    //It's required for a sub-class to implement the initializer
    SymbolFlagRequired = 1 << 23,
};

class SWALLOW_EXPORT Symbol
{
    friend class TypeBuilder;
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

    /*!
     * If this symbol is defined as a member of a type, returns the type that declared this symbol.
     */
    TypePtr getDeclaringType() const;
protected:
    int flags;
    TypePtr declaringType;
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
