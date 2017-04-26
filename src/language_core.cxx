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

namespace yatl {
namespace language_core {

    
lisp_abi::object* add(machine &m, std::vector<lisp_abi::number> numbers) {
    lisp_abi::number* result = m.alloc<lisp_abi::number>(0.0);
    std::for_each(numbers.begin(), numbers.end(), [&result](const lisp_abi::number& n) { result->value += n.value; });
    return result;
}        


void init_language_core(machine& m) {
    utility::bind_syntax(m, "quote",  [&m](lisp_abi::object* o) { return o; } );
    utility::bind_syntax(m, "if",     [&m](lisp_abi::object* o, lisp_abi::object* l, lisp_abi::object* r) { return m.eval(o)? m.eval(l) : m.eval(r); });

    utility::bind_function(m, "eval", [&m](lisp_abi::object* o) { return m.eval(o); });
    utility::bind_function(m, "car",  [&m](lisp_abi::pair&   l) { return l.value.head; });
    utility::bind_function(m, "cdr",  [&m](lisp_abi::pair&   l) { return l.value.tail; });
    utility::bind_function(m, "cons", [&m](lisp_abi::object* o, lisp_abi::pair* l) {
        utility::list_view list_view(m, l);
        list_view.push_front(o);
        return list_view.front_pair();
    });
    
    utility::bind_function(m, "print", [&m](lisp_abi::object* o){ std::cout << *o << std::endl; return o; });
    utility::bind_function(m, "quit",  [&m]() -> lisp_abi::object* { exit(EXIT_SUCCESS); return nullptr; });
    // YATL_EXPORT_FUNCTION(m, add);
    // 

    // YATL_EXPORT_FUNCTION(m, cons);
    // YATL_EXPORT_FUNCTION(m, print);
}

}
}
