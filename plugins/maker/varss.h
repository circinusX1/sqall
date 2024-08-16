#ifndef VARSS_H
#define VARSS_H

#include <string>
#include <map>
#include "simplesquirrel/simplesquirrel.hpp"

class Env
{
public:
    Env(const std::string& appname);
    ~Env(){}

    ssq::Table get();
    std::string get(const std::string& key);
private:
    std::map<std::string, std::string> vars_;

};

#endif // VARSS_H
