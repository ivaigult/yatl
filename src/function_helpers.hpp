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
#include "functor_traits.hpp"

#include <utility>
#include <functional>

namespace yatl {
namespace utility {
    
template<typename functor_t>
struct simple_function : public lisp_abi::native_function_type {
    typedef typename functor_traits<functor_t>::callable_type callable_type;
    typedef typename functor_traits<functor_t>::args_type args_type;
    
    simple_function(std::string name, callable_type callable)
        : native_function_type(name)
        , _callable(callable)
    {}
    virtual lisp_abi::object* eval(lisp_abi::pair* list) {
        args_type args = utility::validate_signature<args_type>().validate(list);
        using namespace yatl::apply_workaround;
        return std::apply(_callable, args);
    }    
private:    
    callable_type _callable;
};

template<typename functor_t>
void bind_function(machine& m, std::string name, functor_t functor)
{ m.bindings.define(name, m.alloc<yatl::lisp_abi::native_function>(new simple_function<functor_t>(name, functor))); }

template<typename functor_t>
void bind_syntax(machine& m, std::string name, functor_t functor)
{ m.bindings.define(name, m.alloc<yatl::lisp_abi::native_syntax>(new simple_function<functor_t>(name, functor))); }
    
}
}
