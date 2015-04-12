/* Errors.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SWALLOW_ERRORS_H
#define SWALLOW_ERRORS_H
#include "swallow_conf.h"
#include <string>
#include <vector>

SWALLOW_NS_BEGIN

struct SWALLOW_EXPORT Errors
{
    enum
    {
        E_UNEXPECTED_EOF = 0x1000,
        //grammar errors
        E_EXPECT_1,// %0 is expected
        E_UNEXPECTED_1,// Unexpected %0
        E_EXPECT_IDENTIFIER_1,// Identifier expected, but %0 found.
        E_EXPECT_KEYWORD_1,// Keyword %0 expected
        E_EXPECT_EXPRESSION_1, // Expression expected but %0 found.
        E_EXPECT_OPERATOR_1, // Operator expected but %0 found.
        E_EXPECT_INTEGER_PRECEDENCE,// Operator's precedence must be an integer
        E_EXPECT_INIT_SELF_DYNAMICTYPE_IDENTIFIER_1, // init/self/dynamicType or member field is expected, but %0 found.
        E_UNDEFINED_INFIX_OPERATOR_1, // Undefined infix operator %0
        E_EXPECT_CAPTURE_SPECIFIER, // The capture specifier is not specified.
        E_EXPECT_CASE, // case/default is expected in switch/case statement
        E_GETTER_SETTER_CAN_ONLY_BE_DEFINED_FOR_A_SINGLE_VARIABLE, // Getter/setter can only be defined for a single variable
        E_UNTERMINATED_STRING_LITERAL, //Unterminated string literal
        E_UNEXPECTED_CHARACTER_A_IN_STRING_INTERPOLATION,// Unexpected '%0' character in string interpolation
        E_INVALID_ESCAPE_SEQUENCE_IN_LITERAL,
        E_COMPUTED_PROPERTY_CANNOT_BE_DECLARED_UNDER_FOR_LOOP, //Computed property cannot be declared under for loop

        //semantic errors
        E_INVALID_REDECLARATION_1, // Invalid redeclaration of type %0
        E_USE_OF_UNDECLARED_TYPE_1, // Use of undeclared type %0
        E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION, // cannot assign to the result of this expression
        E_CANNOT_ASSIGN_TO_A_IN_B_2,//Cannot assign to '%0' in '%1'
        E_CANNOT_ASSIGN_TO_A_IN_A_METHOD_1,//Cannot assign to '%0' in a method
        E_CANNOT_ASSIGN_TO_LET_VALUE_A_1,//Cannot assign to 'let' value '%0'
        E_USE_OF_UNRESOLVED_IDENTIFIER_1, //use of unresolved identifier '%0'
        E_USE_OF_UNINITIALIZED_VARIABLE_1, //use of local variable '%0' before its declaration
        E_USE_OF_INITIALIZING_VARIABLE, //variable used within its own initial value
        E_DEFINITION_CONFLICT, //definition conflicts with previous value
        E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, //use of function local variable inside type declaration

        E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1,//tuple pattern cannot match values of the non-tuple type '%0'
        E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT_4, //tuple types '%0' and '%1' have a different number of elements (%2 vs. %3)
        E_OPERATOR_REDECLARED,//operator redeclared
        E_OPERATOR_PRECEDENCE_OUT_OF_RANGE,//'precedence' must be in the range of 0 to 255
        E_UNKNOWN_BINARY_OPERATOR_1, //operator '%0' is not a known binary operator
        E_IS_NOT_BINARY_OPERATOR_1, //'%0' is not a binary operator
        E_NO_OVERLOAD_ACCEPTS_ARGUMENTS_1,//could not find an overload for '%0' that accepts the supplied arguments
        E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2,//invalid use of '%0' to call a value of non-function type '%1'
        E_UNMATCHED_PARAMETERS, // Unmatched number of supplied parameters
        E_UNMATCHED_PARAMETER_1, // Unmatched type of parameter %0
        E_NO_MATCHED_OVERLOAD_FOR_A_1, // No matched overload found
        E_MISSING_ARGUMENT_LABEL_IN_CALL_1,// Missing argument label '%0:' in call
        E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1, //Extraneous argument label '%0:' in call
        E_EXTRANEOUS_ARGUMENT, //Extraneous argument in call
        E_INCORRECT_ARGUMENT_LABEL_IN_CALL_HAVE_A_EXPECTED_B_2,//incorrect argument label in call (have 'b:', expected 'a:')
        E_AMBIGUOUS_USE_1,    // Ambiguous use of '%0'
        E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC, // Inout arguments cannot be variadic
        E_DOES_NOT_HAVE_A_MEMBER_2, // '%0' does not have a member named '%1'

        E_CANNOT_CONVERT_EXPRESSION_TYPE_2, //Cannot convert expression's type '%0' to type '%1'
        E_LET_REQUIRES_INITIALIZER, //'let' declarations require an initializer expression
        E_CANNOT_DEFINE_AN_EMPTY_ARRAY_WITHOUT_CONTEXTUAL_TYPE, // cannot define an empty array without contextual type.
        E_CANNOT_DEFINE_AN_EMPTY_DICTIONARY_WITHOUT_CONTEXTUAL_TYPE,// cannot define an empty dictionary without contextual type.
        E_ARRAY_CONTAINS_DIFFERENT_TYPES, // Array contains different types
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, // Type '%0' does not conform to '%1' protocol
        E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE_1, // Superclass '%0' must appear first in the inheritance clause
        E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE_1, //Inheritance from none-protocol, non-class type '%0'
        E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE_1, //Inheritance from none-protocol type '%0'
        E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD, //Default argument not permitted in a protocol method
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3,// Type %0 does not conform to protocol %1, unimplemented function %2
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3, //Type %0 does not conform to protocol %1, unimplemented type %2
        E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1, // Protocol %0 cannot define let constant
        E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1, // Protocol %0's variable must be a computed property
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3, //Type '%0' does not conform to protocol '%1', unimplemented property '%2'
        E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3, //Type '%0' does not conform to protocol '%1', unwritable property '%2'
        E_TYPE_ANNOTATION_MISSING_IN_PATTERN, // Type annotation missing in pattern
        E_NESTED_TYPE_IS_NOT_ALLOWED_HERE, // Nested type is not allowed here
        E_GENERIC_TYPE_ARGUMENT_REQUIRED, // Generic type argument required
        E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3, //Generic type '%0' specialized with too many type parameters(got %1, but expected %2)
        E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3, //Generic type '%0' specialized with insufficient type parameters(got %1, but expected %2);
        E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, //Cannot specialize non-generic type '%0'
        E_MULTIPLE_INHERITANCE_FROM_CLASS_2_, //Multiple inheritance from class '%0' and '%1'
        E_IS_NOT_A_MEMBER_OF_2, // %0 is not a member of %1
        E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1,//Same-type requirement makes generic parameter '%' non-generic
        E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1,//Protocol '%0' can only be used as a generic constraint because it has Self or associated type requirements
        E_UNDEFINED_SUBSCRIPT_ACCESS_FOR_1,// Undefined subscript access for '%0'
        E_A_IS_NOT_A_MEMBER_TYPE_OF_B_2, // '%0' is not a member type of '%2'
        E_TYPE_A_NESTED_IN_GENERIC_TYPE_B_IS_NOT_ALLOWED_2, //type '%0' nested in generic type '%1' is not allowed
        E_GENERIC_TYPE_A_NESTED_IN_TYPE_B_IS_NOT_ALLOWED_2, //generic type '%0' nested in type '%1' is not allowed

        E_TUPLE_ACCESS_ONLY_WORKS_FOR_TUPLE_TYPE, //Tuple access only works for tuple type
        E_TUPLE_ACCESS_A_OUT_OF_RANGE_IN_B_2, //Tuple access '%0' out of range in '%1'
        E_VARLET_CANNOT_APPEAR_INSIDE_ANOTHER_VAR_OR_LET_PATTERN_1, //%0 cannot appear inside another var or let pattern
        E_EXPECT_TUPLE_OR_IDENTIFIER, //Expect tuple or identifier
        E_TYPE_ANNOTATION_DOES_NOT_MATCH_CONTEXTUAL_TYPE_A_1, //Type annotation does not match contextual type '%0'
        E_TUPLE_PATTERN_CANNOT_MATCH_VALUES_OF_THE_NON_TUPLE_TYPE_A_1, //Tuple pattern cannot match values of the non-tuple type '%0'
        E_A_IS_NOT_CONVERTIBLE_TO_B_2, //'%0' is not convertible to '%1'
        E_NO_CONTEXTUAL_TYPE_TO_ACCESS_MEMBER_A_1, // No contextual type to access member '%0"
        E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, //'%0' label in a 'switch' should have at least one executable statement'
        E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE, //Switch must be exhausive, consider adding a default clause
        E_PARTIAL_APPLICATION_OF_ENUM_CONSTRUCTOR_IS_NOT_ALLOWED, //partial application of enum constructor is not allowed

        E_MULTIPLE_ENUM_RAW_TYPES_A_AND_B_2, //Multiple enum raw types '%0' and '%1'
        E_RAW_TYPE_A_MUST_APPEAR_FIRST_IN_THE_ENUM_INHERITANCE_CLAUSE_1, //Raw type 'Int' must appear first in the enum inheritance clause
        E_RAW_TYPE_A_IS_NOT_CONVERTIBLE_FROM_ANY_LITERAL_1, //Raw type 'TestBase' is not convertible from any literal
        E_RAWREPRESENTABLE_INIT_CANNOT_BE_SYNTHESIZED_BECAUSE_RAW_TYPE_A_IS_NOT_EQUATABLE_1,//RawRepresentable 'init' cannot be synthesized because raw type 'MyInt' is not Equatable
        E_ENUM_WITH_NO_CASES_CANNOT_DECLARE_A_RAW_TYPE,//An enum with no cases cannot declare a raw type
        E_RAW_VALUE_FOR_ENUM_CASE_MUST_BE_LITERAL,//Raw value for enum case must be literal
        E_ENUM_CASES_REQUIRE_EXPLICIT_RAW_VALUES_WHEN_THE_RAW_TYPE_IS_NOT_INTEGER_LITERAL_CONVERTIBLE, //Enum cases require explicit raw values when the raw type is not integer literal convertible
        E_ENUM_WITH_RAW_TYPE_CANNOT_HAVE_CASES_WITH_ARGUMENTS, //Enum with raw type cannot have cases with arguments
        E_ENUM_CASE_CANNOT_HAVE_A_RAW_VALUE_IF_THE_ENUM_DOES_NOT_HAVE_A_RAW_TYPE, //Enum case cannot have a raw value if the enum does not have a raw type
        E_EXPRESSION_DOES_NOT_CONFORM_TO_PROTOCOL_1, // Expression does not conform to protocol '%0'

        E_OPERAND_OF_POSTFIX_A_SHOULD_HAVE_OPTIONAL_TYPE_TYPE_IS_B_2, // Operand of postfix '%0' should have optional type; type is '%1'
        E_BOUND_VALUE_IN_A_CONDITIONAL_BINDING_MUST_BE_OF_OPTIONAL_TYPE, //Bound value in a conditional binding must be of Optional type
        E_EXPECTED_EXPRESSION_VAR_OR_LET_IN_A_CONDITION_1, //Expected expression, var, or let in '%0' condition
        E_VARIABLE_BINDING_IN_A_CONDITION_REQUIRES_AN_INITIALIZER,//Variable binding in a condition requires an initializer
        E_A_IS_NOT_IDENTICIAL_TO_B_2,//'%0' is not identical to '%1'
        E_RETURN_INVALID_OUTSIDE_OF_A_FUNC,//return invalid outside of a func
        E_SUBSCRIPT_ACCESS_ON_A_IS_NOT_WRITABLE_1,// Subscript access on '%0' is not writable
        E_DICTIONARY_KEY_CONTAINS_DIFFERENT_TYPES,// Dictionary key contains different types
        E_DICTIONARY_VALUE_CONTAINS_DIFFERENT_TYPES,// Dictionary key contains different types

        E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION,//Operator implementation without matching operator declaration
        E_A_REQUIRES_A_FUNCTION_WITH_ONE_ARGUMENT_1, //'%0' requires a function with one argument
        E_UNARY_OPERATOR_IMPLEMENTATION_MUST_HAVE_A_PREFIX_OR_POSTFIX_MODIFIER,//Unary operator implementation must have a 'prefix' or 'postfix' modifier
        E_OPERATORS_MUST_HAVE_ONE_OR_TWO_ARGUMENTS,//Operators must have one or two arguments
        E_OPERATOR_MUST_BE_DECLARED_AS_PREFIX_POSTFIX_OR_INFIX,//Operator must be declared as 'prefix', 'postfix', or 'infix'
        E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, //'%0' may only be declared at file scope
        E_CANNOT_OVERRIDE_WITH_A_STORED_PROPERTY_A_1,//Cannot override with a stored property 'a'


        E_GENERIC_ARGUMENTS_ARE_NOT_ALLOWED_ON_AN_EXTENSION,//Generic arguments are not allowed on an extension
        E_PROTOCOL_A_CANNOT_BE_EXTENDED_1,//Protocol '%0' cannot be extended
        E_NON_NOMINAL_TYPE_A_CANNOT_BE_EXTENDED_1,//Non-nominal type '%0' cannot be extended
        E_EXTENSIONS_MAY_NOT_CONTAIN_STORED_PROPERTIES,//Extensions may not contain stored properties
        E_ENUMS_MAY_NOT_CONTAIN_STORED_PROPERTIES,//Enums may not contain stored properties

        E_INIT_CAN_ONLY_REFER_TO_THE_INITIALIZERS_OF_SELF,//'init' can only refer to the initializers of 'self'
        E_INITIALIZER_DELEGATION_CAN_ONLY_OCCUR_WITHIN_AN_INITIALIZER,// Initializer delegation can only occur within an initializer
        E_FUNCTION_PROCEDURES_EXPECTD_TYPE_A_DID_YOU_MEAN_TO_CALL_IT_WITH_1,//function produces expected type '%0'; did you mean to call it with '()'?

        E_LAZY_PROPERTIES_MUST_HAVE_AN_INITIALIZER,//Lazy properties must have an initializer
        E_LAZY_CANNOT_DESTRUCTURE_AN_INITIALIZER,//'lazy' cannot destructure an initializer
        E_LAZY_CANNOT_BE_USED_ON_A_LET,//'lazy' cannot be used on a let
        E_LAZY_IS_ONLY_VALID_FOR_MEMBERS_OF_A_STRUCT_OR_CLASS,//Lazy is only valid for members of a struct or class
        E_LAZY_MAY_NOT_BE_USED_ON_A_COMPUTED_PROPERTY,//'lazy' may not be used on a computed property
        E_CLASS_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE,//Class properties may only be declared on a type
        E_STATIC_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE,//Static properties may only be declared on a type
        E_STATIC_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_STRUCTS_AND_ENUMS,//Static properties are only allowed within structs and enums; use 'class' to declare a class property
        E_CLASS_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_CLASSES_AND_PROTOCOLS,//Class properties are only allowed within classes and protocols; use 'static' to declare a static property

        E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2,//'%0' may only be used on '%1' declaration
        E_A_IS_ONLY_VALID_ON_METHODS_1,//'%0' is only valid on methods
        E_A_ISNT_VALID_ON_METHODS_IN_CLASSES_OR_CLASS_BOUND_PROTOCOLS,//'%0' isn't valid on methods in classes or class-bound protocols
        E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1,//Static functions may not be declared %0
        E_METHOD_MAY_NOT_BE_DECLARED_BOTH_MUTATING_AND_NONMUTATING,//Method may not be declared both mutating and nonmutating
        E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2,//Immutable value of type '%0' only has mutating members named '%1'
        E_A_CAN_ONLY_BE_SPECIFIED_ON_CLASS_MEMBERS,//'%0' can only be specified on class members
        E_METHOD_DOES_NOT_OVERRIDE_ANY_METHOD_FROM_ITS_SUPERCLASS,//Method does not override any method from its superclass
        E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD,// Overriding declaration requires an 'override' keyword
        E_DECLARATIONS_IN_EXTENSIONS_CANNOT_OVERRIDE_YET,//Declarations in extensions cannot override yet
        E_PROPERTY_DOES_NOT_OVERRIDE_ANY_PROPERTY_FROM_ITS_SUPERCLASS,//Property does not override any property from its superclass
        E_PROPERTY_A_WITH_TYPE_B_CANNOT_OVERRIDE_A_PROPERTY_WITH_TYPE_C_3,//Property '%0' with type '%1' cannot override a property with type '%2'
        E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1,//Cannot override mutable property with read-only property '%0'
        E_SUBSCRIPT_DECLARATIONS_MUST_HAVE_A_GETTER,//Subscript declarations must have a getter
        E_SUBSCRIPT_DOES_NOT_OVERRIDE_ANY_SUBSCRIPT_FROM_ITS_SUPERCLASS,//Subscript does not override any subscript from its superclass
        E_INOUT_IS_ONLY_VALID_IN_PARAMTER_LISTS,//'inout' is only valid in parameter lists
        E_INHERITANCE_FROM_A_FINAL_CLASS_A_1,//Inheritance from a final class '%0'
        E_A_MODIFIER_CANNOT_BE_APPLIED_TO_THIS_DECLARATION_1,//'%0' modifier cannot be applied to this declaration
        E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL,//Only classes and class members may be marked with 'final'
        E_VAR_OVERRIDES_A_FINAL_VAR,//Var overrides a 'final' var
        E_INSTANCE_METHOD_OVERRIDES_A_FINAL_INSTANCE_METHOD,//Instance method overrides a 'final' instance method
        E_SUBSCRIPT_OVERRIDES_A_FINAL_SUBSCRIPT,//Subscript overrides a 'final' subscript
        E_SUPER_INIT_CANNOT_BE_CALLED_OUTSIDE_OF_AN_INITIALIZER,//'super.init' cannot be called outside of an initializer
        E_SUPER_INIT_ISNT_CALLED_BEFORE_RETURNING_FROM_INITIALIZER,//Super.init isn't called before returning from initializer
        E_SELF_INIT_ISNT_CALLED_ON_ALL_PATHS_IN_DELEGATING_INITIALIZER,//Self.init isn't called on all paths in delegating initializer
        E_SELF_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER,//Self.init called multiple times in initializer
        E_DESIGNATED_INITIALIZER_FOR_A_CANNOT_DELEGATE_1,//Designated initializer for 'Base' cannot delegate(with 'self.init'); did you mean this to be a convenience initializer?
        E_SUPER_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER,//Self.init called multiple times in initializer
        E_MUST_CALL_A_DESIGNATED_INITIALIZER_OF_THE_SUPER_CLASS_A_1,//Must call a designated initializer of the superclass '%0'
        E_CONVENIENCE_INITIALIZER_FOR_A_MUST_DELEGATE_WITH_SELF_INIT_1,//Convenience initializer for '%0' must delegate(with 'self.init') rather than chaining to a superclass initializer(with 'super.init')
        E_MISSING_RETURN_IN_A_FUNCTION_EXPECTED_TO_RETURN_A_1,//Missing return in a function expected to return '%0'
        E_PROPERTY_A_NOT_INITIALIZED_AT_SUPER_INIT_CALL_1,//Property 'self.a' not initialized at super.init call
        E_VARIABLE_A_USED_BEFORE_BEING_INITIALIZED_1,//Variable 'a' used before being initialized
        E_USE_OF_PROPERTY_A_IN_BASE_OBJECT_BEFORE_SUPER_INIT_INITIALIZES_IT,//Use of property '%0' in base object before super.init initializes it
        E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED,//Use of 'self' in delegating initializer before self.init is called
        E_SELF_USED_BEFORE_SUPER_INIT_CALL,//'self' used before super.init call
        E_PROPERTY_A_NOT_INITIALIZED,//Property '%0' not initialized
        E_NIL_IS_THE_ONLY_RETURN_VALUE_PERMITTED_IN_AN_INITIALIZER,//'nil' is the only return value permitted in an initializer
        E_ONLY_A_FAILABLE_INITIALIZER_CAN_RETURN_NIL,//Only a failable initializer can return 'nil'
        E_ALL_STORED_PROPERTIES_OF_A_CLASS_MUST_BE_INITIALIZED_BEFORE_RETURNING_NIL,//All stored properties of a class instance must be initialized before returning nil from an initializer
        E_A_NON_FAILABLE_INITIALIZER_CANNOT_CHAINING_TO_FAILABLE_INITIALIZER_A_WRITTEN_WITH_INIT_1,//A non-failable initializer cannot chaining to failable initializer 'init(name:)' written with 'init?'
        E_REQUIRED_INITIALIZER_IN_NON_CLASS_TYPE_A_1,//'required' initializer in non-class type '%0'
        E_REQUIRED_MODIFIER_MUST_BE_PRESENT_ON_ALL_OVERRIDES_OF_A_REQUIRED_INITIALIZER,//'required' modifier must be present on all overrides of a required initializer

        E_DUPLICATE_MODIFIER,//Duplicate modifier
        E_NON_PROTOCOL_TYPE_A_CANNOT_BE_USED_WITHIN_PROTOCOL_COMPOSITION_1,//Non-protocol type '%0' cannot be used within 'protocol<...>'


        E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4,//Method must be declared private because its result uses a private type
        E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4,//Property cannot be declared public because its type uses a private type











        W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY_1, // '%0 %0' can be expressed more succinctly as '#%0'
        W_EXTRANEOUS_SHARTP_IN_PARAMETER_1, //Extraneous '#' in parameter: '%0' is already the keyword argument name
        W_CODE_AFTER_A_WILL_NEVER_BE_EXECUTED_1,//Code after 'return' will never be executed
        //linking errors
    };
    static std::wstring format(int code, const std::vector<std::wstring>& items);
    static std::wstring getErrorTemplate(int errorCode);
};


SWALLOW_NS_END

#endif//SWALLOW_ERRORS_H
