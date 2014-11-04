/* TypeBuilder.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
#ifndef TYPE_BUILDER_H
#define TYPE_BUILDER_H
#include "Type.h"


SWALLOW_NS_BEGIN

class SWALLOW_DLL TypeBuilder : public Type
{
public://Constructors
    TypeBuilder(Category category);
public:
    void setInitializer(const FunctionOverloadedSymbolPtr& initializer);

    void setParentType(const TypePtr& type);

    void setInnerType(const TypePtr& type);

    /**
    * Adds a protocol that this type conform to
    */
    void addProtocol(const TypePtr& protocol);

    /**
     * Add function's parameter if it's a function type
     */
    void addParameter(const Parameter& param);

    void addMember(const std::wstring& name, const SymbolPtr& member);
    void addMember(const SymbolPtr& symbol);
    void addParentTypesFrom(const TypePtr& type);
    void addParentType(const TypePtr& type, int distance);

};
typedef std::shared_ptr<TypeBuilder> TypeBuilderPtr;


SWALLOW_NS_END


#endif//TYPE_BUILDER_H
