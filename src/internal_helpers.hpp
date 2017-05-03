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
        return false;
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

}
}