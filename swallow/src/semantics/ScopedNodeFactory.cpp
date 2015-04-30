/* ScopedNodeFactory.cpp --
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
#include "semantics/ScopedNodeFactory.h"
#include "semantics/ScopedNodes.h"
#include "ast/InitializerDef.h"
USE_SWALLOW_NS


IdentifierPtr ScopedNodeFactory::createIdentifier(const SourceInfo& state)
{
    return NodeFactory::createIdentifier(state);
}
EnumDefPtr ScopedNodeFactory::createEnum(const SourceInfo& state)
{
    return create<ScopedEnum>(state);
}
StructDefPtr ScopedNodeFactory::createStruct(const SourceInfo& state)
{
    return create<ScopedStruct>(state);
}
ClassDefPtr ScopedNodeFactory::createClass(const SourceInfo& state)
{
    return create<ScopedClass>(state);
}
ProtocolDefPtr ScopedNodeFactory::createProtocol(const SourceInfo& state)
{
    return create<ScopedProtocol>(state);
}
ExtensionDefPtr ScopedNodeFactory::createExtension(const SourceInfo& state)
{
    return create<ScopedExtension>(state);
}
ProgramPtr ScopedNodeFactory::createProgram()
{
    return create<ScopedProgram>(SourceInfo());
}
CodeBlockPtr ScopedNodeFactory::createCodeBlock(const SourceInfo& state)
{
    return create<ScopedCodeBlock>(state);
}
ClosurePtr ScopedNodeFactory::createClosure(const SourceInfo& state)
{
    return create<ScopedClosure>(state);
}
FunctionDefPtr ScopedNodeFactory::createFunction(const SourceInfo& state)
{
    return create<SymboledFunction>(state);
}
InitializerDefPtr ScopedNodeFactory::createInitializer(const SourceInfo& state)
{
    return create<SymboledInit>(state);
}
DeinitializerDefPtr ScopedNodeFactory::createDeinitializer(const SourceInfo& state)
{
    return NodeFactory::createDeinitializer(state);
}
ComputedPropertyPtr ScopedNodeFactory::createComputedProperty(const SourceInfo& state)
{
    return create<ComposedComputedProperty>(state);
}
