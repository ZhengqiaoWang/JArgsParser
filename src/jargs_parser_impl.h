#ifndef JARGS_PARSER_IMPL_H
#define JARGS_PARSER_IMPL_H
#include "jargs_parser_api.h"
#include <unordered_map>

namespace Joger
{
    class JArgsParserImpl
    {
    public:
        JArgsParserImpl(int argc,
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
        void setArgument(const std::string &key, const ArgsDesc &args_desc);
        bool parseArgs();

        template <typename ValueType>
        bool getArgument(const std::string &key, ValueType &out_val);

    public:
        void showHelp() noexcept;

    private:
        bool posArgParser(const std::string &key, unsigned int pos, const ArgsDesc &args_desc);
        bool flagArgParser(const std::string &key, const ArgsDesc &args_desc);
        bool valArgParser(const std::string &key, const ArgsDesc &args_desc);
        bool actArgParser(const std::string &key, const ArgsDesc &args_desc);
    
    private:
        bool typeCheck(ArgsValType target_type, const std::string& in_val);
        void showErrorMsg(const std::string &key, const ArgsDesc &args_desc, const std::string desc);

    private:
        int m_argc{0};                   ///< 参数数量
        std::string m_app_file_name{""}; ///< 程序名称
        std::string m_description{""};
        std::string m_copyright{""};
        std::string m_version_txt{""};
        std::vector<std::string> m_argv_vec;   ///< 参数存放

        std::vector<std::pair<std::string, ArgsDesc>> m_param_desc;     ///< 其他参数描述
        std::vector<std::pair<std::string, ArgsDesc>> m_pos_param_desc; ///< 位置参数说明
        unsigned int m_cur_pos{0};
        std::unordered_map<std::string, std::function<bool(void)>> m_key_func;
        std::unordered_map<std::string, std::vector<std::string>> m_key_val;
    };
};
#endif