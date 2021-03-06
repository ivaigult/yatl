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

#include "object.hpp"

namespace yatl {
class machine;

namespace io {

struct input_port {
    // @todo: implement me, once chars are supported
    // virtual lisp_abi::object* read_char() = 0;
    // virtual lisp_abi::object* peek_char() = 0;
    // virtual lisp_abi::object* is_char_ready() = 0;
    virtual lisp_abi::object* read() = 0;
    virtual lisp_abi::object* is_eof() = 0;
    virtual lisp_abi::object* read_string() = 0;
    virtual lisp_abi::object* read_line() = 0;

    static uint32_t type_id;
};

struct output_port {
    virtual ~output_port() {}

    // @todo: implement me, once chars are supported
    // virtual lisp_abi::object* write_char() = 0;
    virtual lisp_abi::object* write_string(lisp_abi::string& string) = 0;
    virtual lisp_abi::object* write_substring(lisp_abi::string& string, lisp_abi::number& start, lisp_abi::number& end) = 0;
    virtual lisp_abi::object* write(lisp_abi::object* obj) = 0;
    virtual lisp_abi::object* display(lisp_abi::object* obj) = 0;
    virtual lisp_abi::object* newline() = 0;
    virtual lisp_abi::object* fresh_line() = 0;
    virtual lisp_abi::object* write_line(lisp_abi::object* obj) = 0;
    virtual lisp_abi::object* flush_output() = 0;

    static uint32_t type_id;
};

lisp_abi::object* create_console_input_port(machine& m);
lisp_abi::object* open_input_file(machine& m, lisp_abi::string& filename);

lisp_abi::object* create_console_output_port(machine& m);
lisp_abi::object* open_output_file(machine& m, lisp_abi::string& filename);

}
}