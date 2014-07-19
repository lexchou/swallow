#include "node-visitor.h"
#include "pattern.h"
#include "conditional-operator.h"
#include "binary-operator.h"
#include "unary-operator.h"
#include "tuple.h"
#include "identifier.h"
#include "compile-constant.h"
#include "enum-case-pattern.h"
#include "member-access.h"
#include "function-call.h"
#include "initializer-reference.h"
#include "optional-chaining.h"
#include "forced-value.h"
#include "self-expression.h"
#include "parenthesized-expression.h"
#include "subscript-access.h"
#include "closure.h"
#include "statement.h"
#include "dynamic-type.h"
USE_SWIFT_NS




void NodeVisitor::acceptPattern(Pattern* pattern, NodeVisitor* visitor)
{
    if(!pattern)
        return;
    NodeType::T type = pattern->getNodeType();
    switch(type)
    {
        case NodeType::ConditionalOperator:
        {
            ConditionalOperator* op = static_cast<ConditionalOperator*>(pattern);
            visitor->visitConditionalOperator(op);
            acceptPattern(op->getCondition(), visitor);
            acceptPattern(op->getTrueExpression(), visitor);
            acceptPattern(op->getFalseExpression(), visitor);
            break;
        }
        case NodeType::BinaryOperator:
        {
            BinaryOperator* op = static_cast<BinaryOperator*>(pattern);
            visitor->visitBinaryOperator(op);
            acceptPattern(op->getLHS(), visitor);
            acceptPattern(op->getRHS(), visitor);
            break;
        }
        case NodeType::InOut:
        case NodeType::UnaryOperator:
        {
            UnaryOperator* op = static_cast<UnaryOperator*>(pattern);
            visitor->visitUnaryOperator(op);
            acceptPattern(op->getOperand(), visitor);
            break;
        }
        case NodeType::Identifier:
        {
            Identifier* id = static_cast<Identifier*>(pattern);
            visitor->visitIdentifier(id);
            break;
        }
        case NodeType::CompileConstant:
        {
            CompileConstant* id = static_cast<CompileConstant*>(pattern);
            visitor->visitCompileConstant(id);
            break;
        }
        case NodeType::Tuple:
        {
            Tuple* tuple = static_cast<Tuple*>(pattern);
            visitor->visitTuple(tuple);
            for(Pattern* child : tuple->elements)
            {
                acceptPattern(child, visitor);
            }
            break;
        }
        case NodeType::LetBinding:
        case NodeType::VarBinding:
            break;
        case NodeType::EnumCasePattern:
        {
            EnumCasePattern* e = static_cast<EnumCasePattern*>(pattern);
            visitor->visitEnumCasePattern(e);
            break;
        }
        case NodeType::Closure:
        {
            Closure* c = static_cast<Closure*>(pattern);
            visitor->visitClosure(c);
            acceptPattern(c->getCapture(), visitor);
            for(Statement* st : c->statements)
            {
                visitor->visitStatement(st);
            }

            break;
        }
        case NodeType::DynamicType:
        {
            DynamicType* d = static_cast<DynamicType*>(pattern);
            visitor->visitDynamicType(d);
            acceptPattern(d->getExpression(), visitor);
            break;
        }
        case NodeType::ForcedValue:
        {
            ForcedValue* f = static_cast<ForcedValue*>(pattern);
            visitor->visitForcedValue(f);
            acceptPattern(f->getExpression(), visitor);
            break;
        }
        case NodeType::FunctionCall:
        {
            FunctionCall* f = static_cast<FunctionCall*>(pattern);
            visitor->visitFunctionCall(f);
            acceptPattern(f->getFunction(), visitor);
            acceptPattern(f->getArguments(), visitor);
            acceptPattern(f->getTrailingClosure(), visitor);
            break;
        }
        case NodeType::InitRef:
        {
            InitializerReference* i = static_cast<InitializerReference*>(pattern);
            visitor->visitInitializerReference(i);
            acceptPattern(i->getExpression(), visitor);
            break;
        }
        case NodeType::MemberAccess:
        {
            MemberAccess* m = static_cast<MemberAccess*>(pattern);
            visitor->visitMemberAccess(m);
            acceptPattern(m->getSelf(), visitor);
            acceptPattern(m->getField(), visitor);
            break;
        }
        case NodeType::OptionalChaining:
        {
            OptionalChaining* o = static_cast<OptionalChaining*>(pattern);
            visitor->visitOptionalChaining(o);
            acceptPattern(o->getExpression(), visitor);
            break;
        }
        case NodeType::ParenthesizedExpression:
        {
            ParenthesizedExpression* p = static_cast<ParenthesizedExpression*>(pattern);
            visitor->visitParenthesizedExpression(p);
            for(const ParenthesizedExpression::Term& term : p->expressions)
            {
                acceptPattern(term.second, visitor);
            }
            break;
        }
        case NodeType::Self:
        {
            SelfExpression* s = static_cast<SelfExpression*>(pattern);
            visitor->visitSelf(s);
            acceptPattern(s->getExpression(), visitor);
            break;
        }
        case NodeType::SubscriptAccess:
        {
            SubscriptAccess* sub = static_cast<SubscriptAccess*>(pattern);
            visitor->visitSubscriptAccess(sub);
            acceptPattern(sub->getSelf(), visitor);
            for(Expression* index : sub->indices)
            {
                acceptPattern(index, visitor);
            }
            break;
        }
        default:
            break;
    }
}
