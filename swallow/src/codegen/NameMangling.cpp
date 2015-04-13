#include "codegen/NameMangling.h"
#include "semantics/Symbol.h"
#include "semantics/Type.h"
#include "semantics/FunctionSymbol.h"
#include <cassert>
#include "semantics/SymbolRegistry.h"
#include "semantics/GlobalScope.h"
#include "common/SwallowUtils.h"
#include "semantics/GenericArgument.h"
#include "semantics/GenericDefinition.h"
#include "3rdparty/md5.h"
#include <algorithm>
#include <semantics/CollectionTypeAnalyzer.h>

USE_SWALLOW_NS
using namespace std;

/*!
 * Context data used during encoding/decoding
 */
SWALLOW_NS_BEGIN
struct ManglingContext
{
    wstring currentModule;
    wstringstream& out;
    vector<TypePtr> types;
    vector<wstring> genericParameters;

    ManglingContext(const wstring& module, wstringstream& out)
            :currentModule(module), out(out)
    {

    }
    void saveTypeReference(const TypePtr& type)
    {
        Type::Category c = type->getCategory();
        switch(c)
        {
            case Type::Enum:
            case Type::Protocol:
            case Type::Struct:
            case Type::Class:
            case Type::Extension:
                if(getTypeReference(type) == -1)
                    types.push_back(type);
                break;
            default:
                break;
        }
    }
    int getGenericReference(const wstring& name)
    {
        for(size_t i = 0; i < genericParameters.size(); i++)
        {
            if(genericParameters[i] == name)
                return i;
        }
        return -1;
    }
    int getTypeReference(const TypePtr& t)
    {
        for(size_t i = 0; i < types.size(); i++)
        {
            if(Type::equals(types[i], t))
                return i;
        }
        return -1;
    }
};
SWALLOW_NS_END




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


static void encodeName(wstringstream& out, const wchar_t* name)
{
    const wchar_t* p = name;
    while(p && *p)
    {
        //TODO: Add punycode encoding
        const wchar_t* next = std::wcschr(p, (wchar_t)'.');
        int len = 0;
        if(next)
            len = next - p;
        else
            len = wcslen(p);
        out<<len;
        for(int i = 0; i < len; i++)
        {
            wchar_t c = p[i];
            switch (c)
            {
                case '+': c = 'p'; break;
                case '=': c = 'e'; break;
                case '-': c = 's'; break;
                case '!': c = 'n'; break;
                case '%': c = 'r'; break;
                case '^': c = 'x'; break;
                case '&': c = 'a'; break;
                case '*': c = 'm'; break;
                case '|': c = 'o'; break;
                case '~': c = 't'; break;
                case '/': c = 'd'; break;
                case '<': c = 'l'; break;
                case '>': c = 'g'; break;
                case '?': c = 'q'; break;
            }
            out<<c;
        }
        p = next;
    }
}
static void encodeName(wstringstream& out, const wstring& name)
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

/*!
 * Sort protocol by :
 * 1) Module Name, 'Swift' module has higher priority
 * 2) Type name
 */
static bool sortProtocol(const TypePtr& lhs, const TypePtr& rhs)
{
    bool ls = lhs->getModuleName() == L"Swift";
    bool rs = rhs->getModuleName() == L"Swift";
    if(ls != rs)
    {
        if(ls)
            return true;
        return false;
    }
    int n = lhs->getModuleName().compare(rhs->getModuleName());
    if(n < 0)
        return true;
    else if(n > 0)
        return false;
    return lhs->getName() < rhs->getName();
}
static vector<TypePtr> sortTypes(vector<TypePtr> types)
{
    sort(types.begin(), types.end(), sortProtocol);
    return types;
}
void NameMangling::encodeType(ManglingContext& ctx, const TypePtr& type, bool wrapCollections)
{
    //check for abbreviation
    auto iter = typeToName.find(type);
    wstringstream& out = ctx.out;
    if(iter != typeToName.end())
    {
        out << iter->second;
        return;
    }
    Type::Category category = type->getCategory();
    int idx = ctx.getTypeReference(type);
    //use short syntax to make a reference
    if(idx != -1)
    {
        out <<L"S" <<idx << L"_";
        return;
    }
    ctx.saveTypeReference(type);



    //no abbreviation
    switch(category)
    {
        case Type::Specialized:
            out<<L"G";
            encodeType(ctx, type->getInnerType());
            for(const TypePtr& arg : *type->getGenericArguments())
            {
                encodeType(ctx, arg);
            }
            out<<L"_";
            break;
        case Type::Alias:
        {
            int n = ctx.getGenericReference(type->getName());
            assert(n != -1);
            if(n == 0)
                out<<L"Q_";
            else
                out<<L"Q" << (n - 1) <<L"_";
            break;
        }
        case Type::Tuple:
            if(wrapCollections)
                out<<L"T";
            for(int i = 0; i < type->numElementTypes(); i++)
            {
                TypePtr element = type->getElementType(i);
                encodeType(ctx, element);
            }
            if(wrapCollections)
                out<<L"_";
            break;
        case Type::Enum:
            out<<L"O";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::Protocol:
            if(wrapCollections)
                out<<L"P";
            encodeType(out, type->getModuleName(), type->getName());
            if(wrapCollections)
                out<<L"_";
            break;
        case Type::Struct:
            out<<L"V";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::Class:
            out<<L"C";
            encodeType(out, type->getModuleName(), type->getName());
            break;
        case Type::MetaType:
            out<<L"M";
            encodeType(ctx, type->getInnerType());
            break;
        case Type::Function:
        {
            //There's no need to encode parameters for enum case constructor without associated data
            bool encodeParameters = !(type->hasFlags(SymbolFlagEnumCase) && type->getParameters().empty());
            if (type->hasFlags(SymbolFlagMember))
            {
                if(!encodeParameters)
                    out << L"F";
                else
                    out << L"f";
                if (type->hasFlags(SymbolFlagStatic) || type->hasFlags(SymbolFlagInit))
                    out << L"M";
                if (type->hasFlags(SymbolFlagMutating) && type->getDeclaringType() && type->getDeclaringType()->isValueType())
                    out << L"R";
                encodeType(ctx, type->getDeclaringType());
            }
            if(encodeParameters)
            {
                out << L"F";
                bool ignoreTuple = type->getParameters().size() == 1 && !type->hasFlags(SymbolFlagInit);
                if (!ignoreTuple)
                    out << L"T";
                for (const Parameter &param : type->getParameters())
                {
                    if (!param.name.empty())
                        encodeName(out, param.name);
                    if (type->hasFlags(SymbolFlagMutating))
                        out << L"M";
                    if (param.inout)
                        out << L"R";
                    encodeType(ctx, param.type);
                }
                if (!ignoreTuple)
                    out << L"_";
            }
            if (type->hasFlags(SymbolFlagInit))
            {
                if(type->hasFlags(SymbolFlagImplicitFailableInitializer))
                    out << L"GSQ";
                else if(type->hasFlags(SymbolFlagFailableInitializer))
                    out << L"GSq";
                encodeType(ctx, type->getDeclaringType());
                if(type->hasFlags(SymbolFlagFailableInitializer))
                    out << L"_";
            }
            else
                encodeType(ctx, type->getReturnType());
            break;
        }
        case Type::ProtocolComposition:
            {
                if(wrapCollections)
                    out<<L"P";
                vector<TypePtr> protocols = sortTypes(type->getProtocols());
                for (const TypePtr &protocol : protocols)
                {
                    encodeType(out, protocol->getModuleName(), protocol->getName());
                }
                if(wrapCollections)
                    out << L"_";
            }
            break;
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
    return L"";
}

/*!
 * Encode type's declaring type, all ancestor names will be encoded.
 */
static void encodeDeclaringType(ManglingContext& ctx, const TypePtr& type)
{
    if(type->getDeclaringType())
        encodeDeclaringType(ctx, type->getDeclaringType());
    ctx.saveTypeReference(type);
    encodeName(ctx.out, type->getName());
}
static void encodeTypeKind(wostream& out, const TypePtr& type)
{
    if(type->getDeclaringType())
        encodeTypeKind(out, type->getDeclaringType());
    switch(type->getCategory())
    {
        case Type::Enum:
            out<<L"O";
            break;
        case Type::Struct:
            out<<L"V";
            break;
        case Type::Protocol:
            out<<L"P";
            break;
        case Type::Class:
            out<<L"C";
            break;
        case Type::Extension:
            out<<L"E";
            break;
        default:
            break;
    }
}

/*!
 * \brief Encode a symbol into a mangled name
 * \param symbol
 * \return
 */
std::wstring NameMangling::encode(const SymbolPtr& symbol)
{
    wstring moduleName = L"main";
    wstringstream out;
    ManglingContext context(moduleName, out);
    out<<L"_T";//mark for swift symbol
    if(SymbolPlaceHolderPtr sym = dynamic_pointer_cast<SymbolPlaceHolder>(symbol))
    {
        if(!sym->hasFlags(SymbolFlagStatic) && sym->getRole() != SymbolPlaceHolder::R_TOP_LEVEL_VARIABLE)
            return L"";//Non top-level variable will not be encoded.
        out<<L"v";//mark for variable
    }
    else if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(symbol))
    {
        out<<L"F";//mark for function
    }
    else
        return L"";//unsupported symbol type
    if(symbol->getDeclaringType())
        encodeTypeKind(out, symbol->getDeclaringType());

    encodeName(out, moduleName);
    if(symbol->getDeclaringType())
    {
        if(symbol->getDeclaringType()->getCategory() == Type::Extension)
            encodeType(context, symbol->getDeclaringType()->getInnerType());
        else
            encodeDeclaringType(context, symbol->getDeclaringType());
    }

    wstring symbolName = symbol->getName();
    TypePtr symbolType = symbol->getType();
    if(symbol->hasFlags(SymbolFlagAccessor))
    {
        //it's an accessor of a computed property
        FunctionSymbolPtr accessor = dynamic_pointer_cast<FunctionSymbol>(symbol);
        assert(accessor != nullptr && accessor->getOwnerProperty() != nullptr);
        symbolName = accessor->getOwnerProperty()->getName();
        FunctionRole role = accessor->getRole();
        symbolType = accessor->getOwnerProperty()->getType();
        if(role == FunctionRoleGetter)
            out << L"g";
        else if(role == FunctionRoleSetter)
            out << L"s";
        else if(role == FunctionRoleWillSet)
            out << L"w";
        else if(role == FunctionRoleDidSet)
            out << L"W";
        else
            assert(0 && "Invalid role for a property accessor");
    }
    if(symbol->hasFlags(SymbolFlagOperator))
    {
        out << L"o";
        if(symbol->hasFlags(SymbolFlagInfix))
            out << L"i";
        else if(symbol->hasFlags(SymbolFlagPrefix))
            out << L"p";
        else if(symbol->hasFlags(SymbolFlagPostfix))
            out << L"P";
    }

    if(symbol->getAccessLevel() == AccessLevelPrivate)
    {
        out<<L"P33_";
        string s = md5(SwallowUtils::toString(moduleName));
        out<<SwallowUtils::toWString(s);
    }
    if(symbolType->hasFlags(SymbolFlagAllocatingInit))
        out<<L"C";
    else if(symbolType->hasFlags(SymbolFlagInit))
        out<<L"c";
    else if(symbolType->hasFlags(SymbolFlagDeallocatingInit))
        out << L"D";
    else if(symbolType->hasFlags(SymbolFlagDeinit))
        out << L"d";
    else
        encodeName(out, symbolName);
    if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(symbol))
    {
        GenericDefinitionPtr generic = func->getType()->getGenericDefinition();
        if(generic)
        {
            encodeGeneric(context, generic);
        }
    }

    if(!symbolType->hasFlags(SymbolFlagDeinit))//no symbol type for deinit
        encodeType(context, symbolType);
    //dump references
    #if 0
    int i = 0;
    for(TypePtr t : context.types)
    {
        wcout<<L"S" << (i++) << L"\t" << t->toString() << endl;

    }
    #endif
    return out.str();
}

void NameMangling::encodeGeneric(ManglingContext &context, const GenericDefinitionPtr &def)
{
    context.out<<L"U";
    for(const GenericDefinition::Parameter& param : def->getParameters())
    {
        GenericDefinition::NodeDefPtr node = def->getConstraint(param.name);
        if(node)
        {
            vector<TypePtr> types;
            //collect all constraints for generic parameter
            for(const GenericDefinition::Constraint& constraint : node->constraints)
            {
                if(constraint.type != GenericDefinition::AssignableTo)
                    continue;
                //unpack protocol composition
                if(constraint.reference->getCategory() == Type::ProtocolComposition)
                {
                    for(const TypePtr& protocol : constraint.reference->getProtocols())
                    {
                        types.push_back(protocol);
                    }
                }
                else
                {
                    types.push_back(constraint.reference);
                }
            }
            context.genericParameters.push_back(param.name);
            //then encode them to result stream
            for(const TypePtr& type : sortTypes(types))
            {
                encodeType(context, type, false);
            }
        }
        context.out<<L"_";
    }

    context.out<<L"_";
}
