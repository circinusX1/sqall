

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
#include "sq_module.hpp"


sq_api*       SQ_PTRS;

// SQUIRREL_API SQRESULT sqstd_register_bloblib(HSQUIRRELVM v);
// SQUIRREL_API SQRESULT sqstd_register_iolib(HSQUIRRELVM v);
// SQUIRREL_API SQRESULT sqstd_register_mathlib(HSQUIRRELVM v);
// SQUIRREL_API SQRESULT sqstd_register_systemlib(HSQUIRRELVM v);
// SQUIRREL_API SQRESULT sqstd_register_stringlib(HSQUIRRELVM v);



// Create and populate the HSQAPI structure with function pointers
// If new functions are added to the Squirrel API, they should be added here too
sq_api* sqrat_newapi()
{
    static sq_api _sqapi;
    SQ_PTRS = &_sqapi;

    _sqapi.sqstd_loadfile = sqstd_loadfile;
    _sqapi.sqstd_register_bloblib = sqstd_register_bloblib;
    _sqapi.sqstd_register_iolib = sqstd_register_iolib;
    _sqapi.sqstd_register_mathlib = sqstd_register_mathlib;
    _sqapi.sqstd_register_systemlib = sqstd_register_systemlib;
    _sqapi.sqstd_register_stringlib = sqstd_register_stringlib;


    _sqapi.sq_newthread = sq_newthread;
    _sqapi.sq_seterrorhandler = sq_seterrorhandler;
    _sqapi.sq_open = sq_open;
    _sqapi.sq_close = sq_close;
    _sqapi.sq_setforeignptr = sq_setforeignptr;
    _sqapi.sq_getforeignptr = sq_getforeignptr;
    _sqapi.sq_setsharedforeignptr = sq_setsharedforeignptr;
    _sqapi.sq_getsharedforeignptr = sq_getsharedforeignptr;
    _sqapi.sq_setvmreleasehook = sq_setvmreleasehook;
    _sqapi.sq_getvmreleasehook = sq_getvmreleasehook;
    _sqapi.sq_setsharedreleasehook = sq_setsharedreleasehook;
    _sqapi.sq_getsharedreleasehook = sq_getsharedreleasehook;
    _sqapi.sq_setprintfunc = sq_setprintfunc;
    _sqapi.sq_getprintfunc = sq_getprintfunc;
    _sqapi.sq_geterrorfunc = sq_geterrorfunc;
    _sqapi.sq_suspendvm = sq_suspendvm;
    _sqapi.sq_wakeupvm = sq_wakeupvm;
    _sqapi.sq_getvmstate = sq_getvmstate;
    _sqapi.sq_getversion = sq_getversion;
    _sqapi.sq_compile = sq_compile;
    _sqapi.sq_compilebuffer = sq_compilebuffer;
    _sqapi.sq_enabledebuginfo = sq_enabledebuginfo;
    _sqapi.sq_notifyallexceptions = sq_notifyallexceptions;
    _sqapi.sq_setcompilererrorhandler = sq_setcompilererrorhandler;
    _sqapi.sq_push = sq_push;
    _sqapi.sq_pop = sq_pop;
    _sqapi.sq_poptop = sq_poptop;
    _sqapi.sq_remove = sq_remove;
    _sqapi.sq_gettop = sq_gettop;
    _sqapi.sq_settop = sq_settop;
    _sqapi.sq_reservestack = sq_reservestack;
    _sqapi.sq_cmp = sq_cmp;
    _sqapi.sq_move = sq_move;
    _sqapi.sq_newuserdata = sq_newuserdata;
    _sqapi.sq_newtable = sq_newtable;
    _sqapi.sq_newtableex = sq_newtableex;
    _sqapi.sq_newarray = sq_newarray;
    _sqapi.sq_newclosure = sq_newclosure;
    _sqapi.sq_setparamscheck = sq_setparamscheck;
    _sqapi.sq_bindenv = sq_bindenv;
    _sqapi.sq_setclosureroot = sq_setclosureroot;
    _sqapi.sq_getclosureroot = sq_getclosureroot;
    _sqapi.sq_pushstring = sq_pushstring;
    _sqapi.sq_pushfloat = sq_pushfloat;
    _sqapi.sq_pushinteger = sq_pushinteger;
    _sqapi.sq_pushbool = sq_pushbool;
    _sqapi.sq_pushuserpointer = sq_pushuserpointer;
    _sqapi.sq_pushnull = sq_pushnull;
    _sqapi.sq_pushthread = sq_pushthread;
    _sqapi.sq_gettype = sq_gettype;
    _sqapi.sq_typeof = sq_typeof;
    _sqapi.sq_getsize = sq_getsize;
    _sqapi.sq_gethash = sq_gethash;
    _sqapi.sq_getbase = sq_getbase;
    _sqapi.sq_instanceof = sq_instanceof;
    _sqapi.sq_tostring = sq_tostring;
    _sqapi.sq_tobool = sq_tobool;
    _sqapi.sq_getstringandsize = sq_getstringandsize;
    _sqapi.sq_getstring = sq_getstring;
    _sqapi.sq_getinteger = sq_getinteger;
    _sqapi.sq_getfloat = sq_getfloat;
    _sqapi.sq_getbool = sq_getbool;
    _sqapi.sq_getthread = sq_getthread;
    _sqapi.sq_getuserpointer = sq_getuserpointer;
    _sqapi.sq_getuserdata = sq_getuserdata;
    _sqapi.sq_settypetag = sq_settypetag;
    _sqapi.sq_gettypetag = sq_gettypetag;
    _sqapi.sq_setreleasehook = sq_setreleasehook;
    _sqapi.sq_getreleasehook = sq_getreleasehook;
    _sqapi.sq_getscratchpad = sq_getscratchpad;
    _sqapi.sq_getfunctioninfo = sq_getfunctioninfo;
    _sqapi.sq_getclosureinfo = sq_getclosureinfo;
    _sqapi.sq_getclosurename = sq_getclosurename;
    _sqapi.sq_setnativeclosurename = sq_setnativeclosurename;
    _sqapi.sq_setinstanceup = sq_setinstanceup;
    _sqapi.sq_getinstanceup = sq_getinstanceup;
    _sqapi.sq_setclassudsize = sq_setclassudsize;
    _sqapi.sq_newclass = sq_newclass;
    _sqapi.sq_createinstance = sq_createinstance;
    _sqapi.sq_setattributes = sq_setattributes;
    _sqapi.sq_getattributes = sq_getattributes;
    _sqapi.sq_getclass = sq_getclass;
    _sqapi.sq_weakref = sq_weakref;
    _sqapi.sq_getdefaultdelegate = sq_getdefaultdelegate;
    _sqapi.sq_getmemberhandle = sq_getmemberhandle;
    _sqapi.sq_getbyhandle = sq_getbyhandle;
    _sqapi.sq_setbyhandle = sq_setbyhandle;
    _sqapi.sq_pushroottable = sq_pushroottable;
    _sqapi.sq_pushregistrytable = sq_pushregistrytable;
    _sqapi.sq_pushconsttable = sq_pushconsttable;
    _sqapi.sq_setroottable = sq_setroottable;
    _sqapi.sq_setconsttable = sq_setconsttable;
    _sqapi.sq_newslot = sq_newslot;
    _sqapi.sq_deleteslot = sq_deleteslot;
    _sqapi.sq_set = sq_set;
    _sqapi.sq_get = sq_get;
    _sqapi.sq_rawget = sq_rawget;
    _sqapi.sq_rawset = sq_rawset;
    _sqapi.sq_rawdeleteslot = sq_rawdeleteslot;
    _sqapi.sq_newmember = sq_newmember;
    _sqapi.sq_rawnewmember = sq_rawnewmember;
    _sqapi.sq_arrayappend = sq_arrayappend;
    _sqapi.sq_arraypop = sq_arraypop;
    _sqapi.sq_arrayresize = sq_arrayresize;
    _sqapi.sq_arrayreverse = sq_arrayreverse;
    _sqapi.sq_arrayremove = sq_arrayremove;
    _sqapi.sq_arrayinsert = sq_arrayinsert;
    _sqapi.sq_setdelegate = sq_setdelegate;
    _sqapi.sq_getdelegate = sq_getdelegate;
    _sqapi.sq_clone = sq_clone;
    _sqapi.sq_setfreevariable = sq_setfreevariable;
    _sqapi.sq_next = sq_next;
    _sqapi.sq_getweakrefval = sq_getweakrefval;
    _sqapi.sq_clear = sq_clear;
    _sqapi.sq_call = sq_call;
    _sqapi.sq_resume = sq_resume;
    _sqapi.sq_getlocal = sq_getlocal;
    _sqapi.sq_getcallee = sq_getcallee;
    _sqapi.sq_getfreevariable = sq_getfreevariable;
    _sqapi.sq_throwerror = sq_throwerror;
    _sqapi.sq_throwobject = sq_throwobject;
    _sqapi.sq_reseterror = sq_reseterror;
    _sqapi.sq_getlasterror = sq_getlasterror;
    _sqapi.sq_tailcall = sq_tailcall;
    _sqapi.sq_getstackobj = sq_getstackobj;
    _sqapi.sq_pushobject = sq_pushobject;
    _sqapi.sq_addref = sq_addref;
    _sqapi.sq_release = sq_release;
    _sqapi.sq_getrefcount = sq_getrefcount;
    _sqapi.sq_resetobject = sq_resetobject;
    _sqapi.sq_objtostring = sq_objtostring;
    _sqapi.sq_objtobool = sq_objtobool;
    _sqapi.sq_objtointeger = sq_objtointeger;
    _sqapi.sq_objtofloat = sq_objtofloat;
    _sqapi.sq_objtouserpointer = sq_objtouserpointer;
    _sqapi.sq_getobjtypetag = sq_getobjtypetag;
    _sqapi.sq_getvmrefcount = sq_getvmrefcount;
    _sqapi.sq_collectgarbage = sq_collectgarbage;
    _sqapi.sq_resurrectunreachable = sq_resurrectunreachable;
    _sqapi.sq_writeclosure = sq_writeclosure;
    _sqapi.sq_readclosure = sq_readclosure;
    _sqapi.sq_malloc = sq_malloc;
    _sqapi.sq_realloc = sq_realloc;
    _sqapi.sq_free = sq_free;
    _sqapi.sq_stackinfos = sq_stackinfos;
    _sqapi.sq_setdebughook = sq_setdebughook;
    _sqapi.sq_setnativedebughook = sq_setnativedebughook;


    return SQ_PTRS;
}


