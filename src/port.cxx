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
#include "object_cast.hpp"
#include "register_type.hpp"
#include "machine.hpp"

#include <iostream>

namespace yatl {
namespace io {

struct stream_output_port_impl : public io::output_port {
    stream_output_port_impl(std::ostream& os)
        : _os(os)
    {}

    virtual lisp_abi::object* write_string(lisp_abi::string& string) 
    { 
        _os << string.value; 
        return &string; 
    }

    virtual lisp_abi::object* write_substring(lisp_abi::string& string, lisp_abi::number& start, lisp_abi::number& end)
    {
        // @todo: move me to common algorithms
        std::string& str = string.value;

        size_t left_idx = static_cast<size_t>(start.value);
        if (left_idx >= str.size() ) {
            throw error::error().format("starit index is out of string bounds");
        }

        size_t right_idx = static_cast<size_t>(end.value);
        if (right_idx >= str.size()) {
            throw error::error().format("end index is out of string bounds");
        }

        std::string substr(str.begin() + left_idx, str.begin() + right_idx);
        _os << substr;
        return &string;
    }
    virtual lisp_abi::object* write(lisp_abi::object* obj) { 
        _os << *obj; 
        return obj;
    }
    virtual lisp_abi::object* display(lisp_abi::object* obj) {
        if (lisp_abi::string* str = lisp_abi::object_cast<lisp_abi::string*>(obj)) {
            write_string(*str);
        } else {
            _os << *obj;
        }
        return obj;
    }
    virtual lisp_abi::object* newline() {
        _os << std::endl;
        return nullptr;
    }
    virtual lisp_abi::object* fresh_line() {
        _os << std::endl;
        return nullptr;
    }
    virtual lisp_abi::object* write_line(lisp_abi::object* obj) {
        write(obj);
        newline();
        return obj;
    }

    virtual lisp_abi::object* flush_output() {
        _os << std::flush;
        return nullptr;
    }

    std::ostream& _os;
};

uint32_t output_port::type_id = 0;

typedef user_data_type_impl<stream_output_port_impl, output_port> console_output_port;

lisp_abi::object* create_console_output_port(machine& m) {
    lisp_abi::user_data* result = m.alloc<lisp_abi::user_data>();
    result->value = new console_output_port(std::cout);
    return result;
}

}
}