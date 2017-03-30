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


namespace yatl {
namespace lisp_abi {

const char* object_type2str(object::object_type type) {
    switch (type) {
    case object::object_type::pair:            return "pair";
    case object::object_type::symbol:          return "symbol";
    case object::object_type::number:          return "number";
    case object::object_type::string:          return "string";
    case object::object_type::native_function: return "native_function";
    case object::object_type::user_data:       return "user_data";
    default:                                   return "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const string& obj) {
    // @todo: escape symbols
    os << "\"" << obj.value << "\"";
    return os;
}

std::ostream& operator<<(std::ostream& os, const pair& obj) {
    os << "(";
	// @todo: get rid of recursion and use stack
    const utility::constant_list_view list_view(const_cast<pair*>(&obj));
    utility::constant_list_view::iterator it = list_view.begin();
    for (;;) {
        os << **it;
        ++it;
        if (list_view.end() == it)
            break;
        else
            os << " ";
    }
    os << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const native_function& obj) {
    os << "<native_function: " << obj.value << ">";
    return os;
}

std::ostream& operator<<(std::ostream& os, const user_data& obj) {
    os << "<user_data: " << obj.value << ">";
    return os;
}

std::ostream& operator<<(std::ostream& os, const object& obj) {
    if (!(&obj)) {
        os << "nil";
        return os;
    }
	// @todo: use type list
    switch (obj.type) {
    case object::object_type::pair:            return os << static_cast<const pair&>(obj);
    case object::object_type::symbol:          return os << static_cast<const symbol&>(obj);
    case object::object_type::number:          return os << static_cast<const number&>(obj);
    case object::object_type::string:          return os << static_cast<const string&>(obj);
    case object::object_type::native_function: return os << static_cast<const native_function&>(obj);
    case object::object_type::user_data:       return os << static_cast<const user_data&>(obj);
    default: return os;
    }
}

}
}