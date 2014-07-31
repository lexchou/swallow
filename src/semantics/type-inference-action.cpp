#include <semantics/symbol-registry.h>
#include <common/compiler_results.h>
#include <swift_errors.h>
#include "type-inference-action.h"
#include "ast/variables.h"
#include "ast/constant.h"
#include "ast/identifier.h"
#include "ast/tuple.h"
#include "scoped-nodes.h"
#include "ast/type-node.h"
USE_SWIFT_NS


TypeInferenceAction::TypeInferenceAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :SemanticNodeVisitor(symbolRegistry, compilerResults)
{

}

void TypeInferenceAction::visitVariable(const VariablePtr& node)
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
    else if(TuplePtr id = std::dynamic_pointer_cast<Tuple>(node->name))
    {
        TypeNodePtr declaredType = id->getDeclaredType();
        if(declaredType)
        {
            //this is a tuple definition, all elements must be matched to this
            TypePtr type = symbolRegistry->lookupType(declaredType);
            if(!type)
            {
                std::wstringstream out;
                declaredType->serialize(out);
                compilerResults->add(ErrorLevel::Error, *declaredType->getSourceInfo(), Errors::E_USE_OF_UNDECLARED_TYPE, out.str());
                return;
            }
            

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