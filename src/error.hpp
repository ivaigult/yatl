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

#include <iostream>
#include <stdexcept>
#include <sstream>

namespace yatl {
namespace error {
struct error;
namespace detail {
template<typename head_t, typename... tail_t>
void print(std::ostream& o, head_t&& head, tail_t&&... tail);
inline void print(std::ostream&) {};
    
template<typename head_t, typename... tail_t>
void print(std::ostream& o, head_t&& head, tail_t&&... tail)
{
        o << head;
        print(o, std::forward<tail_t>(tail)...);
}
}

struct error : std::exception {

    template<typename... args_t>
    error& format(args_t&&... args)
    {
        std::stringstream message_stream;
        detail::print(message_stream, std::forward<args_t>(args)...);
        _message += message_stream.str();
        return *this;
    }

    virtual ~error() throw() {}
    virtual const char* what() const throw() { return _message.c_str(); }
protected:
    std::string    _message;
};

struct runtime_error : error {
    runtime_error()
        : error()
    { _message = "runtime_error: "; }
};

}
}
