
#include <string>
#include <vector>
#include <regex>
#include <unistd.h>
#include <filesystem>
#include "maker.h"
#include "varss.h"
#include "utils.h"
#include "maker_global.h"

namespace   fs = std::filesystem;
ssq::VM*    PVM;
sq_api*     SQ_PTRS;
static      std::string WorkDir;
static      std::string PrevDir;
static      char TempBuff[4096];

void directory_iterator(const ssq::Function& f, const std::string& dir)
{
    std::string path = dir, sss;
    if(path.empty())
        path="./";
    if(::access(path.c_str(),0)==0)
    {
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
}

std::string ls(const ssq::Table& env, const std::string& filter)
{
    char lwd[4096];

    std::string sss;
    std::string ret;
    std::string srcdir = env.get<std::string>((const char*)"SRC_DIR");

    if(::access(srcdir.c_str(),0)!=0)
    {
        return ret;
    }
    std::string scwd = ::getcwd(lwd,4095);
    chdir(srcdir.c_str());

    for (const auto & entry : std::filesystem::directory_iterator("./"))
    {
        const auto p = entry.path();
        sss = entry.path().string().c_str();
        if(sss.find_first_of("./")==0)
        {
            sss.erase(0,2);
        }
        try{
            //std::regex ereg(filter.c_str());

            //std::smatch m;
            //if(std::regex_search(sss,ereg))
            //{
            //    if(sss.find_first_of("./")==0)
            //    {
            //        sss.erase(0,1);
            //    }
            //    a.push(sss);
            //}
            if(sss.find(filter) != -1)
            {
                 ret.append(sss);
                 ret.append(" ");
            }
        }
        catch(std::regex_error& e)
        {
            std::cerr << "invalid regex: " << filter << "\n";
        }
    }
    chdir(scwd.c_str());

    return ret;
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

void aprint(const ssq::Array& arr)
{
    size_t l =  arr.size();
    for(size_t idx = 0; idx < l ;idx++)
    {
        std::string value = arr.get<std::string>(idx);
        std::cout << value << " ";
    }
    std::cout << "\n";
}

void tprint(const ssq::Table& tbl)
{
    ssq::Object::iterator it;
    while(tbl.next(it))
    {
        HSQOBJECT value = it.getValue();
        std::string name = it.getName();
        if(_RAW_TYPE(value._type)==_RT_STRING)
        {
            std::string sname = SQ_PTRS->sq_objtostring(&value);
            std::cout << name << "=" << sname << "\n";
        }
        else if(_RAW_TYPE(value._type)==_RT_ARRAY)
        {
            ssq::Object obj(value, PVM->getHandle());
            ssq::Array  arr = obj.toArray();
            aprint(arr);
        }
    }
}



std::string  _search_folder(const std::string& fold, const std::string& file)
{
    for (const auto & entry : std::filesystem::recursive_directory_iterator(fold))
    {
        const auto p = entry.path();
        std::string sss = entry.path().string().c_str();
        if(sss.find(file)!=-1)
        {
            return sss;
        }
    }
    return "";
}


std::string  _find_header(const std::string& cmd, const std::string& cc_flags, const std::string err)
{
    std::string cmdout;
    std::vector< std::string > vcmds;
    std::vector< std::string > vflags;
    std::vector< std::string > verr;
    std::string folder;
    ::split_str(cmd,' ', vcmds);
    ::split_str(cc_flags,' ',vflags);
    ::split_str(err,' ',verr);

    for(const auto& a : verr)
    {
        if(a.find(".h")!=-1)
        {
            for(const auto& f : vflags )
            {
                if(f.find("-I")!=-1)
                {
                    folder = _search_folder(f.substr(2),a);
                    if(!folder.empty())
                    {
                        goto DONE;
                    }
                }
            }
        }
    }
DONE:

    return cmdout;

}

ssq::Array compile(const ssq::Table& env, const std::string& compiler)
{
    ssq::Array a(PVM->getHandle());

    std::string cd = current_path();
    std::string wd = env.get<std::string>((const char*)"SRC_DIR");
    std::string cc = env.get<std::string>(compiler.c_str());
    std::string sflags = compiler + "_FLAGS";
    std::string ccflags = env.get<std::string>(sflags.c_str());
    std::string includes = env.get<std::string>("INCLUDES");

    std::string prefix_cmd = "cd "; prefix_cmd += wd + "; ";
    prefix_cmd += cc + " ";
    prefix_cmd += includes + " ";

    std::vector<std::string> aflgs;
    ::split_str(ccflags,' ', aflgs);


    // -c at end
    for(const auto& a : aflgs)
    {
        if(a=="-c")continue;
        prefix_cmd += a + " ";
    }
    for(const auto& a : aflgs)
    {
        if(a!="-c")continue;
        prefix_cmd += a;
        prefix_cmd += " ";
        break;
    }

    ssq::Object::iterator i;
    std::string           afiles = env.get<std::string>("FILES");
    std::vector<std::string> vfiles;
    ::split_str(afiles,' ', vfiles);

    std::cout << "PWD: " << getcwd(TempBuff,sizeof(TempBuff)) << "\n";

    for(size_t i=0; i < vfiles.size(); i++)
    {
        std::string f = vfiles[i];
        if(f.empty())   continue;
        std::string ofile = f;
        size_t dotpos = f.find_last_of(".");
        if(dotpos != std::string::npos)
        {
            ofile = f.substr(0,dotpos) + ".o";
        }
        std::string exec = prefix_cmd;
        exec += f +" ";
        std::cout << "COMPILE: " << exec << "\n";
        int ec;
        ::proc(exec.c_str(),ec);
        if(ec!=0)
        {
            break;
        }
        a.push(ofile);
        ::chdir(cd.c_str());
    }
    return a;
}

std::string linker(const ssq::Table& env,
                    const ssq::Array& objs,
                    const std::string& compiler)
{
    ssq::Array a(PVM->getHandle());
    std::string wd = env.get<std::string>((const char*)"SRC_DIR");
    std::string cc = env.get<std::string>(compiler.c_str());
    std::string sflags = compiler + "_LINKER_FLAGS";
    std::string ccflags = env.get<std::string>(sflags.c_str());
    std::string cd = current_path();



    std::string prefix_cmd = "cd "; prefix_cmd += wd + "; ";
    prefix_cmd += cc + " ";

    ssq::Object::iterator i;
    for(size_t i=0; i< objs.size(); i++)
    {
        std::string f = objs.get<std::string>(i);
        prefix_cmd += f;
        prefix_cmd += " ";
    }

    std::cout << "PWD: " << getcwd(TempBuff,sizeof(TempBuff)) << "\n";
    std::vector<std::string> aflags;
    split_str(ccflags,' ',aflags);
    prefix_cmd += " ";
    for(const auto& a: aflags)
    {
        prefix_cmd += a;
        if(a=="-o"){
            prefix_cmd += " ";
            std::string odir = current_path() + "/" + env.get<std::string>("OUT_DIR");
            std::filesystem::create_directories(odir);
            prefix_cmd += odir;
            prefix_cmd += "/";
            prefix_cmd += env.get<std::string>("ELF");
        }
        prefix_cmd += " ";
    }
    prefix_cmd += env.get<std::string>((const char*)"LIBS");
    prefix_cmd += " ";


    int err;
    std::cout << "LINK: " << "\n" << prefix_cmd << "\n";
    std::string ret =  ::proc(prefix_cmd.c_str(), err);
    ::chdir(cd.c_str());
    return ret;
}


bool _init_apis(ssq::VM* pvm, sq_api* ptrs)
{
    PVM = pvm;
    SQ_PTRS = ptrs;
    pvm->addFunc("directory_iterator",directory_iterator);
    pvm->addFunc("ls",ls);
    pvm->addFunc("pwd",current_path);
    pvm->addFunc("file_size",file_size);
    pvm->addFunc("compile",compile);
    pvm->addFunc("link",linker);
    pvm->addFunc("aprint",aprint);
    pvm->addFunc("tprint",tprint);
    pvm->addFunc("pushd",[](const std::string& dir){
        PrevDir = ::getcwd(TempBuff,sizeof(TempBuff));
        ::chdir(dir.c_str()); WorkDir=dir;
    });
    pvm->addFunc("popd",[](){::chdir(PrevDir.c_str()); WorkDir=PrevDir; });
    pvm->addFunc("chdir",[](const std::string& dir){::chdir(dir.c_str()); WorkDir=dir; });

    pvm->addFunc("rm",[](const std::string& fls){
        std::string cmd ;
        if(!WorkDir.empty()){
            ::getcwd(TempBuff,4096);
            cmd += "cd "; cmd += TempBuff; cmd += "; ";
        }
        cmd += "rm "; cmd += fls;
        int ec;
        ::proc(cmd.c_str(), ec);
    });

    //auto cls = pvm->addClass("Env",ssq::Class::Ctor<Env()>());
    auto cls = pvm->addClass("ENV", [](std::string appname) -> Env* {
            std::cout << "Creating ENV \n";
            return new Env(appname);
        });

    cls.addFunc("at", [](Env* self, const std::string& k) -> std::string {
        return self->get(k);
    });

    cls.addFunc("get", [](Env* self) -> ssq::Table {
        return self->get();
    });

    //
    ssq::Table t(pvm->getHandle());
    t.set("SOME",std::string("sdfasdf"));
    pvm->addTable(t, "CUSTOM");


    return false;
}

void _deinit_apis(ssq::VM* pvm, sq_api* ptrs)
{
}


