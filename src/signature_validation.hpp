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

#include "lisp_abi.hpp"
#include "list_view.hpp"
#include "error.hpp"

namespace yatl {
namespace utility {

namespace detail {

template<typename>
struct arg_handler;

template<typename value_t, lisp_abi::object::object_type type>
struct arg_handler<lisp_abi::custom_object<value_t, type>*>
{
    typedef lisp_abi::custom_object<value_t, type>* result_type;
    result_type operator()(constant_list_view::iterator& it) {
        lisp_abi::object* current_obj = *it;
        if (!current_obj)
            return nullptr;
        if (current_obj->type != type) {
            throw error::error().format("unexpected object type \'",
				current_obj->type, 
				"\', \'", 
				type, 
				"\' was expected");
        }
        return static_cast<result_type>(current_obj);
    }
};

template<typename value_t, lisp_abi::object::object_type type>
struct arg_handler<lisp_abi::custom_object<value_t, type>&>
{
    typedef lisp_abi::custom_object<value_t, type>& result_type;
    result_type operator()(constant_list_view::iterator& it) {
        lisp_abi::object* current_obj = *it;
        if (!current_obj) {
            throw error::error().format("got nil, non nil was expected");
        }
        if (current_obj->type != type) {
            throw error::error().format("unexpected object type \'",
                current_obj->type,
                "\', \'",
                type,
                "\' was expected");
        }
        return static_cast<result_type>(*current_obj);
    }
};

template<>
struct arg_handler<lisp_abi::object*>
{
    typedef lisp_abi::object* result_type;
    result_type operator()(constant_list_view::iterator& it) {
        return static_cast<result_type>(*it);
    }
};

template<template<typename> class functional_t, typename... args_t>
struct list2tuple;

template<template<typename> class functional_t, typename head_t, typename... tail_t>
struct list2tuple<functional_t, head_t, tail_t...> {
    std::tuple<head_t, tail_t...> convert(constant_list_view::iterator& it) {
        functional_t<head_t> func;
        list2tuple<functional_t, tail_t...> next_iteration;
        std::tuple<head_t, tail_t...> result = std::tuple_cat(std::forward_as_tuple(func(it)), next_iteration.convert(it));
        ++it;
        return std::move(result);
    }
};

template<template<typename> class functional_t>
struct list2tuple<functional_t> {
    std::tuple<> convert(constant_list_view::iterator& it) {
        return std::tuple<>();
    }
};
}

template<typename... args_t>
std::tuple<args_t...> validate_signature(lisp_abi::pair* list) {
    size_t args_required = sizeof...(args_t);
    constant_list_view list_view(list);

    if (args_required < list_view.size()) {
        throw error::error().format("too many arguments: ", args_required, " expected, ", list_view.size(), " provided");
    } else if (args_required > list_view.size()) {
        throw error::error().format("too few arguments: ", args_required, " expected, ", list_view.size(), " provided");
    }

    constant_list_view::iterator it = list_view.begin();
    detail::list2tuple<detail::arg_handler, args_t...> list2tuple;
    return list2tuple.convert(it);
}

}
}