#ifndef TRANSFER_UTILS_H
#define TRANSFER_UTILS_H
#include <string>
#include <vector>
namespace Joger
{

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

    static std::vector<std::string> splitString(const std::string& in_val, const char& spliter)
    {
        std::vector<std::string> result;
        int start_pos{0};

        for(int i=0;i<in_val.size();++i)
        {
            if(in_val.at(i) == spliter)
            {
                result.emplace_back(in_val.substr(start_pos, i-start_pos));
                start_pos = i + 1;
            }
        }
        result.emplace_back(in_val.substr(start_pos, in_val.size() - start_pos));
        return result;
    }

}

#endif