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

#include "function_helpers.hpp"
#include "machine.hpp"

#include <algorithm>
#include <cassert>

namespace yatl {
namespace language_core {
   
void init_language_core(machine& m) {
    utility::bind_syntax(m, "define", [&m](lisp_abi::symbol& s, lisp_abi::object* o) { 
        lisp_abi::object* result = m.eval(o);
        m.bindings.define(s.value, result); 
        return result;
    });
    utility::bind_syntax(m, "set!",   [&m](lisp_abi::symbol& s, lisp_abi::object* o) {
        lisp_abi::object* result = m.eval(o);
        m.bindings.define(s.value, result);
        return result;
    });
    utility::bind_syntax(m, "let", [&m](std::vector<std::tuple<lisp_abi::symbol&, lisp_abi::object*> > bindings, utility::rest_arguments<lisp_abi::pair*> body) {
        scope_bindings scope = { scope_bindings::scope_type::let };
        std::for_each(bindings.begin(), bindings.end(), [&scope, &m](std::tuple<lisp_abi::symbol&, lisp_abi::object*>& b)
        { scope.bindings[std::get<0>(b).value] = m.eval(std::get<1>(b)); } );
        scope_guard g(m.bindings, std::move(scope));

        utility::constant_list_view progn(body.args);
        lisp_abi::object* result = nullptr;
        for (lisp_abi::object* expr : progn) {
            result = m.eval(expr);
        }
        return result;
    });

    utility::bind_syntax(m,   "quote",[&m](lisp_abi::object* o) { return o; } );
    utility::bind_syntax(m,   "if",   [&m](lisp_abi::object* o, lisp_abi::object* l, lisp_abi::object* r) { return m.eval(o)? m.eval(l) : m.eval(r); });

    utility::bind_function(m, "eval", [&m](lisp_abi::object* o) { return m.eval(o); });
    utility::bind_function(m, "car",  [&m](lisp_abi::pair&   l) { return l.value.head; });
    utility::bind_function(m, "cdr",  [&m](lisp_abi::pair&   l) { return l.value.tail; });
    utility::bind_function(m, "cons", [&m](lisp_abi::object* o, lisp_abi::pair* l) {
        utility::list_view list_view(m, l);
        list_view.push_front(o);
        return list_view.front_pair();
    });
    utility::bind_function(m, "+",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(0.0f);
        std::for_each(numbers.args.begin(), numbers.args.end(), [&result](const lisp_abi::number& n) { result->value += n.value; });
        return result;
    });
    utility::bind_function(m, "-",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(0.0f);
        std::for_each(numbers.args.begin(), numbers.args.end(), [&result](const lisp_abi::number& n) { result->value -= n.value; });
        return result;
    });
    utility::bind_function(m, "*",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(1.0f);
        std::for_each(numbers.args.begin(), numbers.args.end(), [&result](const lisp_abi::number& n) { result->value *= n.value; });
        return result;
    });
    utility::bind_function(m, "/",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(1.0f);
        if (numbers.args.empty()) {
            return result;
        } else if (1 == numbers.args.size()) {
            result->value = 1.0f / numbers.args.front().get().value;
            return result;
        }
        result->value = numbers.args.front().get().value;
        std::for_each(numbers.args.begin() + 1, numbers.args.end(), [&result](const lisp_abi::number& n) { result->value /= n.value; });
        return result;
    });

    utility::bind_function(m, "print", [&m](lisp_abi::object* o){ std::cout << *o << std::endl; return o; });
    utility::bind_function(m, "quit",  [&m]() -> lisp_abi::object* { exit(EXIT_SUCCESS); return nullptr; });
}

}
}
