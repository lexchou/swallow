#include "compiler_results.h"
#include "swift_errors.h"
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


std::wstring CompilerResults::format(const CompilerResult& result)
{
    std::wstring temp = getErrorTemplate(result.code);
    int p = temp.find('%');
    if(p == -1)
        return temp;
    temp.replace(p, 1, result.item);
    return temp;
}
std::wstring CompilerResults::getErrorTemplate(int errorCode)
{
    switch(errorCode)
    {
        case Errors::E_UNEXPECTED_EOF:
            return L"Unexpected end of file.";
        case Errors::E_EXPECT:
            return L"% is expected";
        case Errors::E_UNEXPECTED:
            return L"Unexpected %";
        case Errors::E_EXPECT_IDENTIFIER:
            return L"Identifier expected, but % found.";
        case Errors::E_EXPECT_KEYWORD:
            return L"Keyword %s expected";
        case Errors::E_EXPECT_EXPRESSION:
            return L"Expression expected but % found.";
        case Errors::E_EXPECT_OPERATOR:
             return L"Operator expected but % found.";
        case Errors::E_EXPECT_INTEGER_PRECEDENCE:
            return L"Operator's precedence must be an integer";
        case Errors::E_INVALID_OPERATOR_PRECEDENCE:
            return L"Operator's precedence must be between [0, 255]";
        case Errors::E_EXPECT_INIT_SELF_DYNAMICTYPE_IDENTIFIER:
            return L"init/self/dynamicType or member field is expected, but % found.";
        case Errors::E_UNDEFINED_INFIX_OPERATOR:
            return L"Undefined infix operator %";
        case Errors::E_EXPECT_CAPTURE_SPECIFIER:
            return L"The capture specifier is not specified.";
        case Errors::E_EXPECT_CASE:
            return L"case/default is expected in switch/case statement";
        case Errors::E_INVALID_REDECLARATION:
            return L"Invalid redeclaration of type %";
        case Errors::E_USE_OF_UNDECLARED_TYPE:
            return L"Use of undeclared type %";
        case Errors::E_CANNOT_ASSIGN:
            return L"cannot assign to the result of this expression";
        case Errors::E_USE_OF_UNRESOLVED_IDENTIFIER:
            return L"use of unresolved identifier '%'";
        case Errors::E_USE_OF_UNINITIALIZED_VARIABLE:
            return L"use of local variable '%' before its declaration";
        case Errors::E_USE_OF_INITIALIZING_VARIABLE:
            return L"variable used within its own initial value";
        case Errors::E_DEFINITION_CONFLICT:
            return L"definition conflicts with previous value";
        case Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE:
            return L"use of function local variable inside type declaration";
        default:
            return L"Unknown error";
    }

}