
#include "import.h"
#include "node-visitor.h"
USE_SWIFT_NS


Import::Import()
    :Declaration(NodeType::Import), kind(_)
{
}
void Import::serialize(std::wostream& out)
{
    out<<L"import ";
    switch(kind)
    {
        case Typealias:
            out<<L"typealias ";
            break;
        case Struct:
            out<<L"struct ";
            break;
        case Class:
            out<<L"class ";
            break;
        case Enum:
            out<<L"enum ";
            break;
        case Protocol:
            out<<L"protocol ";
            break;
        case Var:
            out<<L"var ";
            break;
        case Func:
            out<<L"func ";
            break;
        default:
            break;
    }
    out<<path;
}
void Import::accept(NodeVisitor* visitor)
{
    visitor->visitImport(this);
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