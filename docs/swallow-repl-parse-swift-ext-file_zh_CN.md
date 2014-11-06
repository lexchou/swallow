让swallow的Repl解析*.swift文件 
===============================

swallow的Repl现在可以像Python解析器一样，输入一行swift代码，回车，打印输出：

```
./repl/repl 
Welcome to Swallow! Type :help for assistance.
  1> var s = "Hello world"
s : String
  2> 
```

再输入一行，回车，接着打印，很有Python的感觉 ;)

不过这样一行一行输入swift代码有点儿不刷辣，特别是面向我这样的懒人，喜欢Ctrl+C/Ctrl+V
学习swift语言，那么就HACKING一下Repl让其像Python解析器一样解析*.swift文件 ;)


## 用您喜爱的编辑器

（请不要引入VIM与Emacs之争）

```
vim ./repl/main.cpp
```

先定位到main入口函数，给REPL构造函数添加一个参数const char* path，默认为NULL

## 开始HACKING

代码风格和@lexchou保持一致

```
REPL repl(out, argv[1] ? argv[1] : NULL);
```

./repl/REPL.h

```
#include <fstream>

// 构造函数添加一个默认参数
REPL(const ConsoleWriterPtr& out, const char* path = NULL);

std::wifstream file; // 私有成员
```

./repl/REPL.cpp

```
file(path) // 调用std::wifstream的构造函数
file.good() ? getline(file, line) : getline(wcin, line); // 逐行读文件
```

## 准备一个hello.swift文件 

看看./repl/repl hello.swift效果

```
func test(a : Int) -> String { return ""; }                                        
                                                                                   
func test(a : String) -> Bool { return true; }                                     
                                                                                   
let a = (test(56), test(""))
```

看看效果 ;)

