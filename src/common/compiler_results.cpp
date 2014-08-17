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
            return L"definition '%' conflicts with previous value";
        case Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE:
            return L"use of function local variable inside type declaration";
        case Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE:
            return L"tuple pattern cannot match values of the non-tuple type '%'";
        case Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT:
            return L"tuple types '($T0, $T1)' and '($T2, $T3, $T4)' have a different number of elements (2 vs. 3)";
        case Errors::E_OPERATOR_REDECLARED:
            return L"operator redeclared";
        case Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE:
            return L"'precedence' must be in the range of 0 to 255";
        case Errors::E_UNKNOWN_BINARY_OPERATOR:
            return L"operator is not a known binary operator";
        case Errors::E_IS_NOT_BINARY_OPERATOR:
            return L"'%' is not a binary operator";
        case Errors::E_NO_OVERLOAD_ACCEPTS_ARGUMENTS:
            return L"could not find an overload for '%' that accepts the supplied arguments";
        case Errors::E_INVALID_CALL_OF_NON_FUNCTION_TYPE:
            return L"invalid use of '()' to call a value of non-function type '%'";
        case Errors::E_UNMATCHED_PARAMETERS:
            return L"Unmatched number of supplied parameters";
        case Errors::E_UNMATCHED_PARAMETER:
            return L"Unmatched type of parameter #%";
        case Errors::E_NO_MATCHED_OVERLOAD:
            return L"No matched overload found";
        case Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL:
            return L"Missing argument label '%:' in call";
        case Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL:
            return L"Extraneous argument label '%:' in call";
        case Errors::E_EXTRANEOUS_ARGUMENT:
            return L"Extraneous argument in call";
        case Errors::E_AMBIGUOUS_USE:
            return L"Ambiguous use of '%'";
        case Errors::E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC:
            return L"Inout arguments cannot be variadic";
        case Errors::E_DOES_NOT_HAVE_A_MEMBER:
            return L"'XXX' does not have a member named '%'";
        case Errors::W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY:
            return L"'% %' can be expressed more succinctly as '#%'";
        case Errors::W_EXTRANEOUS_SHARTP_IN_PARAMETER:
            return L"Extraneous '#' in parameter: '%' is already the keyword argument name";

        default:
            return L"Unknown error";
    }

}