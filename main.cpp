
#include <iostream>
#include <string>
#include <simplesquirrel/simplesquirrel.hpp>
#include <iomanip>
#include <dlfcn.h>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sq_module.hpp"



typedef bool (*api_pfn_t)(ssq::VM* vm, sq_api* ptrs);
static bool loadapi(const std::string&  lib);
thread_local ssq::VM* PVM;
static std::vector<void*>  _dlls;

int main(int argc, char* argv[]) {
    std::string   scrfile;

    if(argc < 2)
    {
        std::cout << "pass a script name \n";
        return 1;
    }
    else
    {
        scrfile = argv[1];
    }

    ::sqrat_newapi();
    PVM = new ssq::VM(4096,ssq::Libs::ALL);
    SQ_PTRS->sq_setconsttable(PVM->getHandle());

    try {
        // Compile script and run it
        ssq::Script script = PVM->compileFile(scrfile.c_str());

        PVM->addFunc("load", loadapi);

        PVM->run(script);

        ssq::Array a(PVM->getHandle());
        for(int i=1;i<argc;i++)
            a.push(argv[i]);

        ssq::Function main = PVM->findFunc("main");

        if(!main.isNull())
        {
            PVM->callFunc(main, PVM->getHandle(), a);
        }

    } catch (ssq::CompileException& e) {
        std::cerr << "Failed to run file: " << e.what() << std::endl;
        return -1;
    } catch (ssq::TypeException& e) {
        std::cerr << "Something went wrong passing objects: " << e.what() << std::endl;
        return -1;
    } catch (ssq::RuntimeException& e) {
        std::cerr << "Something went wrong during execution: " << e.what() << std::endl;
        return -1;
    } catch (ssq::NotFoundException& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    delete PVM;
}

static void  add_dll(void* p){_dlls.push_back(p);}

bool loadapi(const std::string&  lib)
{
    void* module  = ::dlopen(lib.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
    if (module == nullptr)
    {
        char full[1000];
        char dir[500];

        ::getcwd(dir,499);
        ::sprintf(full,"%s/%s",dir,lib.c_str());
        if(::access(full,0)==0)
        {
            module = ::dlopen(full, RTLD_NOW | RTLD_LOCAL);
            if (module == nullptr)
            {
                std::cerr << "cannot load library: " << lib << " " << dlerror() << "\n";
                return false;
            }
        }
        else
        {
            std::cerr << "No such file: " << lib << "\n";
        }
    }

    api_pfn_t startmod = (api_pfn_t)::dlsym(module, "_init_apis");
    if (startmod == nullptr)
    {
        std::cerr << "cannot load _init_apis" << __FUNCTION__ << dlerror() << "\n";
        ::dlclose(module);
        module = nullptr;
        return false;
    }
    ::add_dll(module);
#ifdef TRACE
    printf("vm=%p\n",_psq->theVM());
#endif
    return startmod(PVM, SQ_PTRS);
}

