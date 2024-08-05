#include "../include/simplesquirrel/array.hpp"
#include "../include/simplesquirrel/exceptions.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
    Array::Array(HSQUIRRELVM vm, size_t len):Object(vm) {
        SQ_PTRS->sq_newarray(vm, len);
        SQ_PTRS->sq_getstackobj(vm, -1, &obj);
        SQ_PTRS->sq_addref(vm, &obj);
        SQ_PTRS->sq_pop(vm,1); // Pop array
    }

    Array::Array(const Object& object):Object(object) {
        if (object.getType() != Type::ARRAY) throw TypeException("bad cast", "ARRAY", object.getTypeStr());
    }

    Array::Array(const Array& other):Object(other) {
            
    }

    Array::Array(Array&& other) NOEXCEPT :Object(std::forward<Array>(other)) {
            
    }

    size_t Array::size() {
        SQ_PTRS->sq_pushobject(vm, obj);
        SQInteger s = SQ_PTRS->sq_getsize(vm, -1);
        SQ_PTRS->sq_pop(vm, 1);
        return static_cast<size_t>(s);
    }

    std::vector<Object> Array::convertRaw() {
        SQ_PTRS->sq_pushobject(vm, obj);
        SQInteger s = SQ_PTRS->sq_getsize(vm, -1);
        std::vector<Object> ret;
        ret.reserve(s);
        while(s--) {
            if(SQ_FAILED(SQ_PTRS->sq_arraypop(vm, -1, true))) {
                SQ_PTRS->sq_pop(vm, 1);
                throw TypeException("Failed to pop value from back of the array");
            }
            ret.push_back(detail::pop<Object>(vm, -1));
        }
        SQ_PTRS->sq_pop(vm, 1);
        return ret;
    }

    void Array::pop() {
        SQ_PTRS->sq_pushobject(vm, obj);
        auto s = SQ_PTRS->sq_getsize(vm, -1);
        if(s == 0) {
            SQ_PTRS->sq_pop(vm, 1);
            throw TypeException("Out of bounds");
        }

        if(SQ_FAILED(SQ_PTRS->sq_arraypop(vm, -1, false))) {
            SQ_PTRS->sq_pop(vm, 1);
            throw TypeException("Failed to pop value from back of the array");
        }
        SQ_PTRS->sq_pop(vm, 1);
    }

    Array& Array::operator = (const Array& other){
        Object::operator = (other);
        return *this;
    }

    Array& Array::operator = (Array&& other) NOEXCEPT {
        Object::operator = (std::forward<Array>(other));
        return *this;
    }
}
