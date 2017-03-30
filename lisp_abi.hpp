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

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

#include "error.hpp"

namespace yatl {
class machine;
namespace lisp_abi {
struct object {
    enum class object_type {
        unknown,
        pair,
        symbol,
        number,
        string,
        native_function,
        user_data,
    };
    object(object_type type) : type(type) {}
    object(const object&) = default;
    object_type type;
};

template<typename value_type_t>
struct value_holder {
    template<typename... args_t>
    value_holder(args_t&&... args)
        : value(std::forward<args_t>(args)...)
    {}

    typedef value_type_t value_type;
    value_type value;
};

template<>
struct value_holder<void> {
    typedef void value_type;
};

template<typename value_type_t, object::object_type type_id>
struct custom_object : public object, value_holder<value_type_t>{
    template<typename... args_t>
    custom_object(args_t&&... args) 
        : object(type_id)
        , value_holder<value_type_t>(std::forward<args_t>(args)...)
    {}
    typedef std::integral_constant<object::object_type, type_id> type_id_type;
};

struct pair_type {
    pair_type(object* head = nullptr, object* tail = nullptr)
        : head(head), tail(tail)
    {}
    object* head;
    object* tail;
};

typedef custom_object<std::string,           object::object_type::symbol>          symbol;
typedef custom_object<std::string,           object::object_type::string>          string;
typedef custom_object<float,                 object::object_type::number>          number;
typedef custom_object<pair_type,             object::object_type::pair>            pair;

struct native_function_type {
    virtual ~native_function_type() {}
    virtual lisp_abi::object* eval(machine &m, lisp_abi::pair* obj) = 0;
};

typedef custom_object<native_function_type*, object::object_type::native_function> native_function;
typedef custom_object<void*, object::object_type::user_data>                       user_data;

template<typename object_t>
object_t* object_cast(object* o) {
    if (!o)
        return nullptr;
    // @todo: print propper argument types
    if (object_t::type_id_type::value != o->type)
        throw error::error("unexpected object type ", "", "", ", was expected");
    return static_cast<object_t*>(o);
}

const char* object_type2str(object::object_type type);

std::ostream& operator<<(std::ostream& os, const object& obj);

template<typename value_type_t, object::object_type type_id>
std::ostream& operator<<(std::ostream& os, const custom_object<value_type_t, type_id>& obj) { return os << obj.value; }

std::ostream& operator<<(std::ostream& os, const string& obj);
std::ostream& operator<<(std::ostream& os, const pair& obj);
std::ostream& operator<<(std::ostream& os, const native_function& obj);
std::ostream& operator<<(std::ostream& os, const user_data& obj);


}    
}
