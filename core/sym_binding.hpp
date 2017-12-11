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

#include "object.hpp"

#include <algorithm>
#include <map>
#include <vector>
#include <memory>

namespace yatl {
struct frame {
    enum class frame_type {
	global,
        lambda_closure,
	lambda_args,
	let,
    };
    frame(frame_type type) 
        : type(type) 
    {}

    typedef std::map<std::string, lisp_abi::object*> object_map_t;
    frame_type   type;
    object_map_t bindings;
};

typedef std::shared_ptr<frame> frame_ptr_type;
typedef std::vector<frame_ptr_type> closure_type;

class environment {
public:
    environment();
    lisp_abi::object* lookup(const std::string& name);
    void define(const std::string& name, lisp_abi::object*);
    void undefine(const std::string& name);
    void set(const std::string& name, lisp_abi::object*);

    void push_scope(frame_ptr_type scope);
    void pop_scope();

    closure_type make_closure();
private:
    frame::object_map_t::iterator _find_symbol(const std::string name);
    typedef std::vector<frame_ptr_type>  bindings_stack_t;

    bindings_stack_t  _bindings_stack;
};

struct scope_guard {
    scope_guard(environment& space, frame_ptr_type scope) : _space(space)
    { _space.push_scope(scope); }
    ~scope_guard()
    { _space.pop_scope(); }
private:
    environment& _space;
};

struct fluid_guard {
    fluid_guard(environment& space, frame_ptr_type scope)
        : _space(space)
        , _orig_values(frame::frame_type::let)
    {
        for(const std::pair<std::string, lisp_abi::object*>& name_val: scope->bindings) {
            const std::string name = name_val.first;
            _orig_values.bindings[name] = _space.lookup(name);
            _space.set(name, name_val.second);
        }
    }

    ~fluid_guard() {
        for(const std::pair<std::string, lisp_abi::object*>& name_val: _orig_values.bindings) {
            _space.set(name_val.first, name_val.second);
        }
    }

private:
    environment& _space;
    frame        _orig_values;
};

struct closure_guard {
    closure_guard(environment& space, closure_type& closure)
        : _space(space)
        , _closure(closure)
    { 
        _space.push_scope(std::make_shared<frame>(frame::frame_type::lambda_closure));
        std::for_each(_closure.begin(), _closure.end(), [this](frame_ptr_type s) {_space.push_scope(s); }); 
    }
    ~closure_guard()
    { 
        std::for_each(_closure.begin(), _closure.end(), [this](frame_ptr_type)   {_space.pop_scope(); }); 
        _space.pop_scope();
    }
private:
    environment& _space;
    closure_type& _closure;
};

}
