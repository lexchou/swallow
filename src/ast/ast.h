#ifndef AST_H
#define AST_H
#include "unary-operator.h"
#include "binary-operator.h"
#include "in-out-parameter.h"


#include "literal-nodes.h"
#include "integer-literal.h"
#include "float-literal.h"
#include "string-literal.h"
#include "compile-constant.h"

#include "array-literal.h"
#include "dictionary-literal.h"

#include "identifier.h"
#include "comment-node.h"
#include "member-access.h"
#include "subscript.h"


#include "type-check.h"
#include "type-cast.h"
#include "assignment.h"
#include "conditional-operator.h"
#include "parenthesized-expression.h"
#include "function-call.h"
#include "initializer.h"
#include "self-expression.h"
#include "dynamic-type.h"
#include "forced-value.h"
#include "optional-chaining.h"
#include "closure-expression.h"

#include "ast/break-statement.h"
#include "ast/code-block.h"
#include "ast/continue-statement.h"
#include "ast/do-loop.h"
#include "ast/fallthrough-statement.h"
#include "ast/for-loop.h"
#include "ast/if-statement.h"
#include "ast/labeled-statement.h"
#include "ast/return-statement.h"
#include "ast/statement.h"
#include "ast/switch-case.h"
#include "ast/case-statement.h"
#include "ast/while-loop.h"

#include "ast/value-binding.h"
#include "ast/tuple.h"

#include "ast/type-node.h"
#include "ast/tuple-type.h"
#include "ast/type-identifier.h"
#include "ast/function-type.h"
#include "ast/array-type.h"
#include "ast/optional-type.h"
#include "ast/implicitly-unwrapped-optional.h"
#include "ast/protocol-composition.h"

#endif//AST_H
