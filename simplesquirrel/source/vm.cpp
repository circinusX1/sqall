#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/enum.hpp"
#include "../include/simplesquirrel/vm.hpp"
#include <squirrel.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>
#include <sqstdmath.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <forward_list>
#include <cstdarg>
#include <cstring>
#include <iostream>

namespace ssq {
    VM::VM(size_t stackSize, Libs::Flag flags):Table() {
        vm = SQ_PTRS->sq_open(stackSize);
        SQ_PTRS->sq_resetobject(&obj);
        SQ_PTRS->sq_setforeignptr(vm, this);

        registerStdlib(flags);

        setPrintFunc(&VM::defaultPrintFunc, &VM::defaultErrorFunc);
        setRuntimeErrorFunc(&VM::defaultRuntimeErrorFunc);
        setCompileErrorFunc(&VM::defaultCompilerErrorFunc);

        SQ_PTRS->sq_pushroottable(vm);
        SQ_PTRS->sq_getstackobj(vm,-1,&obj);
        SQ_PTRS->sq_addref(vm, &obj);
        SQ_PTRS->sq_pop(vm, 1);
    }

    void VM::destroy() {
		classMap.clear();
        if (vm != nullptr) {
            SQ_PTRS->sq_resetobject(&obj);
            SQ_PTRS->sq_close(vm);
        }
        vm = nullptr;
    }

    VM::~VM() {
        destroy();
    }

    void VM::swap(VM& other) NOEXCEPT {
        using std::swap;
        Object::swap(other);
        swap(runtimeException, other.runtimeException);
        swap(compileException, other.compileException);
		swap(classMap, other.classMap);

        if(vm != nullptr) {
            SQ_PTRS->sq_setforeignptr(vm, this);
        }
        if(other.vm != nullptr) {
            SQ_PTRS->sq_setforeignptr(other.vm, this);
        }
    }

    VM::VM(VM&& other) NOEXCEPT :Table() {
        swap(other);
    }

    void VM::registerStdlib(Libs::Flag flags) {
        if (flags == 0)return;
        SQ_PTRS->sq_pushroottable(vm);
        if(flags & ssq::Libs::IO)
            SQ_PTRS->sqstd_register_iolib(vm);
        if(flags & ssq::Libs::BLOB)
            SQ_PTRS->sqstd_register_bloblib(vm);
        if(flags & ssq::Libs::MATH)
            SQ_PTRS->sqstd_register_mathlib(vm);
        if(flags & ssq::Libs::SYSTEM)
            SQ_PTRS->sqstd_register_systemlib(vm);
        if(flags & ssq::Libs::STRING)
            SQ_PTRS->sqstd_register_stringlib(vm);
        SQ_PTRS->sq_pop(vm, 1);
    }

    void VM::setPrintFunc(SqPrintFunc printFunc, SqErrorFunc errorFunc) {
        SQ_PTRS->sq_setprintfunc(vm, printFunc, errorFunc);
    }

    void VM::setRuntimeErrorFunc(SqRuntimeErrorFunc runtimeErrorFunc) {
        SQ_PTRS->sq_newclosure(vm, runtimeErrorFunc, 0);
        SQ_PTRS->sq_seterrorhandler(vm);
    }

    void VM::setCompileErrorFunc(SqCompileErrorFunc compileErrorFunc) {
        SQ_PTRS->sq_setcompilererrorhandler(vm, compileErrorFunc);
    }

    SQInteger VM::getTop() const {
        return SQ_PTRS->sq_gettop(vm);
    }

    Script VM::compileSource(const char* source, const char* name) {
        Script script(vm);
        if(SQ_FAILED(SQ_PTRS->sq_compilebuffer(vm, source, strlen(source), name, true))){
            if (!compileException)throw CompileException("Source cannot be compiled!");
            throw *compileException;
        }

        SQ_PTRS->sq_getstackobj(vm,-1,&script.getRaw());
        SQ_PTRS->sq_addref(vm, &script.getRaw());
        SQ_PTRS->sq_pop(vm, 1);
        return script;
    }

    Script VM::compileFile(const char* path) {
        Script script(vm);
        if (SQ_FAILED(SQ_PTRS->sqstd_loadfile(vm, path, true))) {
            if (!compileException)throw CompileException("File not found or cannot be read!");
            throw *compileException;
        }

        SQ_PTRS->sq_getstackobj(vm, -1, &script.getRaw());
        SQ_PTRS->sq_addref(vm, &script.getRaw());
        SQ_PTRS->sq_pop(vm, 1);
        return script;
    }

    void VM::run(const Script& script) const {
        if(!script.isEmpty()) {
            SQInteger top = SQ_PTRS->sq_gettop(vm);
            SQ_PTRS->sq_pushobject(vm, script.getRaw());
            SQ_PTRS->sq_pushroottable(vm);
            SQRESULT result = SQ_PTRS->sq_call(vm, 1, false, true);
            SQ_PTRS->sq_settop(vm, top);
            if(SQ_FAILED(result)){
                throw *runtimeException;
            }
        }
        else {
            throw RuntimeException("Empty script object");
        }
    }

    Enum VM::addEnum(const char* name) {
        Enum enm(vm);
        SQ_PTRS->sq_pushconsttable(vm);
        SQ_PTRS->sq_pushstring(vm, name, strlen(name));
        detail::push<Object>(vm, enm);
        SQ_PTRS->sq_newslot(vm, -3, false);
        SQ_PTRS->sq_pop(vm,1); // pop table
        return std::move(enm);
    }

    VM& VM::operator = (VM&& other) NOEXCEPT {
        if(this != &other) {
            swap(other);
        }
        return *this;
    }

    Object VM::callAndReturn(SQUnsignedInteger nparams, SQInteger top) const {
        if(SQ_FAILED(SQ_PTRS->sq_call(vm, 1 + nparams, true, true))){
            SQ_PTRS->sq_settop(vm, top);
            if (runtimeException == nullptr)
                throw RuntimeException("Unknown squirrel runtime error");
            throw *runtimeException;
        }

        Object ret(vm);
        SQ_PTRS->sq_getstackobj(vm, -1, &ret.getRaw());
        SQ_PTRS->sq_addref(vm, &ret.getRaw());
        SQ_PTRS->sq_settop(vm, top);
        return ret;
    }

    void VM::debugStack() const {
        auto top = getTop();
        while(top >= 0) {
            SQObjectType objectType = SQ_PTRS->sq_gettype(vm, top);
            Type type = Type(objectType);
            std::cout << "stack index: " << top << " type: " << typeToStr(type) << std::endl;
            top--;
        }
    }

    void VM::defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        va_list vl;
        va_start(vl, s);
        vprintf(s, vl);
        printf("\n");
        va_end(vl);
    }

    void VM::defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        va_list vl;
        va_start(vl, s);
        fprintf(stderr, s, vl);
        fprintf(stderr, "\n");
        va_end(vl);
    }

    SQInteger VM::defaultRuntimeErrorFunc(HSQUIRRELVM vm) {
        SQStackInfos si;
        SQ_PTRS->sq_stackinfos(vm, 1, &si);

        auto source = (si.source != nullptr ? si.source : "null");
        auto funcname = (si.funcname != nullptr ? si.funcname : "unknown");

        const SQChar *sErr = 0;
        if(SQ_PTRS->sq_gettop(vm) >= 1){
            if(SQ_FAILED(SQ_PTRS->sq_getstring(vm, 2, &sErr))){
                sErr = "unknown error";
            }
        }

        auto ptr = reinterpret_cast<VM*>(SQ_PTRS->sq_getforeignptr(vm));
        ptr->runtimeException.reset(new RuntimeException(
            sErr,
            source,
            funcname,
            si.line
        ));
        return 0;
    }

    void VM::defaultCompilerErrorFunc(
        HSQUIRRELVM vm,
        const SQChar* desc,
        const SQChar* source,
        SQInteger line,
        SQInteger column) {
        auto ptr = reinterpret_cast<VM*>(SQ_PTRS->sq_getforeignptr(vm));
        ptr->compileException.reset(new CompileException(
            desc,
            source,
            line,
            column
        ));
    }

    void VM::pushArgs() {

    }

	void VM::addClassObj(size_t hashCode, const HSQOBJECT& obj) {
		classMap[hashCode] = obj;
	}

	const HSQOBJECT& VM::getClassObj(size_t hashCode) {
		return classMap.at(hashCode);
	}

	namespace detail {
	    void addClassObj(HSQUIRRELVM vm, size_t hashCode, const HSQOBJECT& obj) {
		    VM* machine = reinterpret_cast<VM*>(SQ_PTRS->sq_getforeignptr(vm));
			machine->addClassObj(hashCode, obj);
	    }

		const HSQOBJECT& getClassObj(HSQUIRRELVM vm, size_t hashCode) {
		    VM* machine = reinterpret_cast<VM*>(SQ_PTRS->sq_getforeignptr(vm));
			return machine->getClassObj(hashCode);
	    }
    }
}
