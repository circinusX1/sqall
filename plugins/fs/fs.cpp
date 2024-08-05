
#include <string>
#include <filesystem>
#include "fs.h"
#include "fs_global.h"

namespace fs = std::filesystem;
static  ssq::VM* PVM;
sq_api*  SQ_PTRS;

void directory_iterator(const ssq::Function& f, const std::string& dir)
{
    std::string path = dir, sss;
    if(path.empty())
        path="./";
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        const auto p = entry.path();
        sss = entry.path().string().c_str();
        if(sss.find_first_of("./")==0)
        {
            sss.erase(0,1);
        }
        if(!f.isNull())
        {
            PVM->callFunc(f, *PVM, sss);
        }
    }
}

std::string current_path()
{
    static std::string path = std::filesystem::current_path().c_str();
    return path;
}

size_t file_size(const char* file)
{
    std::filesystem::file_status s=std::filesystem::status(file);
    if(std::filesystem::is_regular_file(s))
    {
        return std::filesystem::file_size(file);
    }
    return -1;
}

bool _init_apis(ssq::VM* pvm, sq_api* ptrs)
{
    PVM = pvm;
    SQ_PTRS = ptrs;
    pvm->addFunc("directory_iterator",directory_iterator);
    pvm->addFunc("current_path",current_path);
    pvm->addFunc("file_size",file_size);
    return false;
}

void _deinit_apis(ssq::VM* pvm, sq_api* ptrs)
{
}


