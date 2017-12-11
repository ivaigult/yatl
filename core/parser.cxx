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

#include "parser.hpp"

#include "error.hpp"

#include <cassert>
#include <cstdlib>

namespace yatl {

parser::object_stream_t& parser::parse(const tokenizer::token_stream_t& tokens)
{
    _object_stream.clear();
    for(const tokenizer::token& token: tokens) {
        if (tokenizer::token_type::left_bracket == token.type) {
            _list_stack.push(utility::list_view(_machine, nullptr));
        } else if (tokenizer::token_type::right_bracket == token.type) {
            if (_list_stack.empty()) {
                throw error::error().format("unexpected token \'", token.content, "\'");
            }
            _complete_list();
        } else if (tokenizer::token_type::apostrophe == token.type) {
            _list_stack.push(utility::list_view(_machine, nullptr));
            _list_stack.top().push_back(_machine.alloc<lisp_abi::symbol>("quote"));
            _quote_stack.push(_list_stack.top().front_pair());
        } else {
            lisp_abi::object* new_item = nullptr;
            
            assert(!token.content.empty() || token.type == tokenizer::token_type::string);
            
            if (tokenizer::token_type::string == token.type) {
                new_item = _machine.alloc<lisp_abi::string>(token.content);
            } else if (token.content.front() == '#') {
                if (token.content == "#t") {
                    new_item = _machine.alloc<lisp_abi::boolean>(true);
                } else if (token.content == "#f") {
                    new_item = _machine.alloc<lisp_abi::boolean>(false);
                }
            }
            
            if (!new_item) {
                char* endptr = nullptr;
                assert(!token.content.empty());
                float number = std::strtof(token.content.c_str(), &endptr);
                if (endptr != token.content.c_str()) {
                    new_item = _machine.alloc<lisp_abi::number>(number);
                } else {
                    new_item = _machine.alloc<lisp_abi::symbol>(token.content);
                }
            }
            
            _complete_object(new_item);
        }
    }
    return _object_stream;
}

void parser::_complete_object(lisp_abi::object* obj) {
    if (_list_stack.empty()) {
        _object_stream.push_back(obj);
        return;
    } else {
        _list_stack.top().push_back(obj);
    }

    if (!_quote_stack.empty() && _quote_stack.top() == _list_stack.top().front_pair()) {
        _quote_stack.pop();
        _complete_list();
    }
}

void parser::_complete_list() {
    lisp_abi::pair* obj = _list_stack.top().front_pair();
    _list_stack.pop();
    _complete_object(obj);
}

}
