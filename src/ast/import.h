#ifndef IMPORT_H
#define IMPORT_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Import : public Declaration
{
public:
    enum Kind
    {
        _,
        Typealias,
        Struct,
        Class,
        Enum,
        Protocol,
        Var,
        Func,
    };
    
public:
    Import();
public:
    Kind getKind()const;
    void setKind(Kind kind);
    const std::wstring& getPath()const;
    void setPath(const std::wstring& path);
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring path;
    Kind kind;
};

SWIFT_NS_END

#endif//IMPORT_H
