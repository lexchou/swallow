#include "semantic-node-visitor.h"
#include "swift_types.h"
#include "common/compiler_results.h"
#include "ast/node.h"
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
}