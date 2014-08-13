#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H
#include "swift_conf.h"
#include <memory>

SWIFT_NS_BEGIN

class SymbolScope;
//typedef std::shared_ptr<SymbolScope> SymbolScopePtr;
class Type;
typedef std::shared_ptr<Type> TypePtr;
class Symbol;
typedef std::shared_ptr<Symbol> SymbolPtr;
class SymbolRegistry;
class ScopeOwner;
typedef std::shared_ptr<ScopeOwner> ScopeOwnerPtr;
class ScopedCodeBlock;
typedef std::shared_ptr<ScopedCodeBlock> ScopedCodeBlockPtr;
class ScopedProgram;
typedef std::shared_ptr<ScopedProgram> ScopedProgramPtr;
class FunctionOverloadedSymbol;
typedef std::shared_ptr<FunctionOverloadedSymbol> FunctionOverloadedSymbolPtr;
class FunctionSymbol;
typedef std::shared_ptr<FunctionSymbol> FunctionSymbolPtr;

class SymboledFunction;
typedef std::shared_ptr<SymboledFunction> SymboledFunctionPtr;

class ScopedClass;
typedef std::shared_ptr<ScopedClass> ScopedClassPtr;

typedef std::shared_ptr<class SymboledVariable> SymboledVariablePtr;
typedef std::shared_ptr<class SymboledConstant> SymboledConstantPtr;


SWIFT_NS_END

#endif//SEMANTIC_TYPES_H