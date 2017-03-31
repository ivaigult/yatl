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

template<typename, size_t arg_index>
struct arg_handler;

template<typename value_t, lisp_abi::object::object_type type, size_t arg_index>
struct arg_handler<lisp_abi::custom_object<value_t, type>*, arg_index>
{
    typedef lisp_abi::custom_object<value_t, type>* result_type;
    result_type handle(constant_list_view::iterator& it) {
        lisp_abi::object* current_obj = *it;
        ++it;
        if (!current_obj)
            return nullptr;
        if (current_obj->type != type) {
            throw error::error("Error while parsing arg ", 
				arg_index, 
				": unexpected object type \'", 
				current_obj->type, 
				"\', \'", 
				type, 
				"\' was expected");
        }
        return static_cast<result_type>(current_obj);
    }
};

template<typename value_t, lisp_abi::object::object_type type, size_t arg_index>
struct arg_handler<lisp_abi::custom_object<value_t, type>&, arg_index>
{
    typedef lisp_abi::custom_object<value_t, type>& result_type;
    result_type handle(constant_list_view::iterator& it) {
        lisp_abi::object* current_obj = *it;
        ++it;
        if (!current_obj) {
            throw error::error("Error while parsing arg ",
                arg_index,
                ": got nil, non nil was expected");
        }
        if (current_obj->type != type) {
            throw error::error("Error while parsing arg ",
                arg_index,
                ": unexpected object type \'",
                current_obj->type,
                "\', \'",
                type,
                "\' was expected");
        }
        return static_cast<result_type>(*current_obj);
    }
};

template<size_t arg_index>
struct arg_handler<lisp_abi::object*, arg_index>
{
    typedef lisp_abi::object* result_type;
    result_type handle(constant_list_view::iterator& it) {
        lisp_abi::object* current_obj = *it;
        ++it;
        return static_cast<result_type>(current_obj);
    }
};

template<typename head_t, typename... tail_t, typename... tuple_args_t>
std::tuple<tuple_args_t..., head_t, tail_t...> build_arg_list(std::tuple<tuple_args_t...>&& cur_tuple, constant_list_view::iterator& it) {
    arg_handler<head_t, sizeof...(tuple_args_t) + 1> handler;
    return build_arg_list<tail_t...>(std::tuple_cat(cur_tuple, std::tuple<head_t>(handler.handle(it))), it);
}



template<typename... tuple_args_t>
std::tuple<tuple_args_t...> build_arg_list(std::tuple<tuple_args_t...>&& cur_tuple, constant_list_view::iterator& it)
{ return std::move(cur_tuple); }

}

template<typename... args_t>
std::tuple<args_t...> validate_signature(lisp_abi::pair* list) {
    size_t args_required = sizeof...(args_t);
    constant_list_view list_view(list);

    if (args_required < list_view.size()) {
        throw error::error("too many arguments: ", args_required, " expected, ", list_view.size(), " provided");
    } else if (args_required > list_view.size()) {
        throw error::error("too few arguments: ", args_required, " expected, ", list_view.size(), " provided");
    }

    constant_list_view::iterator it = list_view.begin();
    return detail::build_arg_list<args_t...>(std::forward_as_tuple(), it);
}

}
}