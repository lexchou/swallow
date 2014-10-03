#ifndef GENERIC_DEFINITION_H
#define GENERIC_DEFINITION_H

#include "swift_conf.h"
#include <vector>
#include <map>
#include <string>
#include <list>

SWIFT_NS_BEGIN

typedef std::shared_ptr<class Type> TypePtr;
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
class SymbolScope;
class GenericDefinition
{
public:
    struct NodeDef;
    typedef std::shared_ptr<NodeDef> NodeDefPtr;
    typedef std::map<std::wstring, NodeDefPtr> TypeConstraintMap;
    enum ConstraintType
    {
        EqualsTo,
        DerivedFrom
    };
    struct Constraint
    {
        ConstraintType type;
        TypePtr reference;
        Constraint(ConstraintType type, const TypePtr& reference)
            :type(type), reference(reference)
        {}
    };
    typedef std::list<Constraint> Constraints;
    struct NodeDef
    {
        TypePtr type;
        TypeConstraintMap children;
        Constraints constraints;
    };
public:
    void add(const std::wstring& alias, const TypePtr& type);
    void addConstraint(const std::list<std::wstring>& type, ConstraintType constraint, const TypePtr& referenceType);
    void registerTo(SymbolScope* scope);
    TypePtr get(const std::wstring& alias);
    size_t numParameters() const;
    bool equals(const GenericDefinitionPtr& rhs) const;
    int validate(const std::wstring& typeName, const TypePtr& typeToTest, TypePtr& expectedType) const;

    const std::vector<TypePtr> getParameters()const;
private:
    int validate(const NodeDefPtr& node, const TypePtr& typeToTest, TypePtr& expectedType) const;
private:
    std::vector<TypePtr> typeParameters;
    TypeConstraintMap constraints;
};

SWIFT_NS_END


#endif//GENERIC_DEFINITION_H