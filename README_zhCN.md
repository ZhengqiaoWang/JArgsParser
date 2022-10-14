# JArgsParser

`JArgsParser`是一个只有一个头文件的C++11参数解析库。我们可以将头文件引入便可快速地使用。

目前它支持4种类型的参数：

- 位置参数: `./JArgsParser a 2 ./src`
- 标志参数: `./JArgsParser -d`
- 值参数: `./JArgsParser -e 123 -fi 3.14`
- 动作参数: `./JargsParser -g`，与程序内定义的函数绑定，当用于填写动作参数时会优先触发函数

与此同时，还带有默认的标志参数：

- `-h|--help`: 帮助
- `-v|--version`: 版本

下面是效果示例：

![example](assets/show.gif)

## 如何开始

下载最新的发布版本（interface/jargs_parser_api.hpp）然后放到你的项目目录里。我们可以参考`src/main.cpp`来学习使用这个库。

在项目代码中`include`一下`JArgsParser`:

```c++
#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;
```

然后我们手动设置一下都要收到什么参数：

| 参数主键key | 参数类型type         | 收到的参数类型value type    | 是否强制要求required |
| --- | ------------ | ------------- | -------- |
| a   | 位置参数position arg | int           | true     |
| b   | 位置参数position arg | float         | true     |
| c   | 位置参数position arg | string        | true     |
| d   | 标志参数flag arg     | bool          | false    |
| e   | 值参数value arg    | int           | true     |
| f   | 值参数value arg    | float         | false    |
| g   | 动作参数action arg   | none          | false    |
| i   | 值参数value arg    | vec\<float\>  | true     |
| j   | 值参数value arg    | vec\<string\> | true     |

```c++
    JArgsParser arg_parser(argc, argv, "Program ABC, for testing JArgsParser", "Here's the place for copyright", "V1.0.0");
    arg_parser.setArgument({"a", ArgsValType::INT, "this is a float position arg"});
    arg_parser.setArgument({"b", ArgsValType::FLOAT, "this is a float position arg"});
    arg_parser.setArgument({"c", ArgsValType::STRING, "this is a STRING position arg"});

    arg_parser.setArgument({"d", "-d", "--di", "this is a flag arg"});

    arg_parser.setArgument({"e", "-e", "--ei", ArgsValType::INT, "this is a value arg"});
    arg_parser.setArgument({"f", "-f", "--fi", ArgsValType::FLOAT, "this is a value arg", false});

    arg_parser.setArgument({"g", "-g", "--g", [](const std::string &key)
                            { printf("action g!\n"); },
                            "this is a ACTION arg"});

    arg_parser.setArgument({"i", "-i", "--iabcdefg", ArgsValType::LIST_FLOAT, "this is a LIST_FLOAT value arg"});

    arg_parser.setArgument({"j", "-j", "--j", ArgsValType::LIST_STRING, "this is a LIST_STRING value arg"});
```

然后我们可以使用 `parseArgs`拿到想要的参数（通过参数主键）。下面的示例摘自`src/main.cpp`，`GET_ARGS*`开头的宏也在那里定义，不是在库里哟~

```c++
    if (false == arg_parser.parseArgs())
    {
        printf("parse failed\n");
        return -1;
    }
    GET_ARGS("a", int, "%d");
    GET_ARGS("a", long, "%d");
    GET_ARGS("a", unsigned int, "%d");
    GET_ARGS("a", unsigned long, "%d");
    GET_ARGS("b", int, "%d");
    GET_ARGS("b", float, "%f");
    GET_ARGS("b", double, "%f");
    GET_ARGS_STR("c", std::string, "%s");
    GET_ARGS("d", bool, "%d");
    GET_ARGS("e", int, "%d");
    GET_ARGS("f", float, "%f");

    GET_ARGS_VEC("i", float, "%f");
    GET_ARGS_VEC("i", double, "%f");

    GET_ARGS_VEC_STR("j", std::string, "%s");
```
