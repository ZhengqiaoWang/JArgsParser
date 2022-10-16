# 使用帮助

`JArgsParser`使用起来很简单，你只需要将[最新版本的头文件](../interface/jargs_parser_api.hpp)放到项目中并引入即可～

## Hello World

我们开始写第一个`JArgsParser`程序吧。这个示例在[`example/helloworld.cpp`](../example/helloworld.cpp)。

首先，我们引用`JArgsParser`

```c++
#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;
```

然后我们指定要求有4个参数，这四个参数分属不同类型：

```c++
JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
arg_parser.setArgument({"arg1", ArgsValType::FLOAT, "This is a positional arg"});
arg_parser.setArgument({"arg2", "-b", "--bbb", "This is a flag"});
arg_parser.setArgument({"arg3", "-c", "--ccc", ArgsValType::LIST_STRING, "This is a value arg", false});
arg_parser.setArgument({"arg4", "-d", "--ddd", []()
                        { printf("** yoho! You call the ddd! **\n"); },
                        "This is an action flag"});
```

然后我们调用`parseArgs`解析参数。接下来使用`getArgument`获取指定key的参数。

```c++
if (false == arg_parser.parseArgs())
{
    printf("failed to parse\n");
    return -1;
}

{
    // arg 1
    double var_arg1{0};
    if (false == arg_parser.getArgument("arg1", var_arg1))
    {
        printf("failed to get arg\n");
        return -1;
    }
    printf("I got arg1: %f\n", var_arg1);
}

{
    // arg 2
    bool var_arg2{false};
    if (false == arg_parser.getArgument("arg2", var_arg2))
    {
        printf("I didn't got arg2\n");
    }
    printf("I got arg2: %s\n", var_arg2 ? "True" : "False");
}

{
    // arg 3
    std::vector<std::string> var_arg3;
    if (false == arg_parser.getArgument("arg3", var_arg3))
    {
        printf("I didn't got arg3\n");
    }
    else
    {
        printf("I got arg3: ");
        for (auto &arg : var_arg3)
        {
            printf("%s ",arg.c_str());
        }
        printf("\n");
    }
}
```

好了，现在你成功迈出了第一步～

## 帮助文档

你已经成功编译了第一个`JArgsParser`程序，是时候挑战一下学习更多特性了！

`JArgsParser`支持多种类型的参数：

- 位置参数: `./JArgsParser a 2 ./src`
- 标志参数: `./JArgsParser -d`
- 值参数: `./JArgsParser -e 123 -fi 3.14`
- 动作参数: `./JargsParser -g`

除此之外，还有一些默认的标志参数代表特定含义。这也意味着一些参数是被提前占用的～

### 参数描述器

上面提到了`JArgsParser`支持4中类型的参数，这些参数统一使用一个类`ArgsDesc`管理起来，这里我称这个类为`参数描述器`。我们可以注意到这个类的构造函数共有5个，其中4个与参数类型强相关。

这些构造函数中的形参统一符合以下的命名规范，因此只需要通过形参名称你就能知道它的含义是什么：

- `key`: 参数的主键，唯一区分各个参数，用于存储与查询参数结果
- `arg_val_type`: 参数值类型，包含了整数、浮点数、字符串和列表，我们可以通过`ArgsValType`了解这是什么。
- `description`: 参数描述，用于展示帮助时在右方显示的对参数的解释
- `short_name`: 短参数，一般为`-a`,`-b`，当然库的实现里并没有限制，理论上都可以。但要求短参数和长参数都不能冲突（哪怕短参数与长参数互相冲突也不行），只有位置参数没有这个选项。
- `full_name`: 长参数（全称），一般为`--aaa`,`--bbb`，与短参数一致
- `required`: 是否强制要求，目前只有值参数有这个选项。原因可以见下面的详细解释

### 位置参数

位置参数的位置取决于`setArgument`函数调用的顺序。我也强烈建议你将位置参数放在前面定义。

位置参数是强制默认要求用户必须提供的。因此你不需要指定`required`（因为默认是`true`）。

位置参数目前能允许接收的参数仅有`ArgsValType::INT`,`ArgsValType::FLOAT`,`ArgsValType::STRING`，并不支持`LIST`，不排除之后的版本支持这一点，虽然实现上非常简单。

下面我们举一个小例子展示一下[`example/pos_arg.cpp`](../example/pos_arg.cpp)：

```c++
JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
arg_parser
    .setArgument({"arg1", ArgsValType::FLOAT, "aaa"})
    .setArgument({"arg2", ArgsValType::INT, "bbb"})
    .setArgument({"arg3", ArgsValType::STRING, "ccc"});

if (false == arg_parser.parseArgs())
{
    printf("failed to parse\n");
    return -1;
}
```

接下来我们解析`arg1`，不过这里不一样的是，我们使用两个类型的变量重复接收：

```c++
float a1;
double a2;
arg_parser.getArgument("arg1",a1);
arg_parser.getArgument("arg2",a2);
```

此时，我们输入

```shell
./example/pos_arg 11 22 haha
```

可以看到结果为

```shell
a1: 11.000000
a2: 11.000000
```

没错，`JArgsParser`解析的时候默认解析成`double`，再根据用户的需要（参数类型），转换成`float`。

这点也在`INT`展现，实际上解析成了`long`，并根据需要转换成`int`,`unsigned int`,`unsigned long`。

```c++
int b1;
long b2;
unsigned int b3;
unsigned long b4;
arg_parser.getArgument("arg2",b1);
arg_parser.getArgument("arg2",b2);
arg_parser.getArgument("arg2",b3);
arg_parser.getArgument("arg2",b4);
```

结果为

```shell
b1: 22
b2: 22
b3: 22
b4: 22
```

事实上，这个特性也在值参数中得以体现。

对于`STRING`类型，就比较简单了。

```c++
std::string c1;
arg_parser.getArgument("arg3",c1);
```

结果为

```shell
c1: haha
```

### 值参数

值参数的参数就多了许多。但总体而言与位置参数没有过多不同。这里主要介绍二者的差异。

差异主要体现在以下几个方面：

- 支持长短参数
- 支持LIST
- 支持可选参数

值参数允许设置长短参数，一般而言，我们对值参数有一个共识，那就是短参数前加一个`-`，长参数前加一个`--`。虽然`JArgsParser`支持这么写，但我不会告诉你理论上来说你把长短参数调换个位置，甚至不加`-`或者`--`也都没事，前提是你要确定用户不会输入一个和你的参数规则一样的值。

虽然不建议这么做，但这个例子还是这么写了，通过这个例子也能帮助你理解这个库的实现原理以及他的局限性：

我们定义了三个参数，都强制要求了长短参数。在`arg2`中规定拿到一个小数列表。在`arg3`中规定不强制用户指定该参数。

接下来我们看个例子[`example/val_arg.cpp`](../example/val_arg.cpp)：

```c++
JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
arg_parser
    .setArgument({"arg1", "-a", "--aaa", ArgsValType::FLOAT, "aaa"})
    .setArgument({"arg2", "-b", "bbb", ArgsValType::LIST_FLOAT, "bbb"})
    .setArgument({"arg3", "c!", "--ccc", ArgsValType::STRING, "ccc", false});

if (false == arg_parser.parseArgs())
{
    return -1;
}
```

然后我们就可以对他们做解析了。

```c++
    float a1;
    arg_parser.getArgument("arg1", a1);
    printf("a1: %f\n", a1);

    std::vector<float> b1;
    arg_parser.getArgument("arg2", b1);
    printf("b1: ");
    for (auto &item : b1)
    {
        printf("%f ", item);
    }
    printf("\n");

    std::string c1;
    if (false == arg_parser.getArgument("arg3", b1))
    {
        printf("we didn't get arg3!\n");
    }
    else
    {
        printf("c1: %s\n", c1.c_str());
    }
```

此时如果我们提供这样的参数，就会得到如此结果：

```shell
$ ./example/val_arg -a 123 bbb 3,-2,3.2,0
a1: 123.000000
b1: 3.000000 -2.000000 3.200000 0.000000 
we didn't get arg3!
```

我们注意到，一个是`bbb`是我们的第二个参数标志，接受的是一个浮点数列表，另一个是对缺失的`arg3`程序并不会报错，只是在获取参数的时候提醒没有接收到参数。此时我们可以根据需要设置默认参数即可。

### 标志参数

标志参数实际上可以理解成类型是`bool`且没有后续值的参数。一旦触发就设置成`true`。

我们只能通过这种方式获取它的值：

```c++
bool a1;
arg_parser.getArgument("arg1", a1);
```

### 动作参数

动作参数一开始我是打算设计成为`-h`和`-v`提供支持的一类参数，但实际情况是因为优先级的关系（`JArgsParser`不具备参数优先级能力）常常出来的结果很混乱，因此将[帮助](#帮助)和[版本](#版本)单独提出来实现。动作参数也依旧保留。

动作参数不一定是所有参数类型中扫描触发优先级最高的（甚至可以说是毫无优先级的），但一定是早于任意`getArgument`的，因为它在`parseArgs`阶段就执行了。

接下来我们就展示一下[`example/act_arg.cpp`](../example/act_arg.cpp)：

```c++
JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
arg_parser
    .setArgument({"arg1", "-a", "--aaa", []()
                    { printf("aaa!\n"); },
                    "aaa"});

if (false == arg_parser.parseArgs())
{
    return -1;
}
```

是不是特别简单？这段参数的含义是当用户输入`-a`或者`--aaa`时，触发`lambda`表达式，在屏幕打印`aaa!\n`。

我们可以得到这样的结果：

```shell
$ ./example/act_arg -a
aaa!
```

### 默认参数

`JArgsParser`是带有默认缺省的参数的，包含下面的

- `-h`|`--help` 帮助
- `-v`|`--version` 版本

满足一般的代码框架。

#### 帮助

一般帮助在[`参数校验失败`](#参数类型检查)、`用户请求帮助`以及`有强制要求参数时不输入任何参数`时展示。

我们这里以helloworld的程序举例（有强制要求参数）:

```shell
$ ./example/helloworld 
usage: ./example/helloworld arg1(float) [-b|--bbb(flag)] [-c|--ccc(vec<string>)] [-d|--ddd(flag)] 
Hello, this is JArgsParser
Options and arguments (and corresponding environment variables):
  arg1      : This is a position arg {req|float}
  -b|--bbb  : This is a flag {opt|flag}
  -c|--ccc  : This is a value arg {opt|vec<string>}
  -d|--ddd  : This is an action flag {opt|flag}
https://github.com/ZhengqiaoWang/JArgsParser
failed to parse
```

以`act_arg`做展示（没有强制要求参数）：

```shell
$ ./example/act_arg
$ ./example/act_arg --help
usage: ./example/act_arg [-a|--aaa(flag)] 
Hello, this is JArgsParser
Options and arguments (and corresponding environment variables):
  -a|--aaa  : aaa {opt|flag}
https://github.com/ZhengqiaoWang/JArgsParser
```

#### 版本

版本在一开始构造`JArgsParser`就让你提供了，一般而言我们认为版本是在编译期就已经决定的事情。

使用方法也很简单：

```shell
$ ./example/act_arg -v
version: V0.0.2
$ ./example/act_arg --version
version: V0.0.2
```

## 其他特性

`JArgsParser`具有一些其他特性可以拿来说道说道。

### 参数类型检查

当指定参数类型是`INT`,`FLOAT`或者`LIST_INT`，`LIST_FLOAT`时，会自动校验参数是否是数值类型。不过`INT`会比`FLOAT`更严格一点，因为它不允许用户输入`.`。

当用户输入预期类型之外的参数时，会在控制台报错并输出问题所在：

```shell
$ ./example/pos_arg aa 123 ff
ERROR: Failed to parse [|] for [arg1] because of: type check failed
usage: ./example/pos_arg arg1(float) arg2(int) arg3(string) 
Hello, this is JArgsParser
Options and arguments (and corresponding environment variables):
  arg1  : aaa {req|float}
  arg2  : bbb {req|int}
  arg3  : ccc {req|string}
https://github.com/ZhengqiaoWang/JArgsParser
```

## 觉得不好？有新的想法？

注意`JArgsParser`所使用的协议，你随便改，我不负责。当然如果有好的想法好的建议，欢迎你提出想法以及投身进来，帮助完善这个项目。
