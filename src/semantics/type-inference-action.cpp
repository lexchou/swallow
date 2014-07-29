#include <semantics/symbol-registry.h>
#include "type-inference-action.h"
#include "ast/variables.h"
#include "ast/constant.h"
#include "ast/identifier.h"
#include "scoped-nodes.h"
USE_SWIFT_NS


TypeInferenceAction::TypeInferenceAction(SymbolRegistry* symbolRegistry)
    :SemanticNodeVisitor(symbolRegistry)
{

}

void TypeInferenceAction::visitVariables(const VariablesPtr& node)
{
}

void TypeInferenceAction::visitConstant(const ConstantPtr& node)
{
    TypePtr type = evaluateType(node->initializer);
    if(IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(node->name))
    {
        symbolRegistry->getCurrentScope()->addSymbol(std::static_pointer_cast<SymboledConstant>(node));
        if(id->getDeclaredType() == NULL)
        {
            id->setType(type);
        }
        else
        {
            //check if the type is convertible

        }
    }
}
TypePtr TypeInferenceAction::evaluateType(const ExpressionPtr& expr)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return symbolRegistry->lookupType(L"Int");
        case NodeType::FloatLiteral:
            return symbolRegistry->lookupType(L"Float");
        case NodeType::StringLiteral:
            return symbolRegistry->lookupType(L"String");
        default:
            return nullptr;
    }
}