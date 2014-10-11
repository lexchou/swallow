#ifndef TYPE_BUILDER_H
#define TYPE_BUILDER_H
#include "Type.h"


SWIFT_NS_BEGIN

class TypeBuilder : public Type
{
public://Constructors
    TypeBuilder(Category category);
public:
    void setInitializer(const FunctionOverloadedSymbolPtr& initializer);

    void setParentType(const TypePtr& type);

    /**
    * Adds a protocol that this type conform to
    */
    void addProtocol(const TypePtr& protocol);

    /**
     * Add function's parameter if it's a function type
     */
    void addParameter(const Parameter& param);

    void addMember(const std::wstring& name, const SymbolPtr& member);
    void addMember(const SymbolPtr& symbol);
    void addParentTypesFrom(const TypePtr& type);
    void addParentType(const TypePtr& type, int distance);

};
typedef std::shared_ptr<TypeBuilder> TypeBuilderPtr;


SWIFT_NS_END


#endif//TYPE_BUILDER_H
