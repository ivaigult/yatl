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
#include <functional>

namespace yatl {
namespace utility {

template<typename function_t>
struct simple_function;

// @todo: rewrite me for any functor type
template<typename... args_t>
struct simple_function<lisp_abi::object* (machine&, args_t...)> : public lisp_abi::native_function_type {
    simple_function(const char* name, lisp_abi::object* (*callable)(machine&, args_t...))
        : native_function_type(name)
        , _callable(callable)
    {}
    virtual lisp_abi::object* eval(machine &m, lisp_abi::pair* list) {
        typedef std::tuple<args_t...> lisp_args_t;
        lisp_args_t lisp_args = utility::validate_signature<lisp_args_t>().validate(list);
        std::tuple<machine&, args_t...> args = std::tuple_cat(std::make_tuple(std::ref(m)), std::move(lisp_args));
        using namespace yatl::apply_workaround;
        return std::apply(_callable, args);
    }

private:
    lisp_abi::object* (*_callable)(machine&, args_t...);
};
}
}

#define YATL_EXPORT_FUNCTION(m, func) do {                              \
        typedef yatl::utility::simple_function<decltype(func)> __##func##__t; \
        m.assoc(#func,  m.alloc<yatl::lisp_abi::native_function>(new __##func##__t(#func, func))); \
    } while (false)



#define YATL_EXPORT_NAMED_SYNTAX(m, name, func) do {                    \
        typedef yatl::utility::simple_function<decltype(func)> __##func##__t; \
        m.assoc(name,  m.alloc<yatl::lisp_abi::native_syntax>(new __##func##__t(name, func))); \
    } while (false)

#define YATL_EXPORT_SYNTAX(m, func) YATL_EXPORT_NAMED_SYNTAX(m, #func, func)
