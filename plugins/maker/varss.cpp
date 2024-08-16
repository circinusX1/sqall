
#include <filesystem>
#include "varss.h"
#include "utils.h"

extern ssq::VM*    PVM;

Env::Env(const std::string& appname){
    int ec;
    vars_["ELF"]=appname;
    vars_["CC"]="gcc";
    vars_["CXX"]="g++";
    vars_["AR"]="ar";
    vars_["INCLUDES"]="";
    vars_["LIBS"]="";
    vars_["PREFIX"]="";
    vars_["FILES"]="";
    vars_["PATH"]=std::getenv("PATH");
    vars_["CUR_DIR"] = std::filesystem::current_path();
    vars_["SRC_DIR"] = "";
    vars_["OUT_DIR"] = "";
    vars_["USER"]=std::getenv("USER");
    vars_["CC_FLAGS"]="-c ";
    vars_["CXX_FLAGS"]="-c ";
    vars_["CC_LINKER_FLAGS"]="-o ";
    vars_["CXX_LINKER_FLAGS"]="-o ";
    std::string pn = ::proc("uname -m", ec);
    while(pn.back()=='\n'||pn.back()=='\r')
        pn.pop_back();
    vars_["ARCH"]=pn;
    std::cout << __FUNCTION__ <<"ctor \n";
}


ssq::Table Env::get()
{
    ssq::Table tbl(PVM->getHandle());
    for(const auto& a : vars_)
    {
        tbl.set<std::string>(a.first.c_str(), a.second);
    }
    std::cout << __FUNCTION__ <<" called \n";
    return tbl;
}

std::string Env::get(const std::string& key)
{
    const auto a = vars_.find(key);
    if(a != vars_.end())
    {
        return a->second;
    }
    return "";
}
