#ifndef SQ_SYSTEM_GLOBAL_H
#define SQ_SYSTEM_GLOBAL_H

#include "args.cpp"
#include "array.cpp"
#include "class.cpp"
#include "enum.cpp"
#include "function.cpp"
#include "instance.cpp"
#include "object.cpp"
#include "script.cpp"
#include "table.cpp"
#include "vm.cpp"

extern "C"{
    bool _init_apis(ssq::VM* vm, sq_api* ptrs);
    void _deinit_apis(ssq::VM* vm, sq_api* ptrs);
};

#endif // SQ_SYSTEM_GLOBAL_H
