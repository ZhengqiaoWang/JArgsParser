#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;

int main(int argc, char const *argv[])
{
    JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
    arg_parser
        .setArgument({"arg1", "-a", "--aaa", ArgsValType::FLOAT, "aaa"})
        .setArgument({"arg2", "-b", "bbb", ArgsValType::LIST_FLOAT, "bbb"})
        .setArgument({"arg3", "c!", "--ccc", ArgsValType::STRING, "ccc", false});

    if (false == arg_parser.parseArgs())
    {
        return -1;
    }

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
    return 0;
}
