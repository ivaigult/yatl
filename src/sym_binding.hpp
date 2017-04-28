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

#include <map>

namespace yatl {
struct scope_bindings {
    enum class scope_type {
		global,
        lambda_closure,
		lambda_args,
		let,
    };

    typedef std::map<std::string, lisp_abi::object*> object_map_t;
	scope_type   type;
    object_map_t bindings;
};

class symbol_space {
public:
    symbol_space();
    lisp_abi::object* lookup(const std::string& name) const;
    void define(const std::string& name, lisp_abi::object*);
    void undefine(const std::string& name);
    void set(const std::string& name, lisp_abi::object*);

    void push_scope(scope_bindings&& scope);
    void pop_scope();
private:
    typedef std::vector<scope_bindings> bindings_stack_t;
    bindings_stack_t  _bindings_stack;
};

struct scope_guard {
    scope_guard(symbol_space& space, scope_bindings&& scope) : _space(space)
    { _space.push_scope(std::move(scope)); }
    ~scope_guard()
    { _space.pop_scope(); }
private:
    symbol_space& _space;
};

}