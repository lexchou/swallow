/* ScopedNodeFactory.cpp --
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
#include "ScopedNodeFactory.h"
#include "ScopedNodes.h"
USE_SWIFT_NS


IdentifierPtr ScopedNodeFactory::createIdentifier(const SourceInfo& state)
{
    return NodeFactory::createIdentifier(state);
}
EnumDefPtr ScopedNodeFactory::createEnum(const SourceInfo& state)
{
    return _(state, new ScopedEnum());
}
StructDefPtr ScopedNodeFactory::createStruct(const SourceInfo& state)
{
    return _(state, new ScopedStruct());
}
ClassDefPtr ScopedNodeFactory::createClass(const SourceInfo& state)
{
    return _(state, new ScopedClass());
}
ProtocolDefPtr ScopedNodeFactory::createProtocol(const SourceInfo& state)
{
    return _(state, new ScopedProtocol());
}
ExtensionDefPtr ScopedNodeFactory::createExtension(const SourceInfo& state)
{
    return _(state, new ScopedExtension());
}
ProgramPtr ScopedNodeFactory::createProgram()
{
    return _(SourceInfo(), new ScopedProgram());
}
CodeBlockPtr ScopedNodeFactory::createCodeBlock(const SourceInfo& state)
{
    return _(state, new ScopedCodeBlock());
}
ClosurePtr ScopedNodeFactory::createClosure(const SourceInfo& state)
{
    return _(state, new ScopedClosure());
}
FunctionDefPtr ScopedNodeFactory::createFunction(const SourceInfo& state)
{
    return _(state, new SymboledFunction());
}
