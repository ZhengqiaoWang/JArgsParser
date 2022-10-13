#include "jargs_parser_api.h"
#include "jargs_parser_impl.h"

namespace Joger
{

    JArgsParser::JArgsParser(int argc, char const *argv[], const std::string &description, const std::string &copyright, const std::string& version) noexcept
    {
        pimpl = new JArgsParserImpl(argc, argv, description, copyright, version);
    }

    JArgsParser::~JArgsParser() noexcept
    {
        // 非线程安全
        if (pimpl != nullptr)
        {
            delete pimpl;
            pimpl = nullptr;
        }
    }

    JArgsParser &JArgsParser::setArgument(const std::string &key, const ArgsDesc &args_desc)
    {
        pimpl->setArgument(key, args_desc);
        return *this;
    }

    bool JArgsParser::parseArgs()
    {
        return pimpl->parseArgs();
    }

    template <typename ValueType>
    bool JArgsParser::getArgument(const std::string &key, ValueType &out_val)
    {
        return getArgument(key, out_val);
    }

};