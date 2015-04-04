#include "codegen/NameMangling.h"
#include "semantics/Symbol.h"
#include "semantics/Type.h"
#include "semantics/FunctionSymbol.h"

USE_SWALLOW_NS
using namespace std;
/*!
 * \brief Decode a mangled name into a symbol
 * \param name
 * \return nullptr if failed to decode it
 */
SymbolPtr NameMangling::decode(SymbolRegistry* registry, const wchar_t* name)
{
    return nullptr;

}

/*!
 * \brief Encode a symbol into a mangled name
 * \param symbol
 * \return
 */
std::wstring NameMangling::encode(SymbolRegistry* registry, const SymbolPtr& symbol)
{
    return L"";
}
