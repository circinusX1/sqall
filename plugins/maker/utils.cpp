
#include <iostream>
#include "utils.h"

std::string proc(const char* cmd, int& exits, bool verbose)
{
    char buffer[128];
    std::string result = "";
    int iclose=0;
    FILE* pipe = popen(cmd, "r");
    if (pipe)
    {
        try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) {
                if(verbose)
                {
                    std::cout << buffer;
                    std::cout.flush();
                }
                result += buffer;
            }
        }
        catch (...)
        {
            iclose=pclose(pipe);
            pipe=0;
        }
    }
    if(pipe)
        iclose = pclose(pipe);
    exits = WEXITSTATUS(iclose);
    return result;
}

size_t split_str(const std::string &str, char delim,
                        std::vector<std::string> &a)
{
    if (str.empty())
        return 0;

    std::string token;
    size_t prev = 0, pos = 0;
    size_t sl = str.length();
    a.clear();
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos)
            pos = sl;
        token = str.substr(prev, pos - prev);
        if (!token.empty())
            a.push_back(token);
        prev = pos + 1;
    } while (pos < sl && prev < sl);
    return a.size();
}

