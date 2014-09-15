#ifndef GLOBAL_SCOPE_H
#define GLOBAL_SCOPE_H
#include "SymbolScope.h"
#include <vector>
SWIFT_NS_BEGIN

class GlobalScope : public SymbolScope
{
public:
    GlobalScope();
private:
    void initPrimitiveTypes();
    void initOperators();
    /**
    * Register the implementation of binary operator
    */
    bool registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& lhs, const TypePtr& rhs);

public:
    SymbolPtr s_True, s_False;

    TypePtr t_Bool, t_Void, t_String;
    TypePtr t_Int, t_UInt, t_Int8, t_UInt8, t_Int16, t_UInt16, t_Int32, t_UInt32, t_Int64, t_UInt64;
    TypePtr t_Float, t_Double;
    TypePtr t_Array, t_Dictionary, t_Optional;
    std::vector<TypePtr> t_Numbers;
    std::vector<TypePtr> t_Ints;


};

SWIFT_NS_END
#endif//GLOBAL_SCOPE_H