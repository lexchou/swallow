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

先定位到main入口函数，看到while循环，getline(wcin, line)从终端逐行获取wstring
交给Repl来打印CompilerResults输出结果。

## 开始HACKING

代码风格和@lexchou保持一致

```
#include <fstream>

wifstream file;                                                                
if(argv[1])                                                                    
{                                                                              
    file.open(argv[1]);                                                        
    if(!file.good())                                                           
    {                                                                          
        out->printf(L"%s does not exist!\n", argv[1]);                         
        return 0;                                                              
    }                                                                          
}
```

getline(argv[1] ? file : wcin, line)就可以从文件中逐行获取wstring

## 准备一个hello.swift文件 

看看./repl/repl hello.swift效果

```
func test(a : Int) -> String { return ""; }                                        
                                                                                   
func test(a : String) -> Bool { return true; }                                     
                                                                                   
let a = (test(56), test(""))
```

运行hello.swift，会segfault报错The symbol already exists with the same name 
https://github.com/lexchou/swallow/issues/3 
PS: 将在下一篇HACKING教程里讲如何简单的monkey patch（将getCurrentScope替换成getGlobalScope）
不过@lexchou已经在TODO准备用正确的姿势fix这个bug了 
https://github.com/lexchou/swallow/blob/master/TODO.org#codes-that-can-crash-swallow01

