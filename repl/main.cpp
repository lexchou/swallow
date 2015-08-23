/* main.cpp --
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
#include <iostream>
#include "ConsoleWriter.h"
#include "common/Errors.h"
#include "common/SwallowUtils.h"
#include "REPL.h"

using namespace std;
using namespace Swallow;



int main(int argc, char** argv)
{
    ConsoleWriterPtr out(ConsoleWriter::create());
    if(argc > 1)
    {
        //evaluation mode
        const char * fileName = argv[1];
        if(!SwallowUtils::fileExists(fileName))
        {
            out->setForegroundColor(ConsoleColor::Red, ConsoleIntensity::Bright);
            out->printf(L"Swallow source file %s is not existing.\n", fileName);
            out->reset();
            return 1;
        }
        SwallowCompiler* compiler = SwallowCompiler::newCompiler(L"repl");
        wstring code = SwallowUtils::readFile(fileName);
        compiler->addSource(SwallowUtils::toWString(fileName), code);
        out->printf(L"Evaluating %s...\n", fileName);
        compiler->compile();
        if(compiler->getCompilerResults()->numResults() == 0)
            out->printf(L"No errors.\n", fileName);
        else
            REPL::dumpCompilerResults(compiler->getCompilerResults(), out);
        delete compiler;
    }
    else
    {
        //repl mode
        REPL repl(out);
        repl.repl();
    }
    return 0;
}
