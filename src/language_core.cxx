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

#include "function_helpers.hpp"
#include "machine.hpp"
#include "lambda.hpp"
#include "internal_helpers.hpp"

#include <algorithm>
#include <cassert>

namespace yatl {
namespace language_core {

template<lisp_abi::object::object_type type>
struct type_predicate {
    type_predicate(machine& m)
        : m(m)
    {}
    type_predicate(const type_predicate&) = default;
    
    lisp_abi::object* operator()(lisp_abi::object* o) {
        lisp_abi::boolean* result = m.alloc<lisp_abi::boolean>(false);
        if (o) {
            result->value = type == o->type;
        }
        return result;
    }
    machine& m;
};

void init_language_core(machine& m) {
    utility::bind_syntax(m, "define", [&m](lisp_abi::object& first, utility::rest_arguments<lisp_abi::pair*> o) -> lisp_abi::object* {
        lisp_abi::object* result = nullptr;
        if (lisp_abi::symbol* sym = lisp_abi::object_cast<lisp_abi::symbol*>(&first)) {
            if (o.args) {
                result = m.eval(o.args->value.head);
                m.bindings.define(sym->value, result);
            } else {
                m.bindings.undefine(sym->value);
            }
        } else if (lisp_abi::pair* pair = lisp_abi::object_cast<lisp_abi::pair*>(&first)) {
            typedef std::tuple<lisp_abi::symbol&, utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::symbol> > > > name_and_args_t;
            name_and_args_t signature = utility::validate_signature<name_and_args_t>().validate(pair);
            std::string name = std::get<0>(signature).value;
            result = m.alloc<lisp_abi::native_function>(new lambda(m, name, std::move(std::get<1>(signature).args), o.args));
            m.bindings.define(name, result);
        } else {
            throw error::error().format("unexpected object type \'", first.type, "\' sym or list was expected");
        }
        return result;
    });
    utility::bind_syntax(m, "set!",   [&m](lisp_abi::symbol& s, lisp_abi::object* o) {
        lisp_abi::object* result = m.eval(o);
        m.bindings.set(s.value, result);
        return result;
    });
    utility::bind_syntax(m, "let", [&m](std::vector<std::tuple<lisp_abi::symbol&, lisp_abi::object*> > bindings, utility::rest_arguments<lisp_abi::pair*> body) {
        scope_bindings scope = { scope_bindings::scope_type::let };
        std::for_each(bindings.begin(), bindings.end(), [&scope, &m](std::tuple<lisp_abi::symbol&, lisp_abi::object*>& b)
        { scope.bindings[std::get<0>(b).value] = m.eval(std::get<1>(b)); } );

        scope_guard g(m.bindings, std::move(scope));
        return utility::begin(m, body.args);
    });

    utility::bind_syntax(m, "lambda", [&m](std::vector<std::reference_wrapper<lisp_abi::symbol> > signature, utility::rest_arguments<lisp_abi::pair*> body) -> lisp_abi::object* {
        lisp_abi::native_function* result = m.alloc<lisp_abi::native_function>(new lambda(m, std::move(signature), body.args));
        return result;
    });

    utility::bind_syntax(m, "named-lambda", [&m](std::tuple<lisp_abi::symbol&, utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::symbol> > > > signature,
                                                 utility::rest_arguments<lisp_abi::pair*> body) -> lisp_abi::object* {
        lisp_abi::native_function* result = m.alloc<lisp_abi::native_function>(new lambda(m, std::get<0>(signature).value, std::move(std::get<1>(signature).args), body.args));
        return result;
    });

    utility::bind_syntax(m, "cond", [&m](utility::rest_arguments<std::vector< std::tuple<lisp_abi::object*, lisp_abi::object*> > > cond_value_list) -> lisp_abi::object* {
        for (std::tuple<lisp_abi::object*, lisp_abi::object*>& cond_value : cond_value_list.args) {
            if (utility::to_boolean(m.eval(std::get<0>(cond_value)))) {
                return m.eval(std::get<1>(cond_value));
            }
        }
        return nullptr;
    });
    utility::bind_function(m, "=", [&m](lisp_abi::number& l, lisp_abi::number& r) {
        return m.alloc<lisp_abi::boolean>(l.value == r.value);
    });

    utility::bind_syntax(m,   "quote",[&m](lisp_abi::object* o) { return o; } );

    utility::bind_function(m, "not", [&m](lisp_abi::object* o) { return m.alloc<lisp_abi::boolean>(!utility::to_boolean(o)); });

    utility::bind_function(m, "boolean?", type_predicate<lisp_abi::object::object_type::boolean> {m});
    utility::bind_function(m, "number?",  type_predicate<lisp_abi::object::object_type::number>  {m});
    utility::bind_function(m, "string?",  type_predicate<lisp_abi::object::object_type::string>  {m});
    utility::bind_function(m, "symbol?",  type_predicate<lisp_abi::object::object_type::symbol>  {m});
    utility::bind_function(m, "pair?",    type_predicate<lisp_abi::object::object_type::pair>    {m});

    utility::bind_function(m, "eval", [&m](lisp_abi::object* o) { return m.eval(o); });
    utility::bind_function(m, "car",  [&m](lisp_abi::pair&   l) { return l.value.head; });
    utility::bind_function(m, "cdr",  [&m](lisp_abi::pair&   l) { return l.value.tail; });
    utility::bind_function(m, "cons", [&m](lisp_abi::object* o, lisp_abi::pair* l) {
        utility::list_view list_view(m, l);
        list_view.push_front(o);
        return list_view.front_pair();
    });
    utility::bind_function(m, "list", [&m](utility::rest_arguments<lisp_abi::pair*> elements) { return elements.args; });
    utility::bind_function(m, "map", [&m](lisp_abi::native_function& fn, lisp_abi::pair* pair) {
        utility::constant_list_view list(pair);
        lisp_abi::object* result = nullptr;
        for (utility::constant_list_view::iterator it = list.begin(); it != list.end(); ++it) {
            utility::list_view arg_list(m);
            arg_list.push_back(*it);
            result = fn.value->eval(arg_list.front_pair());
        }
        return result;
    });

    utility::bind_function(m, "+",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(0.0f);
        std::for_each(numbers.args.begin(), numbers.args.end(), [&result](const lisp_abi::number& n) { result->value += n.value; });
        return result;
    });
    utility::bind_function(m, "-",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(0.0f);
        if (numbers.args.empty())
            return result;
        result->value = numbers.args.front().get().value;
        std::for_each(numbers.args.begin() + 1, numbers.args.end(), [&result](const lisp_abi::number& n) { result->value -= n.value; });
        return result;
    });
    utility::bind_function(m, "*",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(1.0f);
        std::for_each(numbers.args.begin(), numbers.args.end(), [&result](const lisp_abi::number& n) { result->value *= n.value; });
        return result;
    });
    utility::bind_function(m, "/",     [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
        lisp_abi::number* result = m.alloc<lisp_abi::number>(1.0f);
        if (numbers.args.empty()) {
            return result;
        } else if (1 == numbers.args.size()) {
            result->value = 1.0f / numbers.args.front().get().value;
            return result;
        }
        result->value = numbers.args.front().get().value;
        std::for_each(numbers.args.begin() + 1, numbers.args.end(), [&result](const lisp_abi::number& n) { result->value /= n.value; });
        return result;
    });

    utility::bind_function(m, "print", [&m](utility::rest_arguments<lisp_abi::pair*> rest) 
    { 
        utility::constant_list_view arg_list(rest.args);
        lisp_abi::object* result = nullptr;
        for (utility::constant_list_view::iterator it = arg_list.begin(); it != arg_list.end(); ++it) {
            result = *it;
            std::cout << *result;
        }
        std::cout << std::endl;
        return result;
    });
    utility::bind_function(m, "quit",  [&m]() -> lisp_abi::object* { exit(EXIT_SUCCESS); return nullptr; });
    utility::bind_function(m, "exit",  [&m](lisp_abi::number& code) -> lisp_abi::object* { exit(static_cast<int>(code.value)); });

    utility::bind_function(m, "raise", [&m](lisp_abi::object* o) -> lisp_abi::object* { throw error::runtime_error(m, o); });
    utility::bind_function(m, "error", [&m](lisp_abi::string& message, utility::rest_arguments<lisp_abi::pair*> rest) -> lisp_abi::object* {
        std::stringstream error_stream;
        error_stream << message << " ";
        utility::constant_list_view objs(rest.args);
        for (utility::constant_list_view::iterator it = objs.begin(); it != objs.end(); ++it) {
            error_stream << (*it) << " ";
        }
        throw error::runtime_error(m, message, rest.args);
    });
    utility::bind_function(m, "with-exception-handler", [&m](lisp_abi::native_function& handler, lisp_abi::native_function& thunk) {
        lisp_abi::object* result = nullptr;
        try {
            result = thunk.value->eval(nullptr);
        } catch (const error::runtime_error& e) {
            utility::list_view arg_list(m);
            arg_list.push_back(e.raise_obj);
            result = handler.value->eval(arg_list.front_pair());
        }
        return result;
    });
}

}
}
