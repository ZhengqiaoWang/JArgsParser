#ifndef JARGS_PARSER_API_H
#define JARGS_PARSER_API_H

#include <string>
#include <vector>
#include <functional>

namespace Joger
{
    class JArgsParserImpl;
    enum class ArgsType;
    enum class ArgsValType;

    class ArgsDesc;

    class JArgsParser
    {

    public:
        JArgsParser(int argc, char const *argv[], const std::string &description, const std::string &copyright, const std::string& version) noexcept;
        ~JArgsParser() noexcept;

        JArgsParser &setArgument(const std::string &key, const ArgsDesc &args_desc);
        bool parseArgs();

        template <typename ValueType>
        bool getArgument(const std::string &key, ValueType &out_val);

    private:
        JArgsParserImpl *pimpl{nullptr};
    };

    enum class ArgsType
    {
        POSITION,
        FLAG,
        VALUE,
        ACTION
    };

    enum class ArgsValType
    {
        INT,
        FLOAT,
        STRING,
        LIST_INT,
        LIST_FLOAT,
        LIST_STRING
    };

    class ArgsDesc
    {
    public:
        ArgsDesc(const ArgsValType &arg_val_type,
                 const std::string description) : arg_val_type(arg_val_type),
                                                  description(description),
                                                  arg_type(ArgsType::POSITION) {}
        ArgsDesc(const std::string &short_name,
                 const std::string &full_name,
                 const std::string description) : short_name(short_name),
                                                  full_name(full_name),
                                                  description(description),
                                                  arg_type(ArgsType::FLAG) {}
        ArgsDesc(const std::string &short_name,
                 const std::string &full_name,
                 const ArgsValType &arg_val_type,
                 const std::string description,
                 bool required = true) : short_name(short_name),
                                         full_name(full_name),
                                         required(required),
                                         description(description),
                                         arg_val_type(arg_val_type),
                                         arg_type(ArgsType::VALUE) {}
        ArgsDesc(const std::string &short_name,
                 const std::string &full_name,
                 const std::function<void(const std::string &)> &action,
                 const std::string description) : short_name(short_name),
                                                  full_name(full_name),
                                                  description(description),
                                                  action(action),
                                                  arg_type(ArgsType::ACTION) {}

    public:
        ArgsType arg_type;
        ArgsValType arg_val_type;
        std::string short_name, full_name, description;
        bool required{false};
        std::function<void(const std::string &)> action;
    };
};

#endif