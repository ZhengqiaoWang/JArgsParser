#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;

int main(int argc, char const *argv[])
{
    JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
    arg_parser.setArgument({"arg1", ArgsValType::FLOAT, "This is a position arg"});
    arg_parser.setArgument({"arg2", "-b", "--bbb", "This is a flag"});
    arg_parser.setArgument({"arg3", "-c", "--ccc", ArgsValType::LIST_STRING, "This is a value arg", false});
    arg_parser.setArgument({"arg4", "-d", "--ddd", []()
                            { printf("** yoho! You call the ddd! **\n"); },
                            "This is an action flag"});

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

    return 0;
}
