/* ScopedValue.h --
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
#ifndef SCOPED_VALUE_H
#define SCOPED_VALUE_H
#include "swallow_conf.h"

SWALLOW_NS_BEGIN


    /*!
     * This class uses RAAI to garantee the variable will be changed to specified value during it's scope.
     */
    template<class T>
    struct ScopedValue
    {
        ScopedValue(T & ref)
                :ref(ref), value(ref)
        {

        }
        ScopedValue(T & ref, const T& newValue)
                :ref(ref), value(ref)
        {
            set(newValue);
        }
        void set(const T& val)
        {
            ref = val;
        }

        ~ScopedValue()
        {
            ref = value;
        }
        T& ref;
        T value;
    };
    #define COMBINE1(A,B) A ## B
    #define COMBINE(A,B) COMBINE1(A,B)
    #define SCOPED_SET(ref, val) ScopedValue<decltype(ref)> COMBINE(scopedValue_, __LINE__) (ref, val); (void)COMBINE(scopedValue_, __LINE__);
SWALLOW_NS_END

#endif//SCOPED_VALUE_H