#include "jargs_parser_api.hpp"
using namespace Joger;

#define GET_ARGS(KEY, TYPE, FMT)                       \
    {                                                  \
        TYPE b{0};                                     \
        if (false == arg_parser.getArgument(KEY, b))   \
        {                                              \
            printf("parse failed\n");                  \
            return -1;                                 \
        }                                              \
        printf("(" #TYPE ") " #KEY ": " #FMT "\n", b); \
    }

#define GET_ARGS_STR(KEY, TYPE, FMT)                           \
    {                                                          \
        TYPE b{0};                                             \
        if (false == arg_parser.getArgument(KEY, b))           \
        {                                                      \
            printf("parse failed\n");                          \
            return -1;                                         \
        }                                                      \
        printf("(" #TYPE ") " #KEY ": " #FMT "\n", b.c_str()); \
    }

#define GET_ARGS_VEC(KEY, TYPE, FMT)                 \
    {                                                \
        std::vector<TYPE> b;                         \
        if (false == arg_parser.getArgument(KEY, b)) \
        {                                            \
            printf("parse failed\n");                \
            return -1;                               \
        }                                            \
        printf("(" #TYPE ") " #KEY ": ");            \
        for (auto &item : b)                         \
        {                                            \
            printf(#FMT " ", item);                  \
        }                                            \
        printf("\n");                                \
    }

#define GET_ARGS_VEC_STR(KEY, TYPE, FMT)             \
    {                                                \
        std::vector<TYPE> b;                         \
        if (false == arg_parser.getArgument(KEY, b)) \
        {                                            \
            printf("parse failed\n");                \
            return -1;                               \
        }                                            \
        printf("(" #TYPE ") " #KEY ": ");            \
        for (auto &item : b)                         \
        {                                            \
            printf(#FMT " ", item.c_str());          \
        }                                            \
        printf("\n");                                \
    }

int main(int argc, char const *argv[])
{
    JArgsParser arg_parser(argc, argv, "描述", "版权", "V1.0.0");
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

    // std::vector<float> i_vec;
    GET_ARGS_VEC("i", float, "%f");
    GET_ARGS_VEC("i", double, "%f");

    GET_ARGS_VEC_STR("j", std::string, "%s");
    return 0;
}
