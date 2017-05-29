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

#include "lambda.hpp"
#include "machine.hpp"
#include "list_view.hpp"
#include "internal_helpers.hpp"

#include <algorithm>

namespace yatl {

lambda::lambda(machine& m, std::vector<std::reference_wrapper<lisp_abi::symbol> > signature, lisp_abi::pair* body) 
    : native_function_type("lambda")
    , _m(m)
    , _closure(m.bindings.make_closure())
    , _body(body)
    , _progn(_body)
{
    std::for_each(signature.begin(), signature.end(), [this](lisp_abi::symbol& s) { this->_arg_names.push_back(s.value); });
}

lambda::lambda(machine& m, const std::string& name, std::vector<std::reference_wrapper<lisp_abi::symbol> > signature, lisp_abi::pair* body)
    : native_function_type(name)
    , _m(m)
    , _closure(m.bindings.make_closure())
    , _body(body)
    , _progn(_body)
{
    std::for_each(signature.begin(), signature.end(), [this](lisp_abi::symbol& s) { this->_arg_names.push_back(s.value); });
}

lisp_abi::object* lambda::eval(lisp_abi::pair* list) {
    utility::constant_list_view args(list);
    if (_arg_names.size() < args.size()) {
        throw error::error().format("too many arguments: ", _arg_names.size(), " expected, ", args.size(), " provided");
    }
    else if (_arg_names.size() > args.size()) {
        throw error::error().format("too few arguments: ", _arg_names.size(), " expected, ", args.size(), " provided");
    }

    frame_ptr_type function_arguments = std::make_shared<frame>(frame::frame_type::lambda_args);
    utility::constant_list_view::iterator it = args.begin();

    for (size_t ii = 0; ii < _arg_names.size(); ++ii, ++it) {
        function_arguments->bindings[_arg_names[ii]] = *it;
    }

    closure_guard c(_m.bindings, _closure);
    scope_guard g(_m.bindings, std::move(function_arguments));
    return utility::begin(_m, _body);
}

}
