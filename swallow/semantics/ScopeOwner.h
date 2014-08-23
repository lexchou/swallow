#ifndef SCOPE_OWNER_H
#define SCOPE_OWNER_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

class SymbolScope;
class ScopeOwner
{
protected:
    ScopeOwner();
    virtual ~ScopeOwner();
public:
    SymbolScope* getScope();

protected:
    SymbolScope* symbolScope;
};


SWIFT_NS_END


#endif//SCOPE_OWNER_H