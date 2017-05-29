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

#include "sym_binding.hpp"
#include "error.hpp"
#include "function_helpers.hpp"

#include <cassert>

namespace yatl {
environment::environment() 
{ 
    _bindings_stack.push_back(std::make_shared<frame>(frame::frame_type::global));
}

lisp_abi::object* environment::lookup(const std::string& name) {
    frame::object_map_t::iterator found = _find_symbol(name);
    return found->second;
}

void environment::define(const std::string& name, lisp_abi::object* obj) {
    assert(!_bindings_stack.empty() && _bindings_stack.front()->type == frame::frame_type::global);
    _bindings_stack.back()->bindings[name] = obj;
}

void environment::undefine(const std::string& name) {
    assert(!_bindings_stack.empty() && _bindings_stack.front()->type == frame::frame_type::global);
    frame& scope = *_bindings_stack.back();
    frame::object_map_t::iterator found = scope.bindings.find(name);
    if (scope.bindings.end() != found) {
        scope.bindings.erase(found);
    }
}

void environment::set(const std::string& name, lisp_abi::object* obj) {
    frame::object_map_t::iterator found = _find_symbol(name);
    found->second = obj;
}

void environment::push_scope(frame_ptr_type scope) {
    _bindings_stack.push_back(scope);
}

void environment::pop_scope() {
    assert(_bindings_stack.size() > 1 && _bindings_stack.front()->type == frame::frame_type::global);
    _bindings_stack.pop_back();
}

frame::object_map_t::iterator environment::_find_symbol(const std::string name) {
    assert(!_bindings_stack.empty() && _bindings_stack.front()->type == frame::frame_type::global);
    for (bindings_stack_t::reverse_iterator it = _bindings_stack.rbegin(); it != _bindings_stack.rend(); ) {
        frame::object_map_t::iterator found = (*it)->bindings.find(name);
        if ((*it)->bindings.end() != found) {
            return found;
        }
        else if ((*it)->type == frame::frame_type::lambda_closure) {
            it = _bindings_stack.rend() - 1;
        }
        else {
            ++it;
        }
    }

    throw error::error().format("symbol \'", name, "\' is not bound");
}
}