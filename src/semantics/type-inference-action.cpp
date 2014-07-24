#include <semantics/symbol-registry.h>
#include "type-inference-action.h"
#include "ast/variables.h"
#include "ast/constant.h"
#include "ast/identifier.h"
USE_SWIFT_NS


TypeInferenceAction::TypeInferenceAction(SymbolRegistry* symbolRegistry)
    :SemanticNodeVisitor(symbolRegistry)
{

}

void TypeInferenceAction::visitVariables(Variables* node)
{
}
void TypeInferenceAction::visitConstant(Constant* node)
{
    TypePtr type = evaluateType(node->initializer);
    if(Identifier* id = dynamic_cast<Identifier*>(node->name))
    {
        symbolRegistry->getCurrentScope()->addSymbol(id->getIdentifier(), id);
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
TypePtr TypeInferenceAction::evaluateType(Expression* expr)
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