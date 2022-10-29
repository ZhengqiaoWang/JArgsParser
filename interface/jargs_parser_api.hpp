/**
 * @file jargs_parser_api.hpp
 * @brief A header-only c++ argument parser lib
 * @version 1.0.0
 * @date 2022-10-26
 * 
 * @url https://github.com/ZhengqiaoWang/JArgsParser
 * 
 */
#ifndef JARGS_PARSER_API_H
#define JARGS_PARSER_API_H

/****************************************************
MIT License

Copyright (c) 2022 ZhengqiaoWang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#include <unordered_map>
#include <map>
#include <functional>
#include <vector>
#include <assert.h>
#include <typeinfo>
#include <algorithm>

namespace Joger
{
    namespace ArgsParser
    {
        /**
         * @brief Arguments types
         * 
         */
        enum ArgsType : unsigned int
        {
            POSITION = 0,
            FLAG = 1,
            VALUE = 2,
            ACTION = 3
        };

        /**
         * @brief Argument value type
         * 
         */
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

        /**
         * @brief Argument description
         * 
         */
        class ArgsDesc
        {
        public:
            /**
             * @brief position argument description
             * 
             * @param key key to get argument
             * @param arg_val_type arg value type
             * @param description arg info, display when call for help
             */
            ArgsDesc(const std::string &key,
                     const ArgsValType &arg_val_type,
                     const std::string description) : key(key),
                                                      arg_type(ArgsType::POSITION),
                                                      arg_val_type(arg_val_type),
                                                      description(description),
                                                      required(true)
            {
                arg_width = key.size();
            }

            /**
             * @brief flag argument description
             * 
             * @param key key to get argument
             * @param short_name short arg like -a -b -c
             * @param full_name full arg like --aaa --bbb --ccc
             * @param description arg info, display when call for help
             */
            ArgsDesc(const std::string &key,
                     const std::string &short_name,
                     const std::string &full_name,
                     const std::string description) : key(key),
                                                      arg_type(ArgsType::FLAG),
                                                      arg_val_type(ArgsValType::NOTYPE),
                                                      short_name(short_name),
                                                      full_name(full_name),
                                                      description(description),
                                                      required(false)
            {
                arg_width = short_name.size() + full_name.size() + 1;
            }

            /**
             * @brief value argument description
             * 
             * @param key key to get argument
             * @param short_name short arg like -a -b -c
             * @param full_name full arg like --aaa --bbb --ccc
             * @param arg_val_type arg value type
             * @param description arg info, display when call for help
             * @param required set argument is required or not, true required, false not required.
             */
            ArgsDesc(const std::string &key,
                     const std::string &short_name,
                     const std::string &full_name,
                     const ArgsValType &arg_val_type,
                     const std::string description,
                     bool required = true) : key(key),
                                             arg_type(ArgsType::VALUE),
                                             arg_val_type(arg_val_type),
                                             short_name(short_name),
                                             full_name(full_name),
                                             description(description),
                                             required(required)
            {
                arg_width = short_name.size() + full_name.size() + 1;
            }

            /**
             * @brief action argument description
             * 
             * @param key key to get argument
             * @param short_name short arg like -a -b -c
             * @param full_name full arg like --aaa --bbb --ccc
             * @param action an void(void) function call when user use this argument
             * @param description arg info, display when call for help
             */
            ArgsDesc(const std::string &key,
                     const std::string &short_name,
                     const std::string &full_name,
                     const std::function<void(void)> &action,
                     const std::string description) : key(key),
                                                      arg_type(ArgsType::ACTION),
                                                      arg_val_type(ArgsValType::NOTYPE),
                                                      short_name(short_name),
                                                      full_name(full_name),
                                                      description(description),
                                                      required(false),
                                                      action(action)
            {
                arg_width = short_name.size() + full_name.size() + 1;
            }

            ArgsDesc()
            {
            }

        public:
            std::string key{""};                  ///< primary key for get arg value
            ArgsType arg_type{ArgsType::POSITION};                ///< arg type
            ArgsValType arg_val_type{ArgsValType::STRING};         ///< arg value type
            std::string short_name{""};           ///< short arg
            std::string full_name{""};            ///< full(long) arg
            std::string description{""};          ///< arg info
            bool required{true};             ///< required flag
            std::function<void(void)> action; ///< what function after action arg called
            size_t arg_width{0};                 ///< store the width of both short_name and full_name to display help
        };

        static bool isInterger(const std::string &in_val);
        static bool isFloat(const std::string &in_val);
        static std::vector<std::string> splitString(const std::string &in_val, const char &spliter);

        /**
         * @brief JArgsParser
         * 
         * get document or help from GitHub: https://github.com/ZhengqiaoWang/JArgsParser
         * 
         */
        class JArgsParser
        {
        public:
            /**
             * @brief Construct a new JArgsParser object
             * 
             * @param argc arg num
             * @param argv arg char* array
             * @param description application's description
             * @param copyright application's copyright
             * @param version application's version string
             */
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
            /**
             * @brief Set an argument
             * 
             * @param args_desc argument description
             * @return JArgsParser& JArgsParser
             */
            JArgsParser &setArgument(const ArgsDesc &args_desc)
            {
                auto &key = args_desc.key;
                switch (args_desc.arg_type)
                {
                case ArgsType::POSITION:
                {
                    m_key_func[key] = std::bind(&JArgsParser::posArgParser, this, key, args_desc);
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
                return *this;
            }

            /**
             * @brief start to parse arguments and check
             * 
             * @return true success
             * @return false failed
             */
            bool parseArgs()
            {
                if (m_argc <= 1)
                {
                    for (auto &arg : m_arg_map)
                    {
                        for (auto &arg_desc_iter : arg.second)
                        {
                            if (arg_desc_iter.second.required)
                            {
                                showHelp();
                                return false;
                            }
                        }
                    }
                    return true;
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

                // unknown args:
                if (!m_argv_vec.empty())
                {
                    printf("ERROR: unknown arguments: ");
                    for (auto &item : m_argv_vec)
                    {
                        printf("%s ", item.c_str());
                    }
                    printf("\n");
                    showHelp();
                    return false;
                }

                return true;
            }

            /**
             * @brief Get argument by key
             * 
             * @tparam ValueType local argument type
             * @param key key setted in setArgument
             * @param out_val argument value
             * @return true success
             * @return false failed(not found)
             */
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
            /**
             * @brief show help
             * 
             */
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
                printf("\n\n");
                printf("%s\n", m_description.c_str());
                printf("Options and arguments:\n");

                for (auto iter : m_arg_map)
                {
                    for (auto arg : iter.second)
                    {
                        printf("  %-*s: %s {%s|%s}\n",
                               m_arg_max_length + 2,
                               arg.second.required ? arg.second.arg_type == ArgsType::POSITION ? arg.first.c_str() : (arg.second.short_name + "|" + arg.second.full_name).c_str() : (arg.second.short_name + "|" + arg.second.full_name).c_str(),
                               arg.second.description.c_str(),
                               arg.second.required ? "req" : "opt",
                               getArgsValTypeString(arg.second.arg_val_type).c_str());
                    }
                }
                printf("%s\n", m_copyright.c_str());
                fflush(stdout);
            }

        private:
            bool posArgParser(const std::string &key, const ArgsDesc &args_desc)
            {
                if (m_argv_vec.size() == 0)
                {
                    return false;
                }
                if (false == typeCheck(args_desc.arg_val_type, m_argv_vec[0]))
                {
                    showErrorMsg(key, args_desc, "type check failed");
                    return false;
                }
                m_key_val[key].clear();
                m_key_val[key].emplace_back(std::move(m_argv_vec[0]));

                m_argv_vec.erase(m_argv_vec.begin());
                return true;
            }

            bool flagArgParser(const std::string &key, const ArgsDesc &args_desc)
            {
                m_key_val[key].clear();
                m_key_val[key].emplace_back("F");
                for (auto iter = m_argv_vec.begin(); iter != m_argv_vec.end(); ++iter)
                {
                    auto &item = *iter;
                    if (item == args_desc.short_name || item == args_desc.full_name)
                    {
                        m_key_val[key][0] = "T";
                        m_argv_vec.erase(iter);
                        break;
                    }
                }
                return true;
            }
            bool valArgParser(const std::string &key, const ArgsDesc &args_desc)
            {
                m_key_val[key].clear();
                bool get_value{false};
                for (auto iter = m_argv_vec.begin(); iter != m_argv_vec.end(); ++iter)
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
                        auto key_iter = iter - 1;
                        m_argv_vec.erase(iter);
                        m_argv_vec.erase(key_iter);
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
                for (auto iter = m_argv_vec.begin(); iter != m_argv_vec.end(); ++iter)
                {
                    auto &item = *iter;
                    if (item == args_desc.short_name || item == args_desc.full_name)
                    {
                        args_desc.action();
                        m_argv_vec.erase(iter);
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
                    return "int";
                }
                case ArgsValType::FLOAT:
                {
                    return "float";
                }
                case ArgsValType::STRING:
                {
                    return "string";
                }
                case ArgsValType::LIST_INT:
                {
                    return "vec<int>";
                }
                case ArgsValType::LIST_FLOAT:
                {
                    return "vec<float>";
                }
                case ArgsValType::LIST_STRING:
                {
                    return "vec<string>";
                }
                default:
                {
                    return "flag";
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
            std::map<std::string, std::function<bool(void)>> m_key_func;
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
            for (size_t i = 0; i < in_val.size(); ++i)
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
            for (size_t i = 0; i < in_val.size(); ++i)
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

            for (size_t i = 0; i < in_val.size(); ++i)
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
};
#endif
