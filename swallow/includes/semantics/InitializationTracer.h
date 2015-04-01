/* InitializationTracer.h --
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
#ifndef INITIALIZATION_TRACER_H
#define INITIALIZATION_TRACER_H
#include "Symbol.h"
#include <set>
#include "SemanticUtils.h"

SWALLOW_NS_BEGIN
    /*!
     * A InitializationTracer will trace symbol initialization in all branches
     * This is different to InitializerValidator that it will perform an in-place
     * symbol initialization tracing during a normal semantic pass.
     * It's used to detect if a symbol is initialized in all possible paths
     */
    class InitializationTracer
    {
        typedef std::shared_ptr<Symbol> SymbolPtr;
        typedef std::set<SymbolPtr> SymbolSet;
    public:
        enum Type
        {
            /*!
             * The set of initialized symbols will perform union operation on all sibling tracers
             */
            Sequence,
            /*!
             * The set of initialized symbols will perform intersection operation on all sibling tracers
             */
            Branch
        };
    public:
        InitializationTracer(InitializationTracer* parent, Type type)
        :mergeCount(0), depth(0), parent(parent), type(type), superInit(false), selfInit(false)
        {
            if(parent)
                depth = parent->depth + 1;
        }
        ~InitializationTracer()
        {
            if (!parent)
            {
                //reset all symbol to uninitialized when branch tracer left the initializer scope
                SemanticUtils::setFlags(set, SymbolFlagInitialized, false);
                return;
            }
            if(type == Branch)
            {
                if(parent->mergeCount == 0)
                {
                    parent->set = set;
                    parent->selfInit = selfInit;
                    parent->superInit = superInit;
                }
                else
                {
                    SymbolSet tmp;
                    std::set_intersection(parent->set.begin(), parent->set.end(), set.begin(), set.end(), std::inserter(tmp, tmp.begin()));
                    parent->set = tmp;
                    parent->selfInit &= selfInit;
                    parent->superInit &= superInit;
                }
                //reset all symbol to uninitialized when branch tracer left the scope
                SemanticUtils::setFlags(set, SymbolFlagInitialized, false);
            }
            else
            {
                SymbolSet tmp;
                std::set_union(parent->set.begin(), parent->set.end(), set.begin(), set.end(), std::inserter(tmp, tmp.begin()));
                parent->set = tmp;
                //set all symbol to initialized when sequence tracer left the scope
                SemanticUtils::setFlags(set, SymbolFlagInitialized, true);
                parent->selfInit |= selfInit;
                parent->superInit |= superInit;
            }
            parent->mergeCount ++;
        }
        void add(const SymbolPtr& sym)
        {
            set.insert(sym);
        }
    public:
        SymbolSet set;
        int mergeCount;
        int depth;
        InitializationTracer* parent;
        Type type;
        bool superInit;
        bool selfInit;
    };

SWALLOW_NS_END


#endif//INITIALIZATION_TRACER_H
