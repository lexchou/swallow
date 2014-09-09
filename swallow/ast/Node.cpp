#include "Node.h"
#include "common/AutoReleasePool.h"
#include <cstdlib>
#include <cassert>
#include <set>
#include <algorithm>
USE_SWIFT_NS

#ifdef TRACE_NODE
int Node::NodeCount = 0;
std::list<Node*> Node::UnreleasedNodes;

#endif




/**
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
        CASE_TYPE(GenericArgument)
        CASE_TYPE(GenericConstraint)
        CASE_TYPE(GenericParameter)
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
}



Node::Node(NodeType::T nodeType)
:autoReleasePool(NULL), nodeType(nodeType)
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
    this->setAutoReleasePool(NULL);
}

void Node::setAutoReleasePool(AutoReleasePool* autoReleasePool)
{
    if(this->autoReleasePool == autoReleasePool)
        return;
    if(this->autoReleasePool)
    {
        AutoReleasePool* old = this->autoReleasePool;
        this->autoReleasePool = NULL;
        old->remove(this);

    }
    this->autoReleasePool = autoReleasePool;
    if(autoReleasePool)
    {
        autoReleasePool->add(this);
    }
}
AutoReleasePool* Node::getAutoReleasePool()
{
    return autoReleasePool;
}

SourceInfo* Node::getSourceInfo()
{
    return &sourceInfo;
}
NodeType::T Node::getNodeType()
{
    return nodeType;
}
