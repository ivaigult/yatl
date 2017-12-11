/* Copyright (c) 2017 Ivan Vaigult
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#pragma once

#include "error.hpp"

namespace yatl {
namespace lisp_abi {
namespace detail {
template<typename>
struct object_cast_helper;

template<typename underlying_to_t, object::object_type type_to>
struct object_cast_helper<custom_object<underlying_to_t, type_to>&>
{
    typedef custom_object<underlying_to_t, type_to>& result_type;
    result_type do_cast(object& o) {
        if (type_to != o.type)
            throw error::error().format("unexpected object type \'", o.type, "\', \'", type_to, "\' was expected");
        return static_cast<result_type>(o);
    }
};

template<typename underlying_to_t, object::object_type type_to>
struct object_cast_helper<custom_object<underlying_to_t, type_to>*> {
    typedef custom_object<underlying_to_t, type_to>* result_type;
    result_type do_cast(object* o) {
        if (!o)
            return nullptr;
        if (type_to != o->type)
            return nullptr;
        return static_cast<result_type>(o);
    }
};

template<>
struct object_cast_helper<object*> {
    typedef object* result_type;
    result_type do_cast(object* o)
    { return o; }
};

template<>
struct object_cast_helper<object&> {
    typedef object& result_type;
    result_type do_cast(object& o)
    { return o; }
};

}

template<typename type_to_t>
type_to_t object_cast(object* o) { return detail::object_cast_helper<type_to_t>{}.do_cast(o); }
template<typename type_to_t>
type_to_t object_cast(object& o) { return detail::object_cast_helper<type_to_t>{}.do_cast(o); }

template<typename type_to_t>
type_to_t derefference_object_cast(object* o) {
    if (!o)
        throw error::error().format("unexpected \'nil\' object");
    return object_cast<type_to_t>(*o);
}

}
}
