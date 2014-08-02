#include "semantic-node-visitor.h"
#include "swift_types.h"
USE_SWIFT_NS


/**
* Abort the visitor
*/
void SemanticNodeVisitor::abort()
{
    throw Abort();
}
