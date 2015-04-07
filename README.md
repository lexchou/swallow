# Swallow

## Introduction & Disclaimer
Swallow is an open-sourced implementation of swift compiler, but the current version is only a 'pre-pre-alpha' version which is still undergoing heavy development and testing before its official release, and I can only develop this within my spare time.

I don't garantee the development progress since I need to take care of my new born baby and have my own full-time job.

The semantic analysis is almost done, swallow provides same syntax checking, type inference and symbol processing like official implementation can do.

The current version only implemented a rough REPL that even has no code generation, you can see what it can do from the REPL or a [online demo](https://chou.it/swift/), only some syntax checking and see the result of symbol's type inference.

I'm working on name mangling these days, the related study result are published in [my blog](http://chou.it/2015/04/swift-name-mangling/), an english version will be provided and committed to this repository when it's done. The working on reverse engineering of SIL will be started after name mangling done(including test cases and a full documentation), the documentation of SIL reverse engineering will also be commited to my blog and this repository.

The code generation will be based on LLVM, then we can make use of LLVM's toolchain to build a full ecosystem.

The test cases are used heavily to cover all code, but still got some situations are not well handled, those code that can crash the REPL will be added as test cases sequentially, the codes in [swift-compiler-crashes](https://github.com/practicalswift/swift-compiler-crashes) will eventually merged to test case when the compiler is stable enough.

If you want to see the code, you will find I used some simple hacking trick to handle built-in types(e.g. Builtin.Word), this incorrect approach will be fixed when the compiler can support a complete standard library.



# 介绍与免责声明
Swallow 是一个开源的swift编译器实现，不过目前还没完工，只能当做一个体验版，好多东西都还需要大量的时间去开发，我只能在业余的时间去开发，加之要照顾刚出生的小孩，因此我不能保证开发进度能很快。

语义分析基本完工，能提供和官方编译器一致的语法检查、类型推导和符号处理能力。

现在连代码生成都还没来得及弄，现在的版本只提供了一个粗糙仿制的REPL，以及一个[在线版的演示](https://chou.it/swift/)，可以看到现在的进展情况。暂时自带的REPL只能查看一些语法和语义错误，以及查看符号的类型推导的结果。

更新README.md的这段时间，在做Name Mangling，相关研究结果目前放在我[博客上](http://chou.it/2015/04/swift-name-mangling/)，后期会整理并提供双语版提交到这个repository里。Name Mangling弄完后就要开始弄SIL代码生成了，关于SIL的分析文档我也会整理发到博客以及这个repo里。

代码生成这块打算使用LLVM来完成，这样方便利用LLVM的工具链来提供完整的生态系统。

编译器代码本身尽量用测试用例覆盖，但是在做REPL的时候发现好多情况都没考虑到，那些会引起REPL崩溃的代码 将会陆续放入到测试用例中的，后期也打算将[swift-compiler-crashes](https://github.com/practicalswift/swift-compiler-crashes)的代码也集成到测试用例中去。



# Build instructions
Clone this repository to your drive:
```
$ git clone https://github.com/lexchou/swallow/
```
Build the swallow/repl/test cases:
```
$ cd swallow
$ cmake .
$ make
```
Start repl:
```
$ repl/repl
```


# 构建过程
先将项目克隆到本地：
```
$ git clone https://github.com/lexchou/swallow/
```
然后编译 swallow/repl/test cases:
```
$ cd swallow
$ cmake .
$ make
```
执行 repl:
```
$ repl/repl
```


# Snapshots
Comparison between official REPL and swallow

![Comparison between official REPL and swallow](docs/images/repl-comparison.jpg)

Type inference

![Type inference example](docs/images/type-inference.png)

Function overloads

![Function overloads](docs/images/function-overloads.png)


# License

```
 Copyright (c) 2014, Lex Chou <lex at chou dot it> 
 All rights reserved.
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of Swallow nor the names of its contributors may be
   used to endorse or promote products derived from this software without
   specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS" AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

# 授权许可
```
 Copyright (c) 2014, Lex Chou <lex at chou dot it> 
 
 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
 
 * 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
   及下述的免责声明。
 * 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
   于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
   的免责声明。
 * 未获事前取得书面许可，不得使用Swallow或本软件贡献者之名称，
   来为本软件之衍生物做任何表示支持、认可或推广、促销之行为。
 
 免责声明：本软件是由Swallow及本软件之贡献者以现状（"as is"）提供，
 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
 的的适用性为默示性担保。Swallow及本软件之贡献者，无论任何条件、
 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
```

