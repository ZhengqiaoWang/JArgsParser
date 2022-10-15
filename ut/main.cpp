#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// for unit test
#define private public
#define exit(RTN_NUM) return true
#include "jargs_parser_api.hpp"
using namespace Joger::ArgsParser;

TEST_CASE("test for basic info")
{
    int argc{2};
    const char *argv[]{
        "ut",
        "-v"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    CHECK(jap.m_argc == 2);
    CHECK(jap.m_argv_vec.size() == 1);
    CHECK(jap.m_copyright == "copyright");
    CHECK(jap.m_description == "desc");
    CHECK(jap.m_version_txt == "1.0.0");
    CHECK(jap.parseArgs() == true);
}

#define UT_CHECK_ARG_EQ(KEY, TYPE, EXPECT)        \
    {                                             \
        TYPE tmp;                                 \
        CHECK(jap.getArgument(KEY, tmp) == true); \
        CHECK_EQ(EXPECT, tmp);                    \
    }

// because of doctest's mistake in comaring float numbers
#define UT_CHECK_ARG_LTMS(KEY, TYPE, EXPECT)      \
    {                                             \
        TYPE tmp;                                 \
        CHECK(jap.getArgument(KEY, tmp) == true); \
        CHECK(EXPECT - tmp < 0.0001);             \
    }

#define UT_NO_ARG(KEY)                             \
    {                                              \
        bool tmp;                                  \
        CHECK(jap.getArgument(KEY, tmp) == false); \
    }

#define UT_CHECK_ARR(KEY, TYPE, ...)                    \
    {                                                   \
        std::vector<TYPE> expect_vec{__VA_ARGS__}, tmp; \
        CHECK(jap.getArgument(KEY, tmp) == true);       \
        CHECK(expect_vec.size() == tmp.size());         \
        for (int i = 0; i < expect_vec.size(); ++i)     \
        {                                               \
            CHECK(expect_vec[i] == tmp[i]);             \
        }                                               \
    }

// ------------------------------------------------
// ------- position arguments test - start --------
TEST_CASE("test for position success")
{
    int argc{4};
    const char *argv[]{
        "ut",
        "abc",
        "123",
        "-1.24"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg2", ArgsValType::INT, "haha"});
    jap.setArgument({"arg3", ArgsValType::FLOAT, "haha"});
    CHECK(jap.parseArgs() == true);
    UT_CHECK_ARG_EQ("arg1", std::string, "abc");

    UT_CHECK_ARG_EQ("arg2", unsigned int, 123);
    UT_CHECK_ARG_EQ("arg2", unsigned long, 123);
    UT_CHECK_ARG_EQ("arg2", int, 123);
    UT_CHECK_ARG_EQ("arg2", long, 123);

    UT_CHECK_ARG_LTMS("arg3", float, -1.24);
    UT_CHECK_ARG_LTMS("arg3", double, -1.24);

    UT_NO_ARG("NOARG");
}

TEST_CASE("test for position failed - type failed")
{
    int argc{4};
    const char *argv[]{
        "ut",
        "abc",
        "123a",
        "-1.24"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg2", ArgsValType::INT, "haha"});
    jap.setArgument({"arg3", ArgsValType::FLOAT, "haha"});
    CHECK(jap.parseArgs() == false);
}

TEST_CASE("test for position failed - required failed")
{
    int argc{3};
    const char *argv[]{
        "ut",
        "abc",
        "123"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg2", ArgsValType::INT, "haha"});
    jap.setArgument({"arg3", ArgsValType::FLOAT, "haha"});
    CHECK(jap.parseArgs() == false);
}

// ------------------------------------------------
// ------- position arguments test - end ----------

// ------------------------------------------------
// ------- flag arguments test - start ------------
TEST_CASE("test for flag success")
{
    int argc{3};
    const char *argv[]{
        "ut",
        "-a",
        "--boy"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", "haha"});
    jap.setArgument({"arg2", "-b", "--boy", "haha"});
    jap.setArgument({"arg3", "-c", "--cry", "haha"});

    CHECK(jap.parseArgs() == true);
    UT_CHECK_ARG_EQ("arg1", bool, true);
    UT_CHECK_ARG_EQ("arg2", bool, true);
    UT_CHECK_ARG_EQ("arg3", bool, false);
    UT_NO_ARG("NOARG");
}

TEST_CASE("test for flag failed")
{
    int argc{3};
    const char *argv[]{
        "ut",
        "-a",
        "--boy"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", "haha"});
    jap.setArgument({"arg2", "-b", "--boy", "haha"});
    jap.setArgument({"arg3", "-c", "--cry", "haha"});

    CHECK(jap.parseArgs() == true);
    UT_CHECK_ARG_EQ("arg1", bool, true);
    UT_CHECK_ARG_EQ("arg2", bool, true);
    UT_CHECK_ARG_EQ("arg3", bool, false);
    UT_NO_ARG("NOARG");
}

// ------------------------------------------------
// ------- flag arguments test - end --------------

// ------------------------------------------------
// ------- value arguments test - start -----------
TEST_CASE("test for value success")
{
    int argc{9};
    const char *argv[]{
        "ut",
        "-a", "123",
        "--boy", "hahaha",
        "--cry", "-1,0,3.14,8888888",
        "-d", "hello,hi,bonjor"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", ArgsValType::FLOAT, "haha"});
    jap.setArgument({"arg2", "-b", "--boy", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg3", "-c", "--cry", ArgsValType::LIST_FLOAT, "haha"});
    jap.setArgument({"arg4", "-d", "--dog", ArgsValType::LIST_STRING, "haha"});
    jap.setArgument({"arg5", "-f", "--full", ArgsValType::LIST_STRING, "haha", false});

    CHECK(jap.parseArgs() == true);
    UT_CHECK_ARG_EQ("arg1", float, 123);
    UT_CHECK_ARG_EQ("arg2", std::string, "hahaha");
    UT_CHECK_ARR("arg3", float, -1, 0, 3.14, 8888888);
    UT_CHECK_ARR("arg4", std::string, "hello", "hi", "bonjor");
    UT_NO_ARG("arg5");

    UT_NO_ARG("NOARG");
}

TEST_CASE("test for value failed - type failed - val")
{
    int argc{9};
    const char *argv[]{
        "ut",
        "-a", "123f",
        "--boy", "hahaha",
        "--cry", "-1,0,3.14,8888888",
        "-d", "hello,hi,bonjor"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", ArgsValType::FLOAT, "haha"});
    jap.setArgument({"arg2", "-b", "--boy", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg3", "-c", "--cry", ArgsValType::LIST_FLOAT, "haha"});
    jap.setArgument({"arg4", "-d", "--dog", ArgsValType::LIST_STRING, "haha"});
    jap.setArgument({"arg5", "-f", "--full", ArgsValType::LIST_STRING, "haha", false});

    CHECK(jap.parseArgs() == false);
}

TEST_CASE("test for value failed - type failed - list")
{
    int argc{9};
    const char *argv[]{
        "ut",
        "-a", "123",
        "--boy", "hahaha",
        "--cry", "-1,0,3.14,8888888a",
        "-d", "hello,hi,bonjor"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", ArgsValType::FLOAT, "haha"});
    jap.setArgument({"arg2", "-b", "--boy", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg3", "-c", "--cry", ArgsValType::LIST_FLOAT, "haha"});
    jap.setArgument({"arg4", "-d", "--dog", ArgsValType::LIST_STRING, "haha"});
    jap.setArgument({"arg5", "-f", "--full", ArgsValType::LIST_STRING, "haha", false});

    CHECK(jap.parseArgs() == false);
}

TEST_CASE("test for value failed - required failed")
{
    int argc{7};
    const char *argv[]{
        "ut",
        "-a", "123",
        "--cry", "-1,0,3.14,8888888",
        "-d", "hello,hi,bonjor"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", ArgsValType::FLOAT, "haha"});
    jap.setArgument({"arg2", "-b", "--boy", ArgsValType::STRING, "haha"});
    jap.setArgument({"arg3", "-c", "--cry", ArgsValType::LIST_FLOAT, "haha"});
    jap.setArgument({"arg4", "-d", "--dog", ArgsValType::LIST_STRING, "haha"});
    jap.setArgument({"arg5", "-f", "--full", ArgsValType::LIST_STRING, "haha", false});

    CHECK(jap.parseArgs() == false);
}

// ------------------------------------------------
// ------- position value test - end --------------

// ------------------------------------------------
// ------- action arguments test - start ----------
TEST_CASE("test for action success")
{
    bool flag1{false}, flag2{false}, flag3{false};

    int argc{3};
    const char *argv[]{
        "ut",
        "-a",
        "--boy"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", [&]()
                     { flag1 = true; },
                     "haha"});
    jap.setArgument({"arg2", "-b", "--boy", [&]()
                     { flag2 = true; },
                     "haha"});
    jap.setArgument({"arg3", "-c", "--cry", [&]()
                     { flag3 = true; },
                     "haha"});

    CHECK(jap.parseArgs() == true);
    CHECK(flag1 == true);
    CHECK(flag2 == true);

    UT_NO_ARG("NOARG");
}

// ------------------------------------------------
// ------- action arguments test - end ------------

// ------------------------------------------------
// ------- unknown arguments test - start ---------
TEST_CASE("test for action success")
{
    bool flag1{false}, flag2{false}, flag3{false};

    int argc{5};
    const char *argv[]{
        "ut",
        "-t",
        "--yo",
        "--internet", "123"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", [&]()
                     { flag1 = true; },
                     "haha"});
    jap.setArgument({"arg2", "-b", "--boy", [&]()
                     { flag2 = true; },
                     "haha"});
    jap.setArgument({"arg3", "-c", "--cry", [&]()
                     { flag3 = true; },
                     "haha"});

    CHECK(jap.parseArgs() == false);
}

// ------------------------------------------------
// ------- unknown arguments test - end -----------

// ------------------------------------------------
// ------- no arguments test - start --------------
TEST_CASE("test for action success")
{
    bool flag1{false}, flag2{false}, flag3{false};

    int argc{1};
    const char *argv[]{
        "ut"};
    JArgsParser jap(argc, argv, "desc", "copyright", "1.0.0");
    jap.setArgument({"arg1", "-a", "--aha", [&]()
                     { flag1 = true; },
                     "haha"});
    jap.setArgument({"arg2", "-b", "--boy", [&]()
                     { flag2 = true; },
                     "haha"});
    jap.setArgument({"arg3", "-c", "--cry", [&]()
                     { flag3 = true; },
                     "haha"});

    CHECK(jap.parseArgs() == true);
}

// ------------------------------------------------
// ------- unknown arguments test - end -----------