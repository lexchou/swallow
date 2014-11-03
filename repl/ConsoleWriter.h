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