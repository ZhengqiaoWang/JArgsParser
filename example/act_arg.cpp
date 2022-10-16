#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;

int main(int argc, char const *argv[])
{
    JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
    arg_parser
        .setArgument({"arg1", "-a", "--aaa", []()
                      { printf("aaa!\n"); },
                      "aaa"});

    if (false == arg_parser.parseArgs())
    {
        return -1;
    }

    return 0;
}
