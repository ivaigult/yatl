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

#include "error.hpp"
#include "machine.hpp"
#include "list_view.hpp"

namespace yatl {
namespace error {

runtime_error::runtime_error(machine& m, lisp_abi::object* obj) {
    format("runtime_error: ", *obj);
    raise_obj = obj;
}

runtime_error::runtime_error(machine& m, lisp_abi::string& message, lisp_abi::pair* irritants) {
    format("runtime_error: ", message.value);

    utility::list_view raise_obj_list(m);
    utility::constant_list_view irritants_list(irritants);

    raise_obj_list.push_back(&message);
    for (utility::constant_list_view::iterator it = irritants_list.begin(); it != irritants_list.end(); ++it) {
        raise_obj_list.push_back(*it);
    }

    raise_obj = raise_obj_list.front_pair();
}

}
}