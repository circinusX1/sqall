#ifndef SWMOD

#include "squirrel.h"
#define SWMOD

#ifdef __cplusplus
extern "C" {
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @cond DEV
/// Allows modules to interface with Squirrel's C api without linking to the squirrel library
/// If new functions are added to the Squirrel API, they should be added here too
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct sq_api{
    HSQUIRRELVM (*sq_newthread)(HSQUIRRELVM friendvm, SQInteger initialstacksize);
    void (*sq_seterrorhandler)(HSQUIRRELVM v);
    HSQUIRRELVM (*sq_open)(SQInteger initialstacksize);
    void (*sq_close)(HSQUIRRELVM v);
    void (*sq_setforeignptr)(HSQUIRRELVM v,SQUserPointer p);
    SQUserPointer (*sq_getforeignptr)(HSQUIRRELVM v);
    void (*sq_setsharedforeignptr)(HSQUIRRELVM v,SQUserPointer p);
    SQUserPointer (*sq_getsharedforeignptr)(HSQUIRRELVM v);
    void (*sq_setvmreleasehook)(HSQUIRRELVM v,SQRELEASEHOOK hook);
    SQRELEASEHOOK (*sq_getvmreleasehook)(HSQUIRRELVM v);
    void (*sq_setsharedreleasehook)(HSQUIRRELVM v,SQRELEASEHOOK hook);
    SQRELEASEHOOK (*sq_getsharedreleasehook)(HSQUIRRELVM v);
    void (*sq_setprintfunc)(HSQUIRRELVM v, SQPRINTFUNCTION printfunc,SQPRINTFUNCTION errfunc);
    SQPRINTFUNCTION (*sq_getprintfunc)(HSQUIRRELVM v);
    SQPRINTFUNCTION (*sq_geterrorfunc)(HSQUIRRELVM v);
    SQRESULT (*sq_suspendvm)(HSQUIRRELVM v);
    SQRESULT (*sq_wakeupvm)(HSQUIRRELVM v,SQBool resumedret,SQBool retval,SQBool raiseerror,SQBool throwerror);
    SQInteger (*sq_getvmstate)(HSQUIRRELVM v);
    SQInteger (*sq_getversion)();
            SQRESULT (*sq_compile)(HSQUIRRELVM v,SQLEXREADFUNC read,SQUserPointer p,const SQChar *sourcename,SQBool raiseerror);
    SQRESULT (*sq_compilebuffer)(HSQUIRRELVM v,const SQChar *s,SQInteger size,const SQChar *sourcename,SQBool raiseerror);
    void (*sq_enabledebuginfo)(HSQUIRRELVM v, SQBool enable);
    void (*sq_notifyallexceptions)(HSQUIRRELVM v, SQBool enable);
    void (*sq_setcompilererrorhandler)(HSQUIRRELVM v,SQCOMPILERERROR f);
    void (*sq_push)(HSQUIRRELVM v,SQInteger idx);
    void (*sq_pop)(HSQUIRRELVM v,SQInteger nelemstopop);
    void (*sq_poptop)(HSQUIRRELVM v);
    void (*sq_remove)(HSQUIRRELVM v,SQInteger idx);
    SQInteger (*sq_gettop)(HSQUIRRELVM v);
    void (*sq_settop)(HSQUIRRELVM v,SQInteger newtop);
    SQRESULT (*sq_reservestack)(HSQUIRRELVM v,SQInteger nsize);
    SQInteger (*sq_cmp)(HSQUIRRELVM v);
    void (*sq_move)(HSQUIRRELVM dest,HSQUIRRELVM src,SQInteger idx);
    SQUserPointer (*sq_newuserdata)(HSQUIRRELVM v,SQUnsignedInteger size);
    void (*sq_newtable)(HSQUIRRELVM v);
    void (*sq_newtableex)(HSQUIRRELVM v,SQInteger initialcapacity);
    void (*sq_newarray)(HSQUIRRELVM v,SQInteger size);
    void (*sq_newclosure)(HSQUIRRELVM v,SQFUNCTION func,SQUnsignedInteger nfreevars);
    SQRESULT (*sq_setparamscheck)(HSQUIRRELVM v,SQInteger nparamscheck,const SQChar *typemask);
    SQRESULT (*sq_bindenv)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_setclosureroot)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_getclosureroot)(HSQUIRRELVM v,SQInteger idx);
    void (*sq_pushstring)(HSQUIRRELVM v,const SQChar *s,SQInteger len);
    void (*sq_pushfloat)(HSQUIRRELVM v,SQFloat f);
    void (*sq_pushinteger)(HSQUIRRELVM v,SQInteger n);
    void (*sq_pushbool)(HSQUIRRELVM v,SQBool b);
    void (*sq_pushuserpointer)(HSQUIRRELVM v,SQUserPointer p);
    void (*sq_pushnull)(HSQUIRRELVM v);
    void (*sq_pushthread)(HSQUIRRELVM v, HSQUIRRELVM thread);
    SQObjectType (*sq_gettype)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_typeof)(HSQUIRRELVM v,SQInteger idx);
    SQInteger (*sq_getsize)(HSQUIRRELVM v,SQInteger idx);
    SQHash (*sq_gethash)(HSQUIRRELVM v, SQInteger idx);
    SQRESULT (*sq_getbase)(HSQUIRRELVM v,SQInteger idx);
    SQBool (*sq_instanceof)(HSQUIRRELVM v);
    SQRESULT (*sq_tostring)(HSQUIRRELVM v,SQInteger idx);
    void (*sq_tobool)(HSQUIRRELVM v, SQInteger idx, SQBool *b);
    SQRESULT (*sq_getstringandsize)(HSQUIRRELVM v,SQInteger idx,const SQChar **c,SQInteger *size);
    SQRESULT (*sq_getstring)(HSQUIRRELVM v,SQInteger idx,const SQChar **c);
    SQRESULT (*sq_getinteger)(HSQUIRRELVM v,SQInteger idx,SQInteger *i);
    SQRESULT (*sq_getfloat)(HSQUIRRELVM v,SQInteger idx,SQFloat *f);
    SQRESULT (*sq_getbool)(HSQUIRRELVM v,SQInteger idx,SQBool *b);
    SQRESULT (*sq_getthread)(HSQUIRRELVM v,SQInteger idx,HSQUIRRELVM *thread);
    SQRESULT (*sq_getuserpointer)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *p);
    SQRESULT (*sq_getuserdata)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *p,SQUserPointer *typetag);
    SQRESULT (*sq_settypetag)(HSQUIRRELVM v,SQInteger idx,SQUserPointer typetag);
    SQRESULT (*sq_gettypetag)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *typetag);
    void (*sq_setreleasehook)(HSQUIRRELVM v,SQInteger idx,SQRELEASEHOOK hook);
    SQRELEASEHOOK (*sq_getreleasehook)(HSQUIRRELVM v,SQInteger idx);
    SQChar *(*sq_getscratchpad)(HSQUIRRELVM v,SQInteger minsize);
    SQRESULT (*sq_getfunctioninfo)(HSQUIRRELVM v,SQInteger level,SQFunctionInfo *fi);
    SQRESULT (*sq_getclosureinfo)(HSQUIRRELVM v,SQInteger idx,SQInteger *nparams,SQInteger *nfreevars);
    SQRESULT (*sq_getclosurename)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_setnativeclosurename)(HSQUIRRELVM v,SQInteger idx,const SQChar *name);
    SQRESULT (*sq_setinstanceup)(HSQUIRRELVM v, SQInteger idx, SQUserPointer p);
    SQRESULT (*sq_getinstanceup)(HSQUIRRELVM v, SQInteger idx, SQUserPointer *p,SQUserPointer typetag);
    SQRESULT (*sq_setclassudsize)(HSQUIRRELVM v, SQInteger idx, SQInteger udsize);
    SQRESULT (*sq_newclass)(HSQUIRRELVM v,SQBool hasbase);
    SQRESULT (*sq_createinstance)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_setattributes)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_getattributes)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_getclass)(HSQUIRRELVM v,SQInteger idx);
    void (*sq_weakref)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_getdefaultdelegate)(HSQUIRRELVM v,SQObjectType t);
    SQRESULT (*sq_getmemberhandle)(HSQUIRRELVM v,SQInteger idx,HSQMEMBERHANDLE *handle);
    SQRESULT (*sq_getbyhandle)(HSQUIRRELVM v,SQInteger idx,const HSQMEMBERHANDLE *handle);
    SQRESULT (*sq_setbyhandle)(HSQUIRRELVM v,SQInteger idx,const HSQMEMBERHANDLE *handle);
    void (*sq_pushroottable)(HSQUIRRELVM v);
    void (*sq_pushregistrytable)(HSQUIRRELVM v);
    void (*sq_pushconsttable)(HSQUIRRELVM v);
    SQRESULT (*sq_setroottable)(HSQUIRRELVM v);
    SQRESULT (*sq_setconsttable)(HSQUIRRELVM v);
    SQRESULT (*sq_newslot)(HSQUIRRELVM v, SQInteger idx, SQBool bstatic);
    SQRESULT (*sq_deleteslot)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    SQRESULT (*sq_set)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_get)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_rawget)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_rawset)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_rawdeleteslot)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    SQRESULT (*sq_newmember)(HSQUIRRELVM v,SQInteger idx,SQBool bstatic);
    SQRESULT (*sq_rawnewmember)(HSQUIRRELVM v,SQInteger idx,SQBool bstatic);
    SQRESULT (*sq_arrayappend)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_arraypop)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    SQRESULT (*sq_arrayresize)(HSQUIRRELVM v,SQInteger idx,SQInteger newsize);
    SQRESULT (*sq_arrayreverse)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_arrayremove)(HSQUIRRELVM v,SQInteger idx,SQInteger itemidx);
    SQRESULT (*sq_arrayinsert)(HSQUIRRELVM v,SQInteger idx,SQInteger destpos);
    SQRESULT (*sq_setdelegate)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_getdelegate)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_clone)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_setfreevariable)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger nval);
    SQRESULT (*sq_next)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_getweakrefval)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_clear)(HSQUIRRELVM v,SQInteger idx);
    SQRESULT (*sq_call)(HSQUIRRELVM v,SQInteger params,SQBool retval,SQBool raiseerror);
    SQRESULT (*sq_resume)(HSQUIRRELVM v,SQBool retval,SQBool raiseerror);
    const SQChar *(*sq_getlocal)(HSQUIRRELVM v,SQUnsignedInteger level,SQUnsignedInteger idx);
    SQRESULT (*sq_getcallee)(HSQUIRRELVM v);
    const SQChar *(*sq_getfreevariable)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger nval);
    SQRESULT (*sq_throwerror)(HSQUIRRELVM v,const SQChar *err);
    SQRESULT (*sq_throwobject)(HSQUIRRELVM v);
    void (*sq_reseterror)(HSQUIRRELVM v);
    void (*sq_getlasterror)(HSQUIRRELVM v);
    SQRESULT (*sq_tailcall)(HSQUIRRELVM v, SQInteger nparams);
    SQRESULT (*sq_getstackobj)(HSQUIRRELVM v,SQInteger idx,HSQOBJECT *po);
    void (*sq_pushobject)(HSQUIRRELVM v,HSQOBJECT obj);
    void (*sq_addref)(HSQUIRRELVM v,HSQOBJECT *po);
    SQBool (*sq_release)(HSQUIRRELVM v,HSQOBJECT *po);
    SQUnsignedInteger (*sq_getrefcount)(HSQUIRRELVM v,HSQOBJECT *po);
    void (*sq_resetobject)(HSQOBJECT *po);
    const SQChar *(*sq_objtostring)(const HSQOBJECT *o);
    SQBool (*sq_objtobool)(const HSQOBJECT *o);
    SQInteger (*sq_objtointeger)(const HSQOBJECT *o);
    SQFloat (*sq_objtofloat)(const HSQOBJECT *o);
    SQUserPointer (*sq_objtouserpointer)(const HSQOBJECT *o);
    SQRESULT (*sq_getobjtypetag)(const HSQOBJECT *o,SQUserPointer * typetag);
    SQUnsignedInteger (*sq_getvmrefcount)(HSQUIRRELVM v, const HSQOBJECT *po);
    SQInteger (*sq_collectgarbage)(HSQUIRRELVM v);
    SQRESULT (*sq_resurrectunreachable)(HSQUIRRELVM v);
    SQRESULT (*sq_writeclosure)(HSQUIRRELVM vm,SQWRITEFUNC writef,SQUserPointer up);
    SQRESULT (*sq_readclosure)(HSQUIRRELVM vm,SQREADFUNC readf,SQUserPointer up);
    void *(*sq_malloc)(SQUnsignedInteger size);
    void *(*sq_realloc)(void* p,SQUnsignedInteger oldsize,SQUnsignedInteger newsize);
    void (*sq_free)(void *p,SQUnsignedInteger size);
    SQRESULT (*sq_stackinfos)(HSQUIRRELVM v,SQInteger level,SQStackInfos *si);
    void (*sq_setdebughook)(HSQUIRRELVM v);
    void (*sq_setnativedebughook)(HSQUIRRELVM v,SQDEBUGHOOK hook);

    SQRESULT (*sqstd_register_bloblib)(HSQUIRRELVM v);
    SQRESULT (*sqstd_register_iolib)(HSQUIRRELVM v);
    SQRESULT (*sqstd_register_mathlib)(HSQUIRRELVM v);
    SQRESULT (*sqstd_register_systemlib)(HSQUIRRELVM v);
    SQRESULT (*sqstd_register_stringlib)(HSQUIRRELVM v);
    SQRESULT (*sqstd_loadfile)(HSQUIRRELVM v,const SQChar *filename,SQBool printerror);

}sq_api;

#ifdef __cplusplus
} /*extern "C"*/
#endif

extern sq_api* SQ_PTRS;

sq_api* sqrat_newapi();


#endif // (*sq_MODULE_H
