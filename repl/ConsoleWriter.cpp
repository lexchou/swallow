#include "ConsoleWriter.h"
#include <stdlib.h>
#include <wchar.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>


void ConsoleWriter::printf(const wchar_t* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vwprintf(fmt, va);
    va_end(va);
}

ConsoleWriter* ConsoleWriter::create()
{
    if(isatty(fileno(stdout)))
    {
        return new AnsiConsoleWriter();
    }
    else
    {
        return new PlainConsoleWriter();
    }

}



void PlainConsoleWriter::setForegroundColor(ConsoleColor color, ConsoleIntensity intensity)
{
}
void PlainConsoleWriter::setBackgroundColor(ConsoleColor color, ConsoleIntensity intensity)
{
}
void PlainConsoleWriter::reset()
{
}


void AnsiConsoleWriter::setForegroundColor(ConsoleColor color, ConsoleIntensity intensity)
{
    printf(L"\x1b[%dm", (intensity == Normal ? 30 : 90) + color);
}
void AnsiConsoleWriter::setBackgroundColor(ConsoleColor color, ConsoleIntensity intensity)
{
    printf(L"\x1b[%dm", (intensity == Normal ? 40 : 100) + color);
}
void AnsiConsoleWriter::reset()
{
    printf(L"\x1b[0m");
}