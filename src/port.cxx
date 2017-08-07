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

#include "port.hpp"
#include "object_io.hpp"
#include "register_type.hpp"

#include <iostream>

namespace yatl {

struct console_output_port_impl : public output_port {
    virtual lisp_abi::object* write_string(lisp_abi::string& string) {
        std::cout << string.value << std::endl;
        return nullptr;
    }
    virtual lisp_abi::object* write(lisp_abi::object& obj) {
        std::cout << obj << std::endl;
        return nullptr;
    }
    virtual lisp_abi::object* display(lisp_abi::object& obj) {
        std::cout << obj << std::endl;
        return nullptr;
    }
    virtual lisp_abi::object* new_line() {
        std::cout << std::endl;
        return nullptr;
    }

    static uint32_t type_id;
};

typedef user_data_type_impl<console_output_port_impl, output_port> console_output_port;

}