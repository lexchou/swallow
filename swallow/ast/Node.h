#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include "swift_types.h"
#include "ast-decl.h"
#include <vector>
#include <sstream>
#include <memory>
#ifdef TRACE_NODE
#include <list>
#endif//TRACE_NODE

SWIFT_NS_BEGIN

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
class Node : public std::enable_shared_from_this<Node>
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
public:
    virtual void accept(NodeVisitor* visitor){}

protected:
    template<class ASTNode>
    inline void accept2(NodeVisitor* visitor, void (NodeVisitor::*visit)(const std::shared_ptr<ASTNode>&))
    {
        std::shared_ptr<ASTNode> ptr = std::static_pointer_cast<ASTNode>(shared_from_this());
        (visitor->*visit)(ptr);
    }

public:
    /**
     * Used for debugging, convert node type into it's class name
     */
    static const char* nodeTypeToName(NodeType::T nodeType);
protected:

protected:
    SourceInfo sourceInfo;
    NodeType::T nodeType;
    NodeFactory* nodeFactory;
#ifdef TRACE_NODE
public:
    static std::list<Node*> UnreleasedNodes;
    static int NodeCount;
#endif
};
typedef std::shared_ptr<Node> NodePtr;

SWIFT_NS_END


#endif//NODE_H
