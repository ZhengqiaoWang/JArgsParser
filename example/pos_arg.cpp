#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;

int main(int argc, char const *argv[])
{
    JArgsParser arg_parser(argc, argv, "Hello, this is JArgsParser", "https://github.com/ZhengqiaoWang/JArgsParser", "V0.0.2");
    arg_parser
        .setArgument({"arg1", ArgsValType::FLOAT, "aaa"})
        .setArgument({"arg2", ArgsValType::INT, "bbb"})
        .setArgument({"arg3", ArgsValType::STRING, "ccc"});

    if (false == arg_parser.parseArgs())
    {
        return -1;
    }

    float a1;
    double a2;
    arg_parser.getArgument("arg1",a1);
    arg_parser.getArgument("arg1",a2);

    printf("a1: %f\n",a1);
    printf("a2: %f\n",a2);

    int b1;
    long b2;
    unsigned int b3;
    unsigned long b4;
    arg_parser.getArgument("arg2",b1);
    arg_parser.getArgument("arg2",b2);
    arg_parser.getArgument("arg2",b3);
    arg_parser.getArgument("arg2",b4);

    printf("b1: %d\n",b1);
    printf("b2: %ld\n",b2);
    printf("b3: %u\n",b3);
    printf("b4: %lu\n",b4);

    std::string c1;
    arg_parser.getArgument("arg3",c1);

    printf("c1: %s\n",c1.c_str());

    return 0;
}
