/* GlobalScope.h --
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
#ifndef GLOBAL_SCOPE_H
#define GLOBAL_SCOPE_H
#include "SymbolScope.h"
#include <vector>
SWALLOW_NS_BEGIN

class SWALLOW_EXPORT GlobalScope : public SymbolScope
{
public:
    GlobalScope();
public:
    void initRuntime(SymbolRegistry* symbolRegistry);
private:
    void initPrimitiveTypes();
    void initOperators(SymbolRegistry* symbolRegistry);
    void initProtocols();

    /*!
     * Register the implementation of binary operator
     */
    bool registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& lhs, const TypePtr& rhs);
    bool registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& operand, OperatorType::T operatorType);
    bool registerFunction(const std::wstring& name, const FunctionSymbolPtr& func);
public:
    /*!
     * A short-hand way to declare an external function.
     */
    void declareFunction(const std::wstring&name, int flags, const wchar_t* returnType, ...);

    /*!
     * A short-hand way to create a function symbol
     */
    FunctionSymbolPtr createFunction(const std::wstring&name, int flags, const wchar_t* returnType, ...);

    /*!
     * A short-hand way to create an Array type
     */
    TypePtr makeArray(const TypePtr& elementType) const;

    /*!
     * A short-hand way to create an Optional type
     */
    TypePtr makeOptional(const TypePtr& elementType) const;

    /*!
     * A short-hand way to create an implicitly unwrapped Optional type
     */
    TypePtr makeImplicitlyUnwrappedOptional(const TypePtr& elementType) const;

    /*!
     * A short-hand way to create a Dictionary type
     */
    TypePtr makeDictionary(const TypePtr& keyType, const TypePtr& valueType) const;
public://Generic testing
    bool isArray(const TypePtr& type) const;
    bool isOptional(const TypePtr& type) const;
    bool isImplicitlyUnwrappedOptional(const TypePtr& type) const;
    bool isDictionary(const TypePtr& type) const;
private:
    FunctionSymbolPtr vcreateFunction(const std::wstring&name, int flags, const wchar_t* returnType, va_list va);
public:
    SymbolPtr True;
    SymbolPtr False;

    TypePtr getModuleType() const;


    TypePtr Bool() const;
    TypePtr Void() const;
    TypePtr String() const;
    TypePtr Character() const;
    TypePtr Int() const;
    TypePtr UInt() const;
    TypePtr Int8() const;
    TypePtr UInt8() const;
    TypePtr Int16() const;
    TypePtr UInt16() const;
    TypePtr Int32() const;
    TypePtr UInt32() const;
    TypePtr Int64() const;
    TypePtr UInt64() const;
    TypePtr Float() const;
    TypePtr Float80() const;
    TypePtr Double() const;
    TypePtr Array() const;
    TypePtr Dictionary() const;
    TypePtr Optional() const;
    TypePtr ImplicitlyUnwrappedOptional() const;
    TypePtr BooleanType() const;
    TypePtr Equatable() const;
    TypePtr Comparable() const;
    TypePtr Hashable() const;
    TypePtr RawRepresentable() const;
    TypePtr _OptionalNilComparisonType() const;
    TypePtr _IntegerType() const;
    TypePtr UnsignedIntegerType() const;
    TypePtr SignedIntegerType() const;
    TypePtr FloatingPointType() const;
    TypePtr StringInterpolationConvertible() const;
    TypePtr IntegerLiteralConvertible() const;
    TypePtr BooleanLiteralConvertible() const;
    TypePtr StringLiteralConvertible() const;
    TypePtr FloatLiteralConvertible() const;
    TypePtr NilLiteralConvertible() const;
    TypePtr ArrayLiteralConvertible() const;
    TypePtr DictionaryLiteralConvertible() const;
    TypePtr UnicodeScalarLiteralConvertible() const;
    TypePtr ExtendedGraphemeClusterLiteralConvertible() const;
    TypePtr Printable() const;
    TypePtr DebugPrintable() const;
    TypePtr SequenceType() const;
    TypePtr CollectionType() const;
private:

    TypePtr _Bool;
    TypePtr _Void;
    TypePtr _String;
    TypePtr _Character;
    TypePtr _Int;
    TypePtr _UInt;
    TypePtr _Int8;
    TypePtr _UInt8;
    TypePtr _Int16;
    TypePtr _UInt16;
    TypePtr _Int32;
    TypePtr _UInt32;
    TypePtr _Int64;
    TypePtr _UInt64;
    TypePtr _Float;
    TypePtr _Float80;
    TypePtr _Double;
    TypePtr _Array;
    TypePtr _Dictionary;
    TypePtr _Optional;
    TypePtr _ImplicitlyUnwrappedOptional;
    //Common used protocols
    TypePtr _BooleanType;
    TypePtr _Equatable;
    TypePtr _Comparable;
    TypePtr _Hashable;
    TypePtr _RawRepresentable;
    TypePtr __OptionalNilComparisonType;
    TypePtr __IntegerType;
    TypePtr _UnsignedIntegerType;
    TypePtr _SignedIntegerType;
    TypePtr _FloatingPointType;
    TypePtr _StringInterpolationConvertible;
    TypePtr _IntegerLiteralConvertible;
    TypePtr _BooleanLiteralConvertible;
    TypePtr _StringLiteralConvertible;
    TypePtr _FloatLiteralConvertible;
    TypePtr _NilLiteralConvertible;
    TypePtr _ArrayLiteralConvertible;
    TypePtr _DictionaryLiteralConvertible;
    TypePtr _UnicodeScalarLiteralConvertible;
    TypePtr _ExtendedGraphemeClusterLiteralConvertible;
    TypePtr _Printable;
    TypePtr _DebugPrintable;

    TypePtr _SequenceType;
    TypePtr _CollectionType;
    TypePtr module;
    std::vector<TypePtr> t_Numbers;
    std::vector<TypePtr> t_Ints;

};

SWALLOW_NS_END
#endif//GLOBAL_SCOPE_H
