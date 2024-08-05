#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/class.hpp"
#include "../include/simplesquirrel/exceptions.hpp"
#include "../include/simplesquirrel/function.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
	Class::Class() :Object(), tableSet(), tableGet() {

    }

    Class::Class(HSQUIRRELVM vm) :Object(vm), tableSet(vm), tableGet(vm) {

    }

    Class::Class(const Object& object) : Object(object.getHandle())
        , tableSet(object.getHandle()), tableGet(object.getHandle()) {

        if (object.getType() != Type::CLASS) throw TypeException("bad cast", "CLASS", object.getTypeStr());
        if (vm != nullptr && !object.isEmpty()) {
            obj = object.getRaw();
            SQ_PTRS->sq_addref(vm, &obj);
        }
    }

    Class::Class(const Class& other) :Object(other), tableSet(other.tableSet), tableGet(other.tableGet) {

    }

    Class::Class(Class&& other) NOEXCEPT : Object(std::forward<Class>(other)),
        tableSet(std::forward<Object>(other.tableSet)),
        tableGet(std::forward<Object>(other.tableGet)) {

    }

    void Class::swap(Class& other) NOEXCEPT {
        if (this != &other) {
            Object::swap(other);
            tableSet.swap(other.tableSet);
            tableGet.swap(other.tableGet);
        }
    }

    Function Class::findFunc(const char* name) const {
        Object object = Object::find(name);
        return Function(object);
    }

    Class& Class::operator = (const Class& other) {
        if (this != &other) {
            Class o(other);
            swap(o);
        }
        return *this;
    }

    Class& Class::operator = (Class&& other) NOEXCEPT {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    void Class::findTable(const char* name, Object& table, SQFUNCTION dlg) const {
        // Check if the table has been referenced
        if(!table.isEmpty()) {
            return;
        }
            
        // Find the table
        SQ_PTRS->sq_pushobject(vm, obj);
        SQ_PTRS->sq_pushstring(vm, name, strlen(name));

        if (SQ_FAILED(SQ_PTRS->sq_get(vm, -2))) {
            // Does not exists
            SQ_PTRS->sq_pop(vm, 1);

            // Create one
            table = Object(vm);
            SQ_PTRS->sq_newtable(vm);
            SQ_PTRS->sq_getstackobj(vm, -1, &table.getRaw());
            SQ_PTRS->sq_addref(vm, &table.getRaw());
            SQ_PTRS->sq_pop(vm, 1);

            SQ_PTRS->sq_pushobject(vm, obj); // Push class obj
            SQ_PTRS->sq_pushstring(vm, name, strlen(name));
            SQ_PTRS->sq_pushobject(vm, table.getRaw());
            SQ_PTRS->sq_newclosure(vm, dlg, 1);

            if(SQ_FAILED(SQ_PTRS->sq_newslot(vm, -3, false))) {
                throw TypeException("Failed to create table!");
            }

            SQ_PTRS->sq_pop(vm, 1); // Pop class obj

        } else {
            // Return one
            table = Object(vm);
            SQ_PTRS->sq_getstackobj(vm, -1, &table.getRaw());
            SQ_PTRS->sq_addref(vm, &table.getRaw());
            SQ_PTRS->sq_pop(vm, 2);
        }
    }

    SQInteger Class::dlgGetStub(HSQUIRRELVM vm) {
        // Find the set method in the set table
        SQ_PTRS->sq_push(vm, 2);
        if (!SQ_SUCCEEDED(SQ_PTRS->sq_get(vm, -2))) {
            const SQChar* s;
            SQ_PTRS->sq_getstring(vm, 2, &s);
            return SQ_PTRS->sq_throwerror(vm, "Variable not found");
        }

        // push 'this'
        SQ_PTRS->sq_push(vm, 1);

        // Call the setter
        SQ_PTRS->sq_call(vm, 1, SQTrue, SQTrue);
        return 1;
    }

    SQInteger Class::dlgSetStub(HSQUIRRELVM vm) {
        SQ_PTRS->sq_push(vm, 2);
        if (!SQ_SUCCEEDED(SQ_PTRS->sq_get(vm, -2))) {
            const SQChar* s;
            SQ_PTRS->sq_getstring(vm, 2, &s);
            return SQ_PTRS->sq_throwerror(vm, "Variable not found");
        }

        SQ_PTRS->sq_push(vm, 1);

        // Call the getter
        SQ_PTRS->sq_push(vm, 3);
        SQ_PTRS->sq_call(vm, 2, SQTrue, SQTrue);
        return 1;
    }
}
