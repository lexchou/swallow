#include "semantic-node-visitor.h"
#include "swift_types.h"
#include "common/compiler_results.h"
#include "ast/node.h"
#include "symbol-registry.h"
#include "type.h"
#include "ast/type-node.h"
#include "ast/type-identifier.h"
#include "ast/tuple-type.h"
#include "ast/function-type.h"
#include "ast/array-type.h"
#include "swift_errors.h"


USE_SWIFT_NS


/**
* Abort the visitor
*/
void SemanticNodeVisitor::abort()
{
    throw Abort();
}

/**
 * Outputs an compiler error
 */
void SemanticNodeVisitor::error(const NodePtr& node, int code, const std::wstring& item)
{
    compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), code, item);
    abort();
}
void SemanticNodeVisitor::warning(const NodePtr& node, int code, const std::wstring& item)
{
    compilerResults->add(ErrorLevel::Warning, *node->getSourceInfo(), code, item);
}


TypePtr SemanticNodeVisitor::lookupType(const TypeNodePtr& type)
{
    if(TypeIdentifierPtr id = std::dynamic_pointer_cast<TypeIdentifier>(type))
    {
        //TODO: make a generic type if possible
        TypePtr ret = symbolRegistry->lookupType(id->getName());
        if(!ret)
        {
            std::wstringstream out;
            type->serialize(out);
            error(type, Errors::E_USE_OF_UNDECLARED_TYPE, out.str());
            abort();
        }
        return ret;
    }
    if(TupleTypePtr tuple = std::dynamic_pointer_cast<TupleType>(type))
    {
        std::vector<TypePtr> elementTypes;
        for(const TupleType::TupleElement& e : *tuple)
        {
            TypePtr t = lookupType(e.type);
            elementTypes.push_back(t);
        }
        return Type::newTuple(elementTypes);
    }
    if(ArrayTypePtr array = std::dynamic_pointer_cast<ArrayType>(type))
    {
        TypePtr innerType = lookupType(array->getInnerType());
        return Type::newArrayType(innerType);
    }
    if(FunctionTypePtr func = std::dynamic_pointer_cast<FunctionType>(type))
    {
        TypePtr retType = nullptr;
        if(func->getReturnType())
        {
            retType = lookupType(func->getReturnType());
        }
    }
    return nullptr;
}
