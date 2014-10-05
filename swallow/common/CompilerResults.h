#ifndef ERROR_LIST_H
#define ERROR_LIST_H
#include "swift_conf.h"
#include "swift_types.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN

struct ErrorLevel
{
    enum T
    {
        Fatal,
        Error,
        Warning,
        Note
    };
};
typedef std::vector<std::wstring> ResultItems;
struct CompilerResult : SourceInfo
{
    ErrorLevel::T level;
    int code;
    ResultItems items;

    
    CompilerResult(ErrorLevel::T level, const SourceInfo& sourceInfo, int code, const ResultItems& items)
    :level(level), code(code), items(items)
    {
        this->fileHash = sourceInfo.fileHash;
        this->line = sourceInfo.line;
        this->column = sourceInfo.column;
    }
};

class CompilerResults
{
public:
    void clear();
    int numResults();
    const CompilerResult& getResult(int i);
    void add(ErrorLevel::T level, const SourceInfo&, int code, const ResultItems& items);
    void add(ErrorLevel::T level, const SourceInfo&, int code, const std::wstring& item = std::wstring());
    std::wstring format(const CompilerResult& result);
private:
    std::wstring getErrorTemplate(int errorCode);
private:
    std::vector<CompilerResult> results;
};

SWIFT_NS_END

#endif//ERROR_LIST_H
