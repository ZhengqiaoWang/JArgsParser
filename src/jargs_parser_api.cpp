#include "jargs_parser_api.hpp"


namespace Joger
{
  
   



    // ----- 特化 -----
    template <>
    bool JArgsParser::getArgument<std::string>(const std::string &key, std::string &out_val)
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
    bool JArgsParser::getArgument<long>(const std::string &key, long &out_val)
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
    bool JArgsParser::getArgument<int>(const std::string &key, int &out_val)
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
    bool JArgsParser::getArgument<unsigned int>(const std::string &key, unsigned int &out_val)
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
    bool JArgsParser::getArgument<unsigned long>(const std::string &key, unsigned long &out_val)
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
    bool JArgsParser::getArgument<double>(const std::string &key, double &out_val)
    {
        printf("%s\n", __func__);
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
    bool JArgsParser::getArgument<float>(const std::string &key, float &out_val)
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
    bool JArgsParser::getArgument<bool>(const std::string &key, bool &out_val)
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
    bool JArgsParser::getArgument<std::vector<std::string>>(const std::string &key, std::vector<std::string> &out_val)
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
    bool JArgsParser::getArgument<std::vector<long>>(const std::string &key, std::vector<long> &out_val)
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
    bool JArgsParser::getArgument<std::vector<int>>(const std::string &key, std::vector<int> &out_val)
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
    bool JArgsParser::getArgument<std::vector<unsigned long>>(const std::string &key, std::vector<unsigned long> &out_val)
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
    bool JArgsParser::getArgument<std::vector<unsigned int>>(const std::string &key, std::vector<unsigned int> &out_val)
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
    bool JArgsParser::getArgument<std::vector<double>>(const std::string &key, std::vector<double> &out_val)
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
    bool JArgsParser::getArgument<std::vector<float>>(const std::string &key, std::vector<float> &out_val)
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
}