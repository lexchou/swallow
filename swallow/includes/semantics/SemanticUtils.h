/* SemanticUtils.h --
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
#ifndef SEMANTIC_UTILS_H
#define SEMANTIC_UTILS_H
#include "swallow_conf.h"
#include <memory>
#include "Symbol.h"
#include "swallow_types.h"
#include "semantic-types.h"
#include <vector>
#include <map>

SWALLOW_NS_BEGIN

    typedef std::shared_ptr<class Identifier> IdentifierPtr;
    typedef std::shared_ptr<class Pattern> PatternPtr;
    typedef std::shared_ptr<class Expression> ExpressionPtr;
    struct SemanticUtils
    {
        static bool all(const std::vector<SymbolPtr>& syms, SymbolFlags flags)
        {
            for(const SymbolPtr& sym : syms)
            {
                if(!sym->hasFlags(flags))
                    return false;
            }
            return true;
        }
        static bool allInitialized(const std::vector<SymbolPtr>& syms)
        {
            return all(syms, SymbolFlagInitialized);
        }
        template<class SymbolsT>
        static void setFlags(SymbolsT& container, SymbolFlags flags, bool set)
        {
            for(auto & sym : container)
            {
                sym->setFlags(flags, set);
            }
        }
    };

SWALLOW_NS_END


#endif//SEMANTIC_UTILS_H
