#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H
#include "swift_conf.h"

SWIFT_NS_BEGIN
class SymbolRegistry;
class NodeVisitor;
class ScopeOwner;
struct ScopeGuard
{
    ScopeGuard(ScopeOwner* owner, NodeVisitor* visitor);
    ~ScopeGuard();
    SymbolRegistry* symbolRegistry;
};

SWIFT_NS_END


#endif//SCOPE_GUARD_H