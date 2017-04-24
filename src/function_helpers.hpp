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
#include "apply_workaround.hpp"
#include "signature_validation.hpp"

#include <utility>

namespace yatl {
namespace utility {

template<typename function_t, function_t func_ptr>
struct simple_function;

template<typename... args_t, lisp_abi::object*(*func_ptr)(machine&, args_t...)>
struct simple_function<lisp_abi::object* (machine&, args_t...), func_ptr> : public lisp_abi::native_function_type {
    simple_function(const char* name)
        : native_function_type(name)
    {}
    virtual lisp_abi::object* eval(machine &m, lisp_abi::pair* list) {
        std::tuple<args_t...> lisp_args = utility::validate_signature<args_t...>(list);
        std::tuple<machine&, args_t&&...> args = std::tuple_cat(std::make_tuple(std::ref(m)), lisp_args);
        using namespace yatl::apply_workaround;
        return std::apply(func_ptr, args);
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