#include "jargs_parser_api.h"
// #include "transfer_utils.h"
using namespace Joger;
int main(int argc, char const *argv[])
{
    JArgsParser arg_parser(argc, argv, "描述", "版权", "V1.0.0");
    arg_parser.setArgument("a", {ArgsValType::INT, "this is a float position arg"});
    arg_parser.setArgument("b", {ArgsValType::FLOAT, "this is a float position arg"});
    arg_parser.setArgument("c", {ArgsValType::STRING, "this is a STRING position arg"});

    arg_parser.setArgument("d", {"-d", "--di", "this is a flag arg"});

    arg_parser.setArgument("e", {"-e", "--ei", ArgsValType::INT, "this is a value arg"});
    arg_parser.setArgument("f", {"-f", "--fi", ArgsValType::FLOAT, "this is a value arg", false});

    arg_parser.setArgument("g", {"-g", "--g", [](const std::string &key)
                                 { printf("action g!\n"); },
                                 "this is a ACTION arg"});

    arg_parser.setArgument("i", {"-i", "--i", ArgsValType::LIST_FLOAT, "this is a LIST_FLOAT value arg"});

    arg_parser.setArgument("j", {"-j", "--j", ArgsValType::LIST_STRING, "this is a LIST_STRING value arg"});

    
    if (false == arg_parser.parseArgs())
    {
        printf("parse failed\n");
        return -1;
    }
    return 0;
}
