/* ast.h --
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
#ifndef AST_H
#define AST_H
#include "Program.h"
#include "UnaryOperator.h"
#include "BinaryOperator.h"
#include "InOutParameter.h"


#include "IntegerLiteral.h"
#include "FloatLiteral.h"
#include "StringLiteral.h"
#include "CompileConstant.h"

#include "ArrayLiteral.h"
#include "DictionaryLiteral.h"

#include "Identifier.h"
#include "TypedPattern.h"
#include "GenericArgumentDef.h"
#include "Comment.h"
#include "MemberAccess.h"
#include "SubscriptAccess.h"


#include "TypeCheck.h"
#include "TypeCast.h"
#include "Assignment.h"
#include "ConditionalOperator.h"
#include "ParenthesizedExpression.h"
#include "FunctionCall.h"
#include "InitializerReference.h"
#include "SelfExpression.h"
#include "DynamicType.h"
#include "ForcedValue.h"
#include "OptionalChaining.h"
#include "Closure.h"

#include "BreakStatement.h"
#include "CodeBlock.h"
#include "ContinueStatement.h"
#include "DoLoop.h"
#include "FallthroughStatement.h"
#include "ForLoop.h"
#include "ForInLoop.h"
#include "IfStatement.h"
#include "LabeledStatement.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "SwitchCase.h"
#include "CaseStatement.h"
#include "WhileLoop.h"

#include "ValueBindingPattern.h"
#include "Tuple.h"
#include "EnumCasePattern.h"

#include "TypeNode.h"
#include "TupleType.h"
#include "TypeIdentifier.h"
#include "FunctionType.h"
#include "ArrayType.h"
#include "OptionalType.h"
#include "ImplicitlyUnwrappedOptional.h"
#include "ProtocolComposition.h"
#include "Attribute.h"

#include "Declaration.h"
#include "Import.h"
#include "ValueBinding.h"
#include "ValueBindings.h"
#include "ComputedProperty.h"
#include "TypeAlias.h"
#include "FunctionDef.h"
#include "Parameters.h"
#include "Parameter.h"
#include "EnumDef.h"
#include "StructDef.h"
#include "ClassDef.h"
#include "ProtocolDef.h"
#include "InitializerDef.h"
#include "DeinitializerDef.h"
#include "ExtensionDef.h"
#include "SubscriptDef.h"
#include "OperatorDef.h"
#include "GenericParametersDef.h"
#include "GenericConstraintDef.h"
#include "StringInterpolation.h"

#endif//AST_H
