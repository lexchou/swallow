/* JSONSerializer.h --
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
#include <fcgio.h>
#include "RequestHandler.h"

using namespace std;
int main()
{
    streambuf* cin_buf = cin.rdbuf();
    streambuf* cout_buf = cout.rdbuf();
    streambuf* cerr_buf = cout.rdbuf();
    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);
    RequestHandler handler;

    while(FCGX_Accept_r(&request) == 0)
    {
        fcgi_streambuf fcgi_cin(request.in);
        fcgi_streambuf fcgi_cout(request.out);
        fcgi_streambuf fcgi_cerr(request.err);
        cin.rdbuf(&fcgi_cin);
        cout.rdbuf(&fcgi_cout);
        cerr.rdbuf(&fcgi_cerr);

        handler.handle(&request);

        /*
        const char * const * envp = request.envp;
        for ( ; *envp; ++envp)
        {
            cout << *envp << "\n";
        }
        */
    }
    cin.rdbuf(cin_buf);
    cout.rdbuf(cout_buf);
    cerr.rdbuf(cerr_buf);
    return 0;
}
