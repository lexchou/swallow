#include "CompilerResults.h"
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

std::wstring CompilerResults::format(const CompilerResult& result)
{
    std::wstring temp = getErrorTemplate(result.code);
    while(true)
    {
        int p = temp.find('%');
        if (p == -1)
            break;
        int idx = temp[p + 1] - '0';
        std::wstring str = result.items[idx];
        temp.replace(p, 2, str);
    }
    return temp;
}
std::wstring CompilerResults::getErrorTemplate(int errorCode)
{
    switch(errorCode)
    {
        case Errors::E_EXPECT_1: return L"%0 is expected";
        case Errors::E_UNEXPECTED_1: return L"Unexpected %0";
        case Errors::E_EXPECT_IDENTIFIER_1: return L"Identifier expected, but %0 found.";
        case Errors::E_EXPECT_KEYWORD_1: return L"Keyword %0 expected";
        case Errors::E_EXPECT_EXPRESSION_1: return L"Expression expected but %0 found.";
        case Errors::E_EXPECT_OPERATOR_1: return L"Operator expected but %0 found.";
        case Errors::E_EXPECT_INTEGER_PRECEDENCE: return L"Operator's precedence must be an integer";
        case Errors::E_INVALID_OPERATOR_PRECEDENCE: return L"Operator's precedence must be between [0, 255]";
        case Errors::E_EXPECT_INIT_SELF_DYNAMICTYPE_IDENTIFIER_1: return L"init/self/dynamicType or member field is expected, but %0 found.";
        case Errors::E_UNDEFINED_INFIX_OPERATOR_1: return L"Undefined infix operator %0";
        case Errors::E_EXPECT_CAPTURE_SPECIFIER: return L"The capture specifier is not specified.";
        case Errors::E_EXPECT_CASE: return L"case/default is expected in switch/case statement";
        case Errors::E_GETTER_SETTER_CAN_ONLY_BE_DEFINED_FOR_A_SINGLE_VARIABLE: return L"Getter/setter can only be defined for a single variable";
        case Errors::E_INVALID_REDECLARATION_1: return L"Invalid redeclaration of type %0";
        case Errors::E_USE_OF_UNDECLARED_TYPE_1: return L"Use of undeclared type %0";
        case Errors::E_CANNOT_ASSIGN: return L"cannot assign to the result of this expression";
        case Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1: return L"use of unresolved identifier '%0'";
        case Errors::E_USE_OF_UNINITIALIZED_VARIABLE_1: return L"use of local variable '%0' before its declaration";
        case Errors::E_USE_OF_INITIALIZING_VARIABLE: return L"variable used within its own initial value";
        case Errors::E_DEFINITION_CONFLICT: return L"definition conflicts with previous value";
        case Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE: return L"use of function local variable inside type declaration";
        case Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1: return L"tuple pattern cannot match values of the non-tuple type '%0'";
        case Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT_4: return L"tuple types '%0' and '%1' have a different number of elements (%2 vs. %3)";
        case Errors::E_OPERATOR_REDECLARED: return L"operator redeclared";
        case Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE: return L"'precedence' must be in the range of 0 to 255";
        case Errors::E_UNKNOWN_BINARY_OPERATOR_1: return L"operator '%0' is not a known binary operator";
        case Errors::E_IS_NOT_BINARY_OPERATOR_1: return L"'%0' is not a binary operator";
        case Errors::E_NO_OVERLOAD_ACCEPTS_ARGUMENTS_1: return L"could not find an overload for '%0' that accepts the supplied arguments";
        case Errors::E_INVALID_CALL_OF_NON_FUNCTION_TYPE_2: return L"invalid use of '%0' to call a value of non-function type '%1'";
        case Errors::E_UNMATCHED_PARAMETERS: return L"Unmatched number of supplied parameters";
        case Errors::E_UNMATCHED_PARAMETER_1: return L"Unmatched type of parameter %0";
        case Errors::E_NO_MATCHED_OVERLOAD: return L"No matched overload found";
        case Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL_1: return L"Missing argument label '%0:' in call";
        case Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1: return L"Extraneous argument label '%0:' in call";
        case Errors::E_EXTRANEOUS_ARGUMENT: return L"Extraneous argument in call";
        case Errors::E_AMBIGUOUS_USE_1: return L"Ambiguous use of '%0'";
        case Errors::E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC: return L"Inout arguments cannot be variadic";
        case Errors::E_DOES_NOT_HAVE_A_MEMBER_2: return L"'%0' does not have a member named '%1'";
        case Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2: return L"Cannot convert expression's type '%0' to type '%1'";
        case Errors::E_LET_REQUIRES_INITIALIZER: return L"'let' declarations require an initializer expression";
        case Errors::E_CANNOT_DEFINE_EMPTY_ARRAY_WITHOUT_TYPE: return L"cannot define an empty array without type declaration.";
        case Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES: return L"Array contains different types";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_: return L"Type '%0' does not conform to '%1' protocol";
        case Errors::E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE_1: return L"Superclass '%0' must appear first in the inheritance clause";
        case Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE_1: return L"Inheritance from none-protocol, non-class type '%0'";
        case Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE_1: return L"Inheritance from none-protocol type '%0'";
        case Errors::E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD: return L"Default argument not permitted in a protocol method";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3: return L"Type %0 does not conform to protocol %1, unimplemented function %2";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3: return L"Type %0 does not conform to protocol %1, unimplemented type %2";
        case Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1: return L"Protocol %0 cannot define let constant";
        case Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1: return L"Protocol %0's variable must be a computed property";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3: return L"Type '%0' does not conform to protocol '%1', unimplemented property '%2'";
        case Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3: return L"Type '%0' does not conform to protocol '%1', unwritable property '%2'";
        case Errors::E_TYPE_ANNOTATION_MISSING_IN_PATTERN: return L"Type annotation missing in pattern";
        case Errors::E_NESTED_TYPE_IS_NOT_ALLOWED_HERE: return L"Nested type is not allowed here";
        case Errors::E_GENERIC_TYPE_ARGUMENT_REQUIRED: return L"Generic type argument required";
        case Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3: return L"Generic type '%0' specialized with too many type parameters(got %1, but expected %2)";
        case Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3: return L"Generic type '%0' specialized with insufficient type parameters(got %1, but expected %2);";
        case Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1: return L"Cannot specialize non-generic type '%0'";
        case Errors::E_MULTIPLE_INHERITANCE_FROM_CLASS_2_: return L"Multiple inheritance from class '%0' and '%1'";
        case Errors::E_IS_NOT_A_MEMBER_OF_2: return L"%0 is not a member of %1";
        case Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1: return L"Same-type requirement makes generic parameter '%' non-generic";
        case Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1: return L"Protocol '%0' can only be used as a generic constraint because it has Self or associated type requirements";
        case Errors::E_UNDEFINED_SUBSCRIPT_ACCESS_FOR_1: return L"Undefined subscript access for '%0'";
        case Errors::W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY_1: return L"'%0 %0' can be expressed more succinctly as '#%0'";
        case Errors::W_EXTRANEOUS_SHARTP_IN_PARAMETER_1: return L"Extraneous '#' in parameter: '%0' is already the keyword argument name";
    }

}
