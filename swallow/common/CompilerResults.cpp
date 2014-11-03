#include "CompilerResults.h"
#include "Errors.h"
USE_SWIFT_NS;


void CompilerResults::clear()
{
    results.clear();
}
int CompilerResults::numResults()
{
    return results.size();
}
const CompilerResult& CompilerResults::getResult(int i)
{
    return results[i];
}
void CompilerResults::add(ErrorLevel::T level, const SourceInfo& sourceInfo, int code, const ResultItems& items)
{
    results.push_back(CompilerResult(level, sourceInfo, code, items));
}
void CompilerResults::add(ErrorLevel::T level, const SourceInfo& sourceInfo, int code, const std::wstring& item)
{
    ResultItems items;
    items.push_back(item);
    add(level, sourceInfo, code, items);
}
