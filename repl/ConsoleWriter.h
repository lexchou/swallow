/* ConsoleWriter.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
#ifndef CONSOLE_WRITER_H
#define CONSOLE_WRITER_H


enum ConsoleColor
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};
enum ConsoleIntensity
{
    Normal,
    Bright
};

class ConsoleWriter
{
public:
    virtual void setForegroundColor(ConsoleColor color, ConsoleIntensity intensity = Normal) = 0;
    virtual void setBackgroundColor(ConsoleColor color, ConsoleIntensity intensity = Normal) = 0;
    virtual void reset() = 0;

    void printf(const wchar_t* fmt, ...);
public:
    static ConsoleWriter* create();
protected:

};


class PlainConsoleWriter : public ConsoleWriter
{
public:
    virtual void setForegroundColor(ConsoleColor color, ConsoleIntensity intensity = Normal)override;
    virtual void setBackgroundColor(ConsoleColor color, ConsoleIntensity intensity = Normal)override;
    virtual void reset()override;
};

class AnsiConsoleWriter : public ConsoleWriter
{
public:
    virtual void setForegroundColor(ConsoleColor color, ConsoleIntensity intensity = Normal)override;
    virtual void setBackgroundColor(ConsoleColor color, ConsoleIntensity intensity = Normal)override;
    virtual void reset()override;
};


#endif//CONSOLE_WRITER_H
