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

#include "machine.hpp"
#include "list_view.hpp"

namespace yatl {

void register_list_bindings(machine& m);
machine::machine() {
    register_list_bindings(*this);
}

lisp_abi::object* machine::eval(lisp_abi::object* object) {
    using namespace lisp_abi;
    if (!object)
        return nullptr;

    switch(object->type) {
    case lisp_abi::object::object_type::number:
    case object::object_type::string:
        return object;
    case object::object_type::symbol:
    {
        lisp_abi::symbol* sym = static_cast<lisp_abi::symbol*>(object);
        std::map<std::string, lisp_abi::object*>::iterator found = _assoc.find(sym->value);
        if (found == _assoc.end())
            throw error::error("symbol \'", sym->value, "\' is unbound");
        return found->second;
    }
    break;
    case object::object_type::pair:
    {
        lisp_abi::pair* list = static_cast<lisp_abi::pair*>(object);
        
        utility::list_view list_view(*this, list);
        lisp_abi::object* fisrt_obj = list_view.front();

        lisp_abi::symbol* sym = lisp_abi::object_cast<lisp_abi::symbol>(fisrt_obj);
        lisp_abi::object* assoc_obj = eval(sym);

        lisp_abi::native_function* callable = lisp_abi::object_cast<lisp_abi::native_function>(assoc_obj);

        lisp_abi::object* tail_obj = list->value.tail;
        lisp_abi::pair*   tail_list = lisp_abi::object_cast<lisp_abi::pair>(list->value.tail);

        // @todo: eval the shit
        return callable->value->eval(*this, tail_list);
    }
    break;
    default:
        break;
    }
    return nullptr;
}

void machine::assoc(std::string name, lisp_abi::object* object) {
    _assoc[name] = object;
}

}
