# User Guide

To get started with `JArgsParser`, all you need is to download the [latest release version](../interface/jargs_parser_api.hpp) which is just a single header and include it in your source file.

## Hello World

Now we can make our first program using `JArgsParser`. See also in [`example/helloworld.cpp`](../example/helloworld.cpp).

Firstly, include `JArgsParser`:

```c++
#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;
```

Than, we can use `JArgsParser` to require:

```c++
JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
arg_parser.setArgument({"arg1", ArgsValType::FLOAT, "This is a positional arg"});
arg_parser.setArgument({"arg2", "-b", "--bbb", "This is a flag"});
arg_parser.setArgument({"arg3", "-c", "--ccc", ArgsValType::LIST_STRING, "This is a value arg", false});
arg_parser.setArgument({"arg4", "-d", "--ddd", []()
                        { printf("** yoho! You call the ddd! **\n"); },
                        "This is an action flag"});
```

Next, we `parseArgs` and get them.

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

Congratulations! Now you can use `JArgsParser`!

## Full Guide

You have compiled your first `JArgsParser Hello World`. Now it's time to learn more about it's features.

`JArgsParser` support multi kinds of arguments:

- positional argument: `./JArgsParser a 2 ./src`
- flag argument: `./JArgsParser -d`
- value argument: `./JArgsParser -e 123 -fi 3.14`
- action argument: `./JargsParser -g`

and some default flag arguments.

So in the text below, we will tell you all about `JArgsParser`.

### Positional Argument

The position each positional argument should be in depends on the order of `setArgument` in the code. This also means that setting positional arguments using `setArgument` does not require specifying the position of the argument.

It is worth mentioning that I strongly recommend that positional arguments are always set first, so that the readability of the code is guaranteed.

#### 特点展示

| - | - |
| --- | --- |
| Required | True |
| Support Type | INT, FLOAT, STRING |
| Parsed Type | `int`, `unsigned int`, `long`, `unsigned long`, `double`, `float` |

#### Examples

展示使用位置参数的代码，包含所有的可用参数类型

展示错误使用时的情景（类型错误，缺少参数）

### Value Argument



### Flag Argument

### Action Argument

### Default Argument

#### Help

#### Version

### Get Argument

#### Support Type

#### Examples

## How to improve?

`JArgsParser` uses [`Apache 2.0 License`](../license). Off course, you can edit the source code whatever you want.

Or, you can give us a PR or an Issue.
