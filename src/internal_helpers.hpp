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
#include "list_view.hpp"

namespace yatl {
namespace utility {

inline bool to_boolean(lisp_abi::object* o) {
    if (!o)
        return true;
    lisp_abi::boolean* boolean = lisp_abi::object_cast<lisp_abi::boolean*>(o);
    if (!boolean)
        return true;
    return boolean->value;
}

inline lisp_abi::object* begin(machine& m, lisp_abi::pair* pair) {
    utility::constant_list_view progn(pair);
    lisp_abi::object* result = nullptr;
    for (lisp_abi::object* expr : progn) {
        result = m.eval(expr);
    }
    return result;

}

inline bool equivalent(lisp_abi::object* a, lisp_abi::object* b) {
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;
    if (a->type != b->type)
        return false;

    if (lisp_abi::boolean* boolean_a = lisp_abi::object_cast<lisp_abi::boolean*>(a))
        return boolean_a->value == static_cast<lisp_abi::boolean*>(b)->value;
    if (lisp_abi::number* number_a = lisp_abi::object_cast<lisp_abi::number*>(a))
        return number_a->value == static_cast<lisp_abi::number*>(b)->value;
    if (lisp_abi::string* string_a = lisp_abi::object_cast<lisp_abi::string*>(a))
        return string_a->value == static_cast<lisp_abi::string*>(b)->value;
    if (lisp_abi::symbol* symbol_a = lisp_abi::object_cast<lisp_abi::symbol*>(a))
        return symbol_a->value == static_cast<lisp_abi::symbol*>(b)->value;
    if (lisp_abi::native_syntax* func_a = lisp_abi::object_cast<lisp_abi::native_syntax*>(a))
        return func_a->value == static_cast<lisp_abi::native_syntax*>(b)->value;
    if (lisp_abi::native_function* func_a = lisp_abi::object_cast<lisp_abi::native_function*>(a))
        return func_a->value == static_cast<lisp_abi::native_function*>(b)->value;

    return a == b;

}

inline bool equal(lisp_abi::object* a, lisp_abi::object* b) {
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;
    if (a->type != b->type)
        return false;
    if (lisp_abi::pair* pair_a = lisp_abi::object_cast<lisp_abi::pair*>(a)) {
        lisp_abi::pair* pair_b = static_cast<lisp_abi::pair*>(b);
        return equal(pair_a->value.head, pair_b->value.head) && equal(pair_a->value.tail, pair_b->value.tail);
    }

    return equivalent(a, b);
}

}
}