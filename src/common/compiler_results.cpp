#include "compiler_results.h"
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
void CompilerResults::add(ErrorLevel::T level, const SourceInfo& sourceInfo, int code, const std::wstring& item)
{
    results.push_back(CompilerResult(level, sourceInfo, code, item));
}
