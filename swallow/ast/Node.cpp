/* Node.cpp --
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
#include "Node.h"
#include <cstdlib>
#include <cassert>
#include <set>
#include <algorithm>
USE_SWALLOW_NS

#ifdef TRACE_NODE
int Node::NodeCount = 0;
std::list<Node*> Node::UnreleasedNodes;

#endif




/*!
* Used for debugging, convert node type into it's class name
*/
const char* Node::nodeTypeToName(NodeType::T nodeType)
{
#define CASE_TYPE(T) case NodeType::T: return #T;
    switch(nodeType)
    {
    CASE_TYPE(ArrayLiteral)
    CASE_TYPE(ArrayType)
    CASE_TYPE(Assignment)
    CASE_TYPE(Attribute)
    CASE_TYPE(BinaryOperator)
    CASE_TYPE(BooleanLiteral)
    CASE_TYPE(Break)
    CASE_TYPE(Case)
    CASE_TYPE(Class)
    CASE_TYPE(Closure)
    CASE_TYPE(CodeBlock)
    CASE_TYPE(Comment)
    CASE_TYPE(CompileConstant)
    CASE_TYPE(ConditionalOperator)
    CASE_TYPE(ComputedProperty)
    CASE_TYPE(Continue)
    CASE_TYPE(Deinit)
    CASE_TYPE(DictionaryLiteral)
    CASE_TYPE(DictionaryType)
    CASE_TYPE(Do)
    CASE_TYPE(DynamicType)
    CASE_TYPE(EnumCasePattern)
        CASE_TYPE(Enum)
        CASE_TYPE(Extension)
        CASE_TYPE(Fallthrough)
        CASE_TYPE(FloatLiteral)
        CASE_TYPE(ForIn)
        CASE_TYPE(For)
        CASE_TYPE(ForcedValue)
        CASE_TYPE(FunctionCall)
        CASE_TYPE(Function)
        CASE_TYPE(FunctionType)
        CASE_TYPE(GenericArgumentDef)
        CASE_TYPE(GenericConstraintDef)
        CASE_TYPE(GenericParameterDef)
        CASE_TYPE(Identifier)
        CASE_TYPE(If)
        CASE_TYPE(ImplicitlyUnwrappedOptional)
        CASE_TYPE(Import)
        CASE_TYPE(InOut)
        CASE_TYPE(Init)
        CASE_TYPE(InitRef)
        CASE_TYPE(IntegerLiteral)
        CASE_TYPE(LabeledStatement)
        CASE_TYPE(MemberAccess)
        CASE_TYPE(NilLiteral)
        CASE_TYPE(Operator)
        CASE_TYPE(OptionalChaining)
        CASE_TYPE(OptionalType)
        CASE_TYPE(Parameter)
        CASE_TYPE(Parameters)
        CASE_TYPE(ParenthesizedExpression)
        CASE_TYPE(Program)
        CASE_TYPE(ProtocolComposition)
        CASE_TYPE(Protocol)
        CASE_TYPE(Return)
        CASE_TYPE(Self)
        CASE_TYPE(StringInterpolation)
        CASE_TYPE(StringLiteral)
        CASE_TYPE(Struct)
        CASE_TYPE(SubscriptAccess)
        CASE_TYPE(Subscript)
        CASE_TYPE(SwitchCase)
        CASE_TYPE(Tuple)
        CASE_TYPE(TupleType)
        CASE_TYPE(TypeAlias)
        CASE_TYPE(TypeCase)
        CASE_TYPE(TypeCheck)
        CASE_TYPE(TypeIdentifier)
        CASE_TYPE(TypedPattern)
        CASE_TYPE(UnaryOperator)
        CASE_TYPE(ValueBinding)
        CASE_TYPE(ValueBindings)
        CASE_TYPE(ValueBindingPattern)
        CASE_TYPE(While)
    }
#undef CASE_TYPE
    return "<invalid-node-type>";
}



Node::Node(NodeType::T nodeType)
:nodeType(nodeType), nodeFactory(nullptr)
{
#ifdef TRACE_NODE
    NodeCount++;
    UnreleasedNodes.push_back(this);
#endif
}
Node::~Node()
{
#ifdef TRACE_NODE
    NodeCount--;
    std::list<Node*>::iterator iter = std::find(UnreleasedNodes.begin(), UnreleasedNodes.end(), this);
    if(iter != UnreleasedNodes.end())
        UnreleasedNodes.erase(iter);
#endif
}

NodeFactory* Node::getNodeFactory()
{
    return nodeFactory;
}

SourceInfo* Node::getSourceInfo()
{
    return &sourceInfo;
}
NodeType::T Node::getNodeType()
{
    return nodeType;
}
