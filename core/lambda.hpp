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

#include "lisp_abi.hpp"
#include "list_view.hpp"

#include <vector>
#include <functional>

namespace yatl {

class lambda : public lisp_abi::native_function_type {
public:
    lambda(machine& m, std::vector<std::reference_wrapper<lisp_abi::symbol> > signature, lisp_abi::pair* body);
    lambda(machine& m, const std::string& name, std::vector<std::reference_wrapper<lisp_abi::symbol> > signature, lisp_abi::pair* body);
    virtual lisp_abi::object* eval(lisp_abi::pair* list);
private:
    machine&                    _m;
    closure_type                _closure;
    std::vector<std::string>    _arg_names;
    lisp_abi::pair*             _body;
    utility::constant_list_view _progn;
};

}
