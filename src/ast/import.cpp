
#include "import.h"
#include "node-visitor.h"
USE_SWIFT_NS


Import::Import()
    :Declaration(NodeType::Import), kind(_)
{
}

void Import::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitImport);
}

Import::Kind Import::getKind()const
{
    return kind;
}
void Import::setKind(Kind kind)
{
    this->kind = kind;
}
const std::wstring& Import::getPath()const
{
    return path;
}
void Import::setPath(const std::wstring& path)
{
    this->path = path;
}