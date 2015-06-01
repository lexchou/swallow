/* GenericDefinition.h --
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
#ifndef GENERIC_DEFINITION_H
#define GENERIC_DEFINITION_H

#include "swallow_conf.h"
#include <vector>
#include <map>
#include <string>
#include <list>

SWALLOW_NS_BEGIN

typedef std::shared_ptr<class Type> TypePtr;
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
class SymbolScope;
class SWALLOW_EXPORT GenericDefinition : public  std::enable_shared_from_this<GenericDefinition>
{
    friend class GenericArgument;
public:
    struct NodeDef;
    typedef std::shared_ptr<NodeDef> NodeDefPtr;
    typedef std::map<std::wstring, NodeDefPtr> TypeConstraintMap;
    enum ConstraintType
    {
        EqualsTo,
        AssignableTo
    };
    struct Constraint
    {
        ConstraintType type;
        TypePtr reference;
        Constraint(ConstraintType type, const TypePtr& reference)
            :type(type), reference(reference)
        {}
    };
    typedef std::list<Constraint> Constraints;
    struct NodeDef
    {
        TypePtr type;
        int index;
        TypeConstraintMap children;
        Constraints constraints;
    };
    struct Parameter
    {
        int index;
        std::wstring name;
        TypePtr type;
        Parameter(int index, const std::wstring& name, const TypePtr& type)
            :index(index), name(name), type(type)
        {}
    };
public:
    GenericDefinition(const GenericDefinitionPtr& parent);
public:
    void add(const std::wstring& alias, const TypePtr& type);
    void addConstraint(const std::list<std::wstring>& type, ConstraintType constraint, const TypePtr& referenceType);
    void registerTo(SymbolScope* scope);
    TypePtr get(const std::wstring& alias);
    NodeDefPtr getConstraint(const std::wstring& name);

    /*!
     * The number of all parameters defined in current definition
     */
    size_t numParameters() const;
    /*!
     * The number of all parameters defined, including the parent definition's parameters
     */
    size_t totalParameters() const;
    bool equals(const GenericDefinitionPtr& rhs) const;
    int validate(const std::wstring& typeName, const TypePtr& typeToTest, TypePtr& expectedType) const;

    const std::vector<Parameter> getParameters()const;

    GenericDefinitionPtr getParent();

    /*!
     * Gets the nested definition depth, the depth is 0 if it has no parent definition
     */
    int getDepth() const { return depth;}

    const std::vector<Parameter>::const_iterator begin() const {return typeParameters.begin();}
    const std::vector<Parameter>::const_iterator end() const {return typeParameters.end();}

public:
    static bool equals(const GenericDefinitionPtr& a, const GenericDefinitionPtr& b);
private:
    int validate(const NodeDefPtr& node, const TypePtr& typeToTest, TypePtr& expectedType) const;
private:
    std::vector<Parameter> typeParameters;
    TypeConstraintMap constraints;
    /*!
     * This is used for a nested generic definition. example:
     * struct GStruct<T>
     * {
     *     func map<U>(f: (T) -> U) -> U?
     *     {
     *         return nil;
     *     }
     * }
     */
    GenericDefinitionPtr parent;
    int depth;
};

SWALLOW_NS_END


#endif//GENERIC_DEFINITION_H
