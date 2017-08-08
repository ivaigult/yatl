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
#include <fstream>

namespace yatl {
namespace io {

class input_port_impl : public io::input_port {
public:
    input_port_impl(machine& m, std::istream& is)
        : _m(m)
        , _is(is)
    {}

    virtual lisp_abi::object* read()
    {
        // @todo: this will be painfull to implement,
        // parser and tokenizer are needed here
        return nullptr;
    }

    virtual lisp_abi::object* is_eof()
    {
        return _m.alloc<lisp_abi::boolean>(_is.eof());
    }

    virtual lisp_abi::object* read_string()
    {
        lisp_abi::string* result = _m.alloc<lisp_abi::string>();
        _is >> result->value;
        return result;
    }

    virtual lisp_abi::object* read_line()
    {
        lisp_abi::string* result = _m.alloc<lisp_abi::string>();
        std::getline(_is, result->value);
        return result;
    }
protected:
    machine&      _m;
    std::istream& _is;
};

class output_port_impl : public io::output_port {
public:
    output_port_impl(std::ostream& os)
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
private:
    std::ostream& _os;
};

template<typename stream_t>
class stream_holder {
public:
    template<typename... args_t>
    stream_holder(args_t&&... args) 
        : _stream(std::forward<args_t>(args)...)
    {}
protected:
    stream_t _stream;
};

template<>
class stream_holder<void>
{};

struct stdin_port_impl : public stream_holder<void>, public input_port_impl
{
    stdin_port_impl(machine& m)
        : stream_holder<void>()
        , input_port_impl(m, std::cin)
    {}
};

struct file_input_port_impl : public stream_holder<std::ifstream>, public input_port_impl
{
    file_input_port_impl(machine& m, lisp_abi::string& filename)
        : stream_holder<std::ifstream>(filename.value)
        , input_port_impl(m, _stream)
    {}
};

struct stdout_port_impl : public stream_holder<void>, public output_port_impl
{
    stdout_port_impl()
        : stream_holder<void>()
        , output_port_impl(std::cout)
    {}
};

struct file_output_port_impl : public stream_holder<std::ofstream>, public output_port_impl
{
    file_output_port_impl(lisp_abi::string& filename)
        : stream_holder<std::ofstream>(filename.value)
        , output_port_impl(_stream)
    {}
};

typedef user_data_type_impl<stdin_port_impl,      input_port>   console_input_port;
typedef user_data_type_impl<file_input_port_impl, input_port>   file_input_port;

typedef user_data_type_impl<stdout_port_impl,      output_port> console_output_port;
typedef user_data_type_impl<file_output_port_impl, output_port> file_output_port;

lisp_abi::object* create_console_input_port(machine& m) {
    lisp_abi::user_data* result = m.alloc<lisp_abi::user_data>();
    result->value = new console_input_port(m);
    return result;
}

lisp_abi::object* open_input_file(machine& m, lisp_abi::string& filename) {
    lisp_abi::user_data* result = m.alloc<lisp_abi::user_data>();
    result->value = new file_input_port(m, filename);
    return result;
}

lisp_abi::object* create_console_output_port(machine& m) {
    lisp_abi::user_data* result = m.alloc<lisp_abi::user_data>();
    result->value = new console_output_port();
    return result;
}

lisp_abi::object* open_output_file(machine& m, lisp_abi::string& filename) {
    lisp_abi::user_data* result = m.alloc<lisp_abi::user_data>();
    result->value = new file_output_port(filename);
    return result;
}


uint32_t output_port::type_id = 0;
uint32_t input_port::type_id = 0;

}
}