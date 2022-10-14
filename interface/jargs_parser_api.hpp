#ifndef JARGS_PARSER_API_H
#define JARGS_PARSER_API_H
#include <unordered_map>
#include <map>
#include <functional>
#include <vector>
#include <assert.h>
#include <typeinfo>
#include <algorithm>

namespace Joger
{
    enum ArgsType : unsigned int
    {
        POSITION = 0,
        FLAG = 1,
        VALUE = 2,
        ACTION = 3
    };

    enum class ArgsValType
    {
        INT,
        FLOAT,
        STRING,
        LIST_INT,
        LIST_FLOAT,
        LIST_STRING,
        NOTYPE
    };
    class ArgsDesc
    {
    public:
        ArgsDesc(const std::string &key,
                 const ArgsValType &arg_val_type,
                 const std::string description) : key(key),
                                                  arg_val_type(arg_val_type),
                                                  description(description),
                                                  arg_type(ArgsType::POSITION),
                                                  required(true)
        {
            arg_width = key.size();
        }
        ArgsDesc(const std::string &key,
                 const std::string &short_name,
                 const std::string &full_name,
                 const std::string description) : key(key),
                                                  short_name(short_name),
                                                  full_name(full_name),
                                                  description(description),
                                                  arg_type(ArgsType::FLAG),
                                                  required(false),
                                                  arg_val_type(ArgsValType::NOTYPE)
        {
            arg_width = short_name.size() + full_name.size() + 1;
        }
        ArgsDesc(const std::string &key,
                 const std::string &short_name,
                 const std::string &full_name,
                 const ArgsValType &arg_val_type,
                 const std::string description,
                 bool required = true) : key(key),
                                         short_name(short_name),
                                         full_name(full_name),
                                         required(required),
                                         description(description),
                                         arg_val_type(arg_val_type),
                                         arg_type(ArgsType::VALUE)
        {
            arg_width = short_name.size() + full_name.size() + 1;
        }
        ArgsDesc(const std::string &key,
                 const std::string &short_name,
                 const std::string &full_name,
                 const std::function<void(const std::string &)> &action,
                 const std::string description) : key(key),
                                                  short_name(short_name),
                                                  full_name(full_name),
                                                  description(description),
                                                  action(action),
                                                  arg_type(ArgsType::ACTION),
                                                  required(false),
                                                  arg_val_type(ArgsValType::NOTYPE)
        {
            arg_width = short_name.size() + full_name.size() + 1;
        }

        ArgsDesc()
        {
        }

    public:
        std::string key;
        ArgsType arg_type;
        ArgsValType arg_val_type;
        std::string short_name, full_name, description;
        bool required{false};
        std::function<void(const std::string &)> action;
        int arg_width{0};
    };

    static bool isInterger(const std::string &in_val);
    static bool isFloat(const std::string &in_val);
    static std::vector<std::string> splitString(const std::string &in_val, const char &spliter);

    class JArgsParser
    {
    public:
        JArgsParser(int argc,
                    char const *argv[],
                    const std::string &description,
                    const std::string &copyright,
                    const std::string &version) noexcept : m_argc(argc),
                                                           m_app_file_name(argv[0]),
                                                           m_description(description),
                                                           m_copyright(copyright),
                                                           m_version_txt(version)
        {
            for (int i = 1; i < argc; ++i)
            {
                m_argv_vec.emplace_back(argv[i]);
            }
        }

    public:
        void setArgument(const ArgsDesc &args_desc)
        {
            auto &key = args_desc.key;
            switch (args_desc.arg_type)
            {
            case ArgsType::POSITION:
            {
                m_key_func[key] = std::bind(&JArgsParser::posArgParser, this, key, m_cur_pos, args_desc);
                ++m_cur_pos;
                break;
            }
            case ArgsType::FLAG:
            {
                m_key_func[key] = std::bind(&JArgsParser::flagArgParser, this, key, args_desc);
                break;
            }
            case ArgsType::VALUE:
            {
                m_key_func[key] = std::bind(&JArgsParser::valArgParser, this, key, args_desc);
                break;
            }
            case ArgsType::ACTION:
            {
                m_key_func[key] = std::bind(&JArgsParser::actArgParser, this, key, args_desc);
                break;
            }

            default:
            {
                showErrorMsg(key, args_desc, "regist an invalid args type");
                assert(false); // invalid ArgsType
                break;
            }
            }
            m_arg_map[args_desc.arg_type][key] = args_desc;
            m_arg_max_length = args_desc.arg_width > m_arg_max_length ? args_desc.arg_width : m_arg_max_length;
        }
        bool parseArgs()
        {
            if (m_argc <= 1)
            {
                showHelp();
                exit(0);
            }
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

        template <typename ValueType>
        bool getArgument(const std::string &key, ValueType &out_val)
        {
            if (m_key_val.find(key) == m_key_val.end())
            {
                return false;
            }
            return getArgument(key, out_val);
        }

    public:
        void showHelp() noexcept
        {
            printf("usage: %s ", m_app_file_name.c_str());
            for (auto iter : m_arg_map)
            {
                for (auto arg : iter.second)
                {
                    static std::string format_str_position = "%s(%s) ";
                    static std::string format_str_required = "(%s(%s)) ";
                    static std::string format_str_optional = "[%s(%s)] ";

                    printf(
                        arg.second.required ? arg.second.arg_type == ArgsType::POSITION ? format_str_position.c_str() : format_str_required.c_str()
                                            : format_str_optional.c_str(),
                        arg.second.required ? arg.second.arg_type == ArgsType::POSITION ? arg.first.c_str() : (arg.second.short_name + "|" + arg.second.full_name).c_str() : (arg.second.short_name + "|" + arg.second.full_name).c_str(),
                        getArgsValTypeString(arg.second.arg_val_type).c_str());
                }
            }
            printf("\n");
            printf("%s\n", m_description.c_str());
            printf("Options and arguments (and corresponding environment variables):\n");

            for (auto iter : m_arg_map)
            {
                for (auto arg : iter.second)
                {
                    printf("  %-*s: [%11s|%s]: %s\n",
                           m_arg_max_length + 4,
                           arg.second.required ? arg.second.arg_type == ArgsType::POSITION ? arg.first.c_str() : (arg.second.short_name + "|" + arg.second.full_name).c_str() : (arg.second.short_name + "|" + arg.second.full_name).c_str(),
                           getArgsValTypeString(arg.second.arg_val_type).c_str(),
                           arg.second.required ? "REQ" : "OPT",
                           arg.second.description.c_str());
                }
            }
            printf("%s\n", m_copyright.c_str());
            fflush(stdout);
        }

    private:
        bool posArgParser(const std::string &key, unsigned int pos, const ArgsDesc &args_desc)
        {
            if (false == pos < m_argv_vec.size())
            {
                return false;
            }
            if (false == typeCheck(args_desc.arg_val_type, m_argv_vec[pos]))
            {
                showErrorMsg(key, args_desc, "type check failed");
                return false;
            }
            m_key_val[key].clear();
            m_key_val[key].emplace_back(std::move(m_argv_vec[pos]));

            return true;
        }
        bool flagArgParser(const std::string &key, const ArgsDesc &args_desc)
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
        bool valArgParser(const std::string &key, const ArgsDesc &args_desc)
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

                    if (ArgsValType::LIST_INT == args_desc.arg_val_type || ArgsValType::LIST_FLOAT == args_desc.arg_val_type || ArgsValType::LIST_STRING == args_desc.arg_val_type)
                    {
                        m_key_val[key] = splitString(*iter, ',');
                    }
                    else
                    {
                        m_key_val[key].emplace_back(*iter);
                    }

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
        bool actArgParser(const std::string &key, const ArgsDesc &args_desc)
        {
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

    private:
        bool typeCheck(ArgsValType target_type, const std::string &in_val)
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
        void showErrorMsg(const std::string &key, const ArgsDesc &args_desc, const std::string desc)
        {
            printf("ERROR: Failed to parse [%s|%s] for [%s] because of: %s\n", args_desc.short_name.c_str(), args_desc.full_name.c_str(), key.c_str(), desc.c_str());
        }
        ArgsDesc &getArgsDesc(const std::string &key)
        {
            for (auto &type_map : m_arg_map)
            {
                for (auto &key_arg_iter : type_map.second)
                {
                    if (key_arg_iter.first == key)
                    {
                        return key_arg_iter.second;
                    }
                }
            }
            assert(false); // cannot find key
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
                return "FLAG";
            }
            }
        }

    private:
        int m_argc{0};                   ///< 参数数量
        std::string m_app_file_name{""}; ///< 程序名称
        std::string m_description{""};
        std::string m_copyright{""};
        std::string m_version_txt{""};
        std::vector<std::string> m_argv_vec; ///< 参数存放

        std::map<ArgsType, std::map<std::string, ArgsDesc>, std::less<ArgsType>> m_arg_map;
        unsigned int m_cur_pos{0};
        std::unordered_map<std::string, std::function<bool(void)>> m_key_func;
        std::unordered_map<std::string, std::vector<std::string>> m_key_val;

    private:
        // display
        unsigned int m_arg_max_length{0};
    };

    // ----- 特化 -----
    template <>
    inline bool JArgsParser::getArgument<std::string>(const std::string &key, std::string &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        if (false == typeCheck(desc.arg_val_type, m_key_val[key][0]))
        {
            return false;
        }
        out_val = m_key_val[key][0];
        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<long>(const std::string &key, long &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        if (false == typeCheck(desc.arg_val_type, m_key_val[key][0]))
        {
            return false;
        }
        out_val = atoi(m_key_val[key][0].c_str());
        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<int>(const std::string &key, int &out_val)
    {
        long tmp_val{0};
        auto ret = getArgument<long>(key, tmp_val);
        if (ret)
        {
            out_val = static_cast<int>(tmp_val);
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<unsigned int>(const std::string &key, unsigned int &out_val)
    {
        long tmp_val{0};
        auto ret = getArgument<long>(key, tmp_val);
        if (ret)
        {
            out_val = static_cast<unsigned int>(tmp_val);
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<unsigned long>(const std::string &key, unsigned long &out_val)
    {
        long tmp_val{0};
        auto ret = getArgument<long>(key, tmp_val);
        if (ret)
        {
            out_val = static_cast<unsigned long>(tmp_val);
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<double>(const std::string &key, double &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        if (false == typeCheck(desc.arg_val_type, m_key_val[key][0]))
        {
            return false;
        }
        out_val = atof(m_key_val[key][0].c_str());
        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<float>(const std::string &key, float &out_val)
    {
        double tmp_val{0};
        auto ret = getArgument<double>(key, tmp_val);
        if (ret)
        {
            out_val = static_cast<float>(tmp_val);
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<bool>(const std::string &key, bool &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        if (false == typeCheck(desc.arg_val_type, m_key_val[key][0]))
        {
            return false;
        }
        if (m_key_val[key][0] == "T")
        {
            out_val = true;
        }
        else if (m_key_val[key][0] == "F")
        {
            out_val = false;
        }
        else
        {
            return false;
        }

        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<std::string>>(const std::string &key, std::vector<std::string> &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        out_val = result_vec;
        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<long>>(const std::string &key, std::vector<long> &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        for (auto &item : result_vec)
        {
            if (false == typeCheck(desc.arg_val_type, item))
            {
                return false;
            }
        }
        std::for_each(result_vec.cbegin(), result_vec.cend(), [&out_val](const std::string &val)
                      { out_val.emplace_back(atoi(val.c_str())); });
        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<int>>(const std::string &key, std::vector<int> &out_val)
    {
        std::vector<long> tmp_vec;
        auto ret = getArgument(key, tmp_vec);
        if (ret)
        {
            std::for_each(tmp_vec.cbegin(), tmp_vec.cend(), [&out_val](const long &val)
                          { out_val.emplace_back(static_cast<int>(val)); });
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<unsigned long>>(const std::string &key, std::vector<unsigned long> &out_val)
    {
        std::vector<long> tmp_vec;
        auto ret = getArgument(key, tmp_vec);
        if (ret)
        {
            std::for_each(tmp_vec.cbegin(), tmp_vec.cend(), [&out_val](const long &val)
                          { out_val.emplace_back(static_cast<unsigned long>(val)); });
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<unsigned int>>(const std::string &key, std::vector<unsigned int> &out_val)
    {
        std::vector<long> tmp_vec;
        auto ret = getArgument(key, tmp_vec);
        if (ret)
        {
            std::for_each(tmp_vec.cbegin(), tmp_vec.cend(), [&out_val](const long &val)
                          { out_val.emplace_back(static_cast<unsigned int>(val)); });
        }
        return ret;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<double>>(const std::string &key, std::vector<double> &out_val)
    {
        if (m_key_val.find(key) == m_key_val.end())
        {
            return false;
        }
        auto &desc = getArgsDesc(key);
        auto &result_vec = m_key_val[key];
        if (result_vec.empty())
        {
            return false;
        }
        for (auto &item : result_vec)
        {
            if (false == typeCheck(desc.arg_val_type, item))
            {
                return false;
            }
        }
        std::for_each(result_vec.cbegin(), result_vec.cend(), [&out_val](const std::string &val)
                      { out_val.emplace_back(atof(val.c_str())); });
        return true;
    }

    template <>
    inline bool JArgsParser::getArgument<std::vector<float>>(const std::string &key, std::vector<float> &out_val)
    {
        std::vector<double> tmp_vec;
        auto ret = getArgument(key, tmp_vec);
        if (ret)
        {
            std::for_each(tmp_vec.cbegin(), tmp_vec.cend(), [&out_val](const double &val)
                          { out_val.emplace_back(static_cast<float>(val)); });
        }
        return ret;
    }

    static bool isInterger(const std::string &in_val)
    {
        for (int i = 0; i < in_val.size(); ++i)
        {
            if (in_val.at(i) == '-' && in_val.size() > 0)
            {
                continue;
            }
            if (in_val.at(i) > '9' || in_val.at(i) < '0')
            {
                return false;
            }
        }
        return true;
    }

    static bool isFloat(const std::string &in_val)
    {
        int dot_cnt{0};
        for (int i = 0; i < in_val.size(); ++i)
        {
            if (in_val.at(i) == '-' && in_val.size() > 0)
            {
                continue;
            }
            if (in_val.at(i) == '.' && i > 0)
            {
                ++dot_cnt;
                if (dot_cnt > 1)
                {
                    return false;
                }
                continue;
            }
            if (in_val.at(i) > '9' || in_val.at(i) < '0')
            {
                return false;
            }
        }
        return true;
    }

    static std::vector<std::string> splitString(const std::string &in_val, const char &spliter)
    {
        std::vector<std::string> result;
        int start_pos{0};

        for (int i = 0; i < in_val.size(); ++i)
        {
            if (in_val.at(i) == spliter)
            {
                result.emplace_back(in_val.substr(start_pos, i - start_pos));
                start_pos = i + 1;
            }
        }
        result.emplace_back(in_val.substr(start_pos, in_val.size() - start_pos));
        return result;
    }
};
#endif