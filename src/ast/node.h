#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include "swift_types.h"
#include <vector>
#include <sstream>
#ifdef TRACE_NODE
#include <list>
#endif//TRACE_NODE

SWIFT_NS_BEGIN

class AutoReleasePool;
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
        Constant,
        Continue,
        Deinit,
        DictionaryLiteral,
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
        GenericArgument,
        GenericConstraint,
        GenericParameter,
        Identifier,
        If,
        ImplicitlyUnwrappedOptional,
        Import,
        InOut,
        Init,
        InitRef,
        IntegerLiteral,
        LabeledStatement,
        LetBinding,
        MemberAccess,
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
        StringLiteral,
        Struct,
        SubscriptAccess,
        Subscript,
        SwitchCase,
        Tuple,
        TupleType,
        TypeAlias,
        TypeCase,
        TypeCheck,
        TypeIdentifier,
        UnaryOperator,
        Variable,
        Variables,
        VarBinding,
        While
    };
};

class Node
{
protected:
    Node(NodeType::T nodeType);
public:
    virtual ~Node();
public:
    SourceInfo* getSourceInfo();
    void setAutoReleasePool(AutoReleasePool* autoReleasePool);
    AutoReleasePool* getAutoReleasePool();
    NodeType::T getNodeType();
public:
    virtual void accept(NodeVisitor* visitor){}
public:
    virtual void serialize(std::wostream& out) {};
protected:

protected:
    SourceInfo sourceInfo;
    AutoReleasePool* autoReleasePool;
    NodeType::T nodeType;

#ifdef TRACE_NODE
public:
    static std::list<Node*> UnreleasedNodes;
    static int NodeCount;
#endif
};


SWIFT_NS_END


#endif//NODE_H
