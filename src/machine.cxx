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
namespace language_core {
void init_language_core(machine& m);
}

machine::machine() 
    : _type_id_cnt(0)
{
    language_core::init_language_core(*this);
}

lisp_abi::object* machine::eval(lisp_abi::object* object) {
    using namespace lisp_abi;
    if (!object)
        return nullptr;

    switch(object->type) {
    case lisp_abi::object::object_type::boolean:
    case lisp_abi::object::object_type::number:
    case object::object_type::string:
        return object;
    case object::object_type::symbol:
    {
        lisp_abi::symbol* sym = static_cast<lisp_abi::symbol*>(object);
        return bindings.lookup(sym->value);
    }
    break;
    case object::object_type::pair:
    {
        lisp_abi::pair* list = static_cast<lisp_abi::pair*>(object);
        
        utility::list_view list_view(*this, list);

        lisp_abi::object* head_obj = list_view.front();
        lisp_abi::object* evaluated_head = eval(head_obj);
        lisp_abi::pair* tail_obj = lisp_abi::object_cast<lisp_abi::pair*>(list->value.tail);

        if (lisp_abi::native_function* callable = lisp_abi::object_cast<lisp_abi::native_function*>(evaluated_head)) {
            utility::list_view arguments_list(*this);
            utility::list_view::iterator it = list_view.begin(); ++it;
            for (; it != list_view.end(); ++it) {
                arguments_list.push_back(eval(*it));
            }
            return callable->value->eval(arguments_list.front_pair());
        } else if(lisp_abi::native_syntax* callable = lisp_abi::object_cast<lisp_abi::native_syntax*>(evaluated_head)) {
            return callable->value->eval(tail_obj);
        } else {
            // @todo: evaluated_head might be null
            throw error::error().format("unable to call ", evaluated_head->type);
        }
    }
    break;
    default:
        break;
    }
    return nullptr;
}

uint32_t machine::alloc_type_id() {
    return ++_type_id_cnt;
}

}
