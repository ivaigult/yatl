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

namespace yatl {
namespace language_core {

lisp_abi::object* quote(machine &m, lisp_abi::object* obj) { return obj; }

// Conditionals
lisp_abi::object* _if(machine &m, lisp_abi::object* obj, lisp_abi::object* t_result, lisp_abi::object* f_result) 
{ return m.eval(obj) ? m.eval(t_result) : m.eval(f_result); }

lisp_abi::object* eval(machine &m, lisp_abi::object* obj)  { return m.eval(obj); }

lisp_abi::object* car(machine &m, lisp_abi::pair& list)     { return list.value.head; }
lisp_abi::object* cdr(machine &m, lisp_abi::pair& list)     { return list.value.tail; }

lisp_abi::object* set_car(machine &m, lisp_abi::pair* list, lisp_abi::object* obj) { return nullptr; }

lisp_abi::object* cons(machine &m, lisp_abi::object* obj, lisp_abi::pair* list) { 
    utility::list_view list_view(m, list);
    list_view.push_front(obj);
    return list_view.front_pair();
}

lisp_abi::object* print(machine& m, lisp_abi::pair* obj)   { std::cout << *obj << std::endl; return obj; }
lisp_abi::object* quit(machine &m)                         { exit(EXIT_SUCCESS); }

void init_language_core(machine& m) {
    YATL_EXPORT_SYNTAX(m, quote);
    YATL_EXPORT_NAMED_SYNTAX(m, "if", _if);
    
    YATL_EXPORT_FUNCTION(m, eval);
    YATL_EXPORT_FUNCTION(m, car);
    YATL_EXPORT_FUNCTION(m, cdr);
    YATL_EXPORT_FUNCTION(m, cons);
    YATL_EXPORT_FUNCTION(m, print);
    YATL_EXPORT_FUNCTION(m, quit);
}


}
}