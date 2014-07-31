#ifndef TYPE_INFERENCE_ACTION_H
#define TYPE_INFERENCE_ACTION_H
#include "swift_conf.h"
#include "ast/node-visitor.h"
#include "semantic-node-visitor.h"
SWIFT_NS_BEGIN


class Type;
class Expression;
class TypeInferenceAction : public SemanticNodeVisitor
{
public:
    TypeInferenceAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitVariable(const VariablePtr& node);
    virtual void visitConstant(const ConstantPtr& node);
private:
    TypePtr evaluateType(const ExpressionPtr& expr);
private:
};


SWIFT_NS_END


#endif//TYPE_INFERENCE_ACTION_H