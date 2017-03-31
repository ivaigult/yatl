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

#include "lisp_abi.hpp"
#include "machine.hpp"
#include "list_view.hpp"

#include <utility>


namespace yatl {
namespace utility {
namespace detail {
template<size_t current_index, typename... args_t>
struct tuple_runtime_set {
    void operator()(size_t index, std::tuple<args_t...>& args, lisp_abi::object* obj) const
    {
        if (index == (current_index - 1)) {
            typedef std::tuple<args_t...> args_type;
            typedef typename std::tuple_element<current_index - 1, args_type>::type ptr_argument_type;
            typedef typename std::remove_pointer<ptr_argument_type>::type argument_type;
            std::get<current_index - 1>(args) = &lisp_abi::object_cast<argument_type>(*obj);
        } else {
            tuple_runtime_set<current_index - 1, args_t...>{}(index, args, obj);
        }
    }
};

template<typename... args_t>
struct tuple_runtime_set<0, args_t...> {
    bool operator()(size_t, std::tuple<args_t...>&, lisp_abi::object*) const
    { return false; }
};

template<int ...>
struct sequence { };

template<int index, int ...s>
struct gen_sequence : gen_sequence<index - 1, index - 1, s...> { };

template<int ...s>
struct gen_sequence<0, s...> {
    typedef sequence<s...> type;
};

template<typename... args_t>
struct invoke {
    typedef typename gen_sequence<sizeof...(args_t)>::type sequence_type;

    template<int ...s>
    lisp_abi::object* __invoke(sequence<s...>, lisp_abi::object*(*func_ptr)(machine& m, args_t...), machine& m, std::tuple<args_t...>& args) const {
        return func_ptr(m, std::get<s>(args)...);
    }

    lisp_abi::object* operator() (lisp_abi::object*(*func_ptr)(machine& m, args_t...), machine& m, std::tuple<args_t...>& args) const {
        return __invoke(sequence_type{}, func_ptr, m, args);
    }
};
}

template<typename function_t, function_t func_ptr>
struct simple_function;

template<typename... args_t, lisp_abi::object*(*func_ptr)(machine&, args_t...)>
struct simple_function<lisp_abi::object* (machine&, args_t...), func_ptr> : public lisp_abi::native_function_type {
    simple_function(const char* name)
        : native_function_type(name)
    {}
    virtual lisp_abi::object* eval(machine &m, lisp_abi::pair* list) {
        std::tuple<args_t...> args;
        size_t arg_counter = 0;

        utility::list_view list_view(m, list);
        // @todo: handle too many arguments
        for (lisp_abi::object* o : list_view) {
            try {
                detail::tuple_runtime_set<sizeof...(args_t), args_t...> {} (arg_counter, args, o);
            } catch (error::error& e) {
                throw error::error("error while passing argument ", arg_counter, ": ", e.what());
            }
            ++arg_counter;
        };
        

        if (arg_counter < sizeof...(args_t)) {
            throw error::error("insufficient number of arguments ", 
                arg_counter, 
                " provided ", 
                sizeof...(args_t), 
                " required"
            );
        }
        return detail::invoke<args_t...>{}(func_ptr, m, args);
    }
};
}
}

#define YATL_EXPORT_FUNCTION(m, func) do {                                               \
	typedef yatl::utility::simple_function<decltype(func), func> __##func##__t;          \
	m.assoc(#func,  m.alloc<yatl::lisp_abi::native_function>(new __##func##__t(#func))); \
    } while (false)



#define YATL_EXPORT_NAMED_SYNTAX(m, name, func) do {                                 \
	typedef yatl::utility::simple_function<decltype(func), func> __##func##__t;      \
	m.assoc(name,  m.alloc<yatl::lisp_abi::native_syntax>(new __##func##__t(name))); \
    } while (false)

#define YATL_EXPORT_SYNTAX(m, func) YATL_EXPORT_NAMED_SYNTAX(m, #func, func)
