/* ScopedNodeFactory.h --
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
#ifndef SCOPED_NODE_FACTORY_H
#define SCOPED_NODE_FACTORY_H
#include "ast/NodeFactory.h"

SWALLOW_NS_BEGIN

class SWALLOW_EXPORT ScopedNodeFactory : public NodeFactory
{
public:
    virtual ProgramPtr createProgram() override;
    virtual CodeBlockPtr createCodeBlock(const SourceInfo& state) override;
    virtual ClosurePtr createClosure(const SourceInfo& state) override;
    virtual ExtensionDefPtr createExtension(const SourceInfo& state) override;

    virtual FunctionDefPtr createFunction(const SourceInfo& state) override;
    virtual InitializerDefPtr createInitializer(const SourceInfo& state) override;
    virtual DeinitializerDefPtr createDeinitializer(const SourceInfo& state) override;
    virtual ComputedPropertyPtr createComputedProperty(const SourceInfo& state) override;

};

SWALLOW_NS_END

#endif//SCOPED_NODE_FACTORY_H
