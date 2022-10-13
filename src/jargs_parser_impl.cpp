#include "jargs_parser_impl.h"
#include "transfer_utils.h"
#include <assert.h>
#include <typeinfo>

namespace Joger
{
    void JArgsParserImpl::setArgument(const std::string &key, const ArgsDesc &args_desc)
    {
        switch (args_desc.arg_type)
        {
        case ArgsType::POSITION:
        {
            printf("position arg, %d\n", m_cur_pos);
            m_key_func[key] = std::bind(&JArgsParserImpl::posArgParser, this, key, m_cur_pos, args_desc);
            m_pos_param_desc.emplace_back(std::make_pair(key, args_desc));
            ++m_cur_pos;
            break;
        }
        case ArgsType::FLAG:
        {
            m_key_func[key] = std::bind(&JArgsParserImpl::flagArgParser, this, key, args_desc);
            m_param_desc.emplace_back(std::make_pair(key, args_desc));
            break;
        }
        case ArgsType::VALUE:
        {
            m_key_func[key] = std::bind(&JArgsParserImpl::valArgParser, this, key, args_desc);
            m_param_desc.emplace_back(std::make_pair(key, args_desc));
            break;
        }
        case ArgsType::ACTION:
        {
            m_key_func[key] = std::bind(&JArgsParserImpl::actArgParser, this, key, args_desc);
            m_param_desc.emplace_back(std::make_pair(key, args_desc));
            break;
        }

        default:
        {
            showErrorMsg(key, args_desc, "regist an invalid args type");
            assert(false); // invalid ArgsType
            break;
        }
        }
    }

    bool JArgsParserImpl::posArgParser(const std::string &key, unsigned int pos, const ArgsDesc &args_desc)
    {
        if (false == pos < m_argv_vec.size())
        {
            return false;
        }
        if (false == typeCheck(args_desc.arg_val_type, m_argv_vec[pos]))
        {
            printf("type check failed: %s\n", m_argv_vec[pos].c_str());
            showErrorMsg(key, args_desc, "type check failed");
            return false;
        }
        m_key_val[key].clear();
        m_key_val[key].emplace_back(std::move(m_argv_vec[pos]));

        return true;
    }
    bool JArgsParserImpl::flagArgParser(const std::string &key, const ArgsDesc &args_desc)
    {
        m_key_val[key].clear();
        m_key_val[key].emplace_back("F");
        bool get_value{false};
        for (auto &item : m_argv_vec)
        {
            if (item == args_desc.short_name || item == args_desc.full_name)
            {
                m_key_val[key][0] = "T";
                get_value = true;
                break;
            }
        }
        return true;
    }
    bool JArgsParserImpl::valArgParser(const std::string &key, const ArgsDesc &args_desc)
    {
        m_key_val[key].clear();
        bool get_value{false};
        for (auto iter = m_argv_vec.cbegin(); iter != m_argv_vec.cend(); ++iter)
        {
            if ((*iter) == args_desc.short_name || (*iter) == args_desc.full_name)
            {
                iter++;
                if (iter == m_argv_vec.cend())
                {
                    showErrorMsg(key, args_desc, "value required");
                    return false;
                }

                if (false == typeCheck(args_desc.arg_val_type, *iter))
                {
                    showErrorMsg(key, args_desc, "type check failed");
                    return false;
                }
                m_key_val[key].emplace_back(*iter);
                get_value = true;
                break;
            }
        }
        if (args_desc.required && false == get_value)
        {
            showErrorMsg(key, args_desc, "arg required");
            return false;
        }
        return true;
    }
    bool JArgsParserImpl::actArgParser(const std::string &key, const ArgsDesc &args_desc)
    {
        printf("action %s\n", key.c_str());
        for (auto &item : m_argv_vec)
        {
            if (item == args_desc.short_name || item == args_desc.full_name)
            {
                args_desc.action(item);
                break;
            }
        }
        return true;
    }

    bool JArgsParserImpl::typeCheck(ArgsValType target_type, const std::string &in_val)
    {
        if (ArgsValType::INT == target_type)
        {
            return isInterger(in_val);
        }
        else if (ArgsValType::FLOAT == target_type)
        {
            return isFloat(in_val);
        }
        else if (ArgsValType::LIST_INT == target_type)
        {
            auto ret_vec = splitString(in_val, ',');
            for (auto &item : ret_vec)
            {
                if (false == typeCheck(ArgsValType::INT, item))
                {
                    return false;
                }
            }
            return true;
        }
        else if (ArgsValType::LIST_FLOAT == target_type)
        {
            auto ret_vec = splitString(in_val, ',');
            for (auto &item : ret_vec)
            {
                if (false == typeCheck(ArgsValType::FLOAT, item))
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            return true;
        }
    }

    bool JArgsParserImpl::parseArgs()
    {
        if (m_argv_vec[0] == "-h" || m_argv_vec[0] == "--help")
        {
            showHelp();
            exit(0);
        }
        else if (m_argv_vec[0] == "-v" || m_argv_vec[0] == "--version")
        {
            printf("version: %s\n", m_version_txt.c_str());
            exit(0);
        }

        for (auto iter : m_key_func)
        {
            if (false == iter.second())
            {
                showHelp();
                return false;
            }
        }

        return true;
    }

    void JArgsParserImpl::showErrorMsg(const std::string &key, const ArgsDesc &args_desc, const std::string desc)
    {
        printf("ERROR: Failed to parse [%s|%s] for [%s] because of: %s\n", args_desc.short_name.c_str(), args_desc.full_name.c_str(), key.c_str(), desc.c_str());
    }

    const std::string getArgsValTypeString(const ArgsValType &type)
    {
        switch (type)
        {
        case ArgsValType::INT:
        {
            return "INT";
        }
        case ArgsValType::FLOAT:
        {
            return "FLOAT";
        }
        case ArgsValType::STRING:
        {
            return "STRING";
        }
        case ArgsValType::LIST_INT:
        {
            return "LIST_INT";
        }
        case ArgsValType::LIST_FLOAT:
        {
            return "LIST_FLOAT";
        }
        case ArgsValType::LIST_STRING:
        {
            return "LIST_STRING";
        }
        default:
        {
            return "UNKNOWN";
        }
        }
    }

    void JArgsParserImpl::showHelp() noexcept
    {
        printf("  %s\n", m_description.c_str());
        printf("usage: %s ", m_app_file_name.c_str());
        for (auto &arg : m_pos_param_desc)
        {
            printf("%s(%s) ", arg.first.c_str(), getArgsValTypeString(arg.second.arg_val_type).c_str());
        }
        for (auto &arg : m_param_desc)
        {
            printf("[%s|%s(%s)] ", arg.second.short_name.c_str(), arg.second.full_name.c_str(), getArgsValTypeString(arg.second.arg_val_type).c_str());
        }
        printf("\n");
        }
}