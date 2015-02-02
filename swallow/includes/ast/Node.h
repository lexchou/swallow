/* Node.h --
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
#ifndef NODE_H
#define NODE_H
#include "swallow_conf.h"
#include "swallow_types.h"
#include "ast-decl.h"
#include <vector>
#include <sstream>
#include <memory>
#include "NodeVisitor.h"
#include "common/ScopedValue.h"
#ifdef TRACE_NODE
#include <list>
#endif//TRACE_NODE

SWALLOW_NS_BEGIN

class NodeVisitor;
struct NodeType
{
    enum T
    {
        ArrayLiteral,
        ArrayType,
        Assignment,
        Attribute,
        BinaryOperator,
        BooleanLiteral,
        Break,
        Case,
        Class,
        Closure,
        CodeBlock,
        Comment,
        CompileConstant,
        ConditionalOperator,
        ComputedProperty,
        Continue,
        Deinit,
        DictionaryLiteral,
        DictionaryType,
        Do,
        DynamicType,
        EnumCasePattern,
        Enum,
        Extension,
        Fallthrough,
        FloatLiteral,
        ForIn,
        For,
        ForcedValue,
        FunctionCall,
        Function,
        FunctionType,
        GenericArgumentDef,
        GenericConstraintDef,
        GenericParameterDef,
        Identifier,
        If,
        ImplicitlyUnwrappedOptional,
        Import,
        InOut,
        Init,
        InitRef,
        IntegerLiteral,
        LabeledStatement,
        MemberAccess,
        NilLiteral,
        Operator,
        OptionalChaining,
        OptionalType,
        Parameter,
        Parameters,
        ParenthesizedExpression,
        Program,
        ProtocolComposition,
        Protocol,
        Return,
        Self,
        StringInterpolation,
        StringLiteral,
        Struct,
        SubscriptAccess,
        Subscript,
        SwitchCase,
        Tuple,
        TupleType,
        TypedPattern,
        TypeAlias,
        TypeCase,
        TypeCheck,
        TypeIdentifier,
        UnaryOperator,
        ValueBinding,
        ValueBindings,
        ValueBindingPattern,
        While
    };
};

class NodeFactory;
class NodeVisitor;
class SWALLOW_EXPORT Node : public std::enable_shared_from_this<Node>
{
    friend class NodeFactory;
protected:
    Node(NodeType::T nodeType);
public:
    virtual ~Node();
public:
    SourceInfo* getSourceInfo();
    NodeType::T getNodeType();
    NodeFactory* getNodeFactory();
    NodePtr getParentNode() const;
public:
    virtual void accept(NodeVisitor* visitor){}

protected:
    template<class ASTNode>
    inline void accept2(NodeVisitor* visitor, void (NodeVisitor::*visit)(const std::shared_ptr<ASTNode>&))
    {
        std::shared_ptr<ASTNode> self = std::static_pointer_cast<ASTNode>(shared_from_this());
        this->parentNode = visitor->currentNode;
        SCOPED_SET(visitor->currentNode, self);
        (visitor->*visit)(self);
    }

public:
    /*!
     * Used for debugging, convert node type into it's class name
     */
    static const char* nodeTypeToName(NodeType::T nodeType);
protected:

protected:
    SourceInfo sourceInfo;
    NodeType::T nodeType;
    NodeFactory* nodeFactory;
    std::weak_ptr<Node> parentNode;
#ifdef TRACE_NODE
public:
    static std::list<Node*> UnreleasedNodes;
    static int NodeCount;
#endif
};
typedef std::shared_ptr<Node> NodePtr;

SWALLOW_NS_END


#endif//NODE_H
