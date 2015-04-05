#include "codegen/NameMangling.h"
#include "semantics/Symbol.h"
#include "semantics/Type.h"
#include "semantics/FunctionSymbol.h"
#include <cassert>
#include "semantics/SymbolRegistry.h"
#include "semantics/GlobalScope.h"
#include "common/SwallowUtils.h"
#include "semantics/GenericArgument.h"
#include "3rdparty/md5.h"

USE_SWALLOW_NS
using namespace std;
NameMangling::NameMangling(SymbolRegistry *registry)
{
    GlobalScope* g = registry->getGlobalScope();
    defineAbbreviation(g->UInt(), L"Su");
    defineAbbreviation(g->Int(), L"Si");
    defineAbbreviation(g->Bool(), L"Sb");
    defineAbbreviation(g->Float(), L"Sf");
    defineAbbreviation(g->Double(), L"Sd");
    defineAbbreviation(g->String(), L"SS");
    defineAbbreviation(g->Array(), L"Sa");
    //defineAbbreviation(g->UnicodeScalar(), L"Sc");
    //defineAbbreviation(g->ObjectiveC(), L"So");
    defineAbbreviation(g->Optional(), L"Sq");
    defineAbbreviation(g->ImplicitlyUnwrappedOptional(), L"SQ");

}
NameMangling::~NameMangling()
{

}

void NameMangling::defineAbbreviation(const TypePtr& type, const std::wstring& abbrev)
{
    nameToType.insert(make_pair(abbrev, type));
    typeToName.insert(make_pair(type, abbrev));
}
/*!
 * \brief Decode a mangled name into a symbol
 * \param name
 * \return nullptr if failed to decode it
 */
SymbolPtr NameMangling::decode(const wchar_t* name)
{
    return nullptr;

}


void NameMangling::encodeName(wstringstream& out, const wchar_t* name)
{
    const wchar_t* p = name;
    while(p && *p)
    {
        const wchar_t* next = std::wcschr(p, (wchar_t)'.');
        int len = 0;
        if(next)
            len = next - p;
        else
            len = wcslen(p);
        out<<len;
        for(int i = 0; i < len; i++)
        {
            out<<p[i];
        }
        p = next;
    }
}
void NameMangling::encodeName(wstringstream& out, const wstring& name)
{
    encodeName(out, name.c_str());
}


void NameMangling::encodeType(wstringstream& out, const wstring& moduleName, const wstring& typeName)
{
    if(moduleName == L"Swift")
        out<<L"Ss";
    else
    {
        out << L"S_";
        encodeName(out, moduleName);
    }
    encodeName(out, typeName);
}
void NameMangling::encodeType(wstringstream& out, const TypePtr& type)
{
    //check for abbreviation
    auto iter = typeToName.find(type);
    if(iter != typeToName.end())
    {
        out << iter->second;
        return;
    }
    //no abbreviation
    Type::Category category = type->getCategory();
    switch(category)
    {
        case Type::Specialized:
            out<<L"G";
            encodeType(out, type->getInnerType());
            for(const TypePtr& arg : *type->getGenericArguments())
            {
                encodeType(out, arg);
            }
            out<<L"_";
            break;
        case Type::Tuple:
            out<<L"T";
            for(int i = 0; i < type->numElementTypes(); i++)
            {
                TypePtr element = type->getElementType(i);
                encodeType(out, element);
            }
            out<<L"_";
            break;
        case Type::Enum:
            out<<L"O";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::Protocol:
            out<<L"P";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::Struct:
            out<<L"V";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::Class:
            out<<L"C";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::Function:
            out<<L"FT";
            for(const Parameter& param : type->getParameters())
            {
                if(!param.name.empty())
                    encodeName(out, param.name);
                if(type->hasFlags(SymbolFlagMutating))
                    out<<L"M";
                if(param.inout)
                    out<<L"R";
                encodeType(out, param.type);
            }
            out<<L"_";
            break;
        case Type::ProtocolComposition:
            out<<L"P";
            for(const TypePtr& protocol : type->getProtocols())
            {
                encodeType(out, protocol);
            }
            out<<L"_";
        default:
            assert(0 && "Unsupported type to encode");
            break;
    }
}

/*!
 * Encode a variable
 */
std::wstring NameMangling::encodeVariable(const SymbolPlaceHolderPtr& symbol)
{
    if(symbol->getRole() != SymbolPlaceHolder::R_TOP_LEVEL_VARIABLE)
        return L"";//Non top-level variable will not be encoded.
    wstringstream out;
    out<<L"_T";//mark for swift symbol
    out<<L"v";//mark for variable
    wstring moduleName = L"main";
    encodeName(out, moduleName);
    if(symbol->getAccessLevel() == AccessLevelPrivate)
    {
        out<<L"P33_";
        string s = md5(SwallowUtils::toString(moduleName));
        out<<SwallowUtils::toWString(s);
    }
    encodeName(out, symbol->getName());
    encodeType(out, symbol->getType());
    return out.str();
}

/*!
 * \brief Encode a symbol into a mangled name
 * \param symbol
 * \return
 */
std::wstring NameMangling::encode(const SymbolPtr& symbol)
{
    if(SymbolPlaceHolderPtr sym = dynamic_pointer_cast<SymbolPlaceHolder>(symbol))
    {
        return encodeVariable(sym);
    }

    return L"";
}
