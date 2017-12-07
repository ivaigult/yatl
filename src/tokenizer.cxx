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

#include "tokenizer.hpp"
#include "error.hpp"

#include <algorithm>
#include <cctype>
#include <cassert>


namespace yatl {

void tokenizer::_emit_token(token_stream_t& token_stream, token& token) {
    if (token.content.empty())
        return;
    token_stream.push_back(token);
    token = {};
}

void tokenizer::add_char(token_stream_t& tokens, char sym) {
    if (_parsing_string) {
	if (sym == '\\' && !_escape) {
	    _escape = true;
	    return;
	} else if (sym == '\"' && !_escape) {
	    _parsing_string = false;
	    tokens.push_back(_current_token);
	    _current_token = {};
	    return;
	}
	_current_token.content += sym;
	_escape = false;
	return;
    }

    if (_parsing_comment) {
	if (sym == '\n') {
	    _parsing_comment = false;
	}
	return;
    }

    if (sym == '(') {
	_emit_token(tokens, _current_token);
	tokens.push_back({tokenizer::token_type::left_bracket, "(" });
	return;
    } else if (sym == ')') {
	_emit_token(tokens, _current_token);
	tokens.push_back({tokenizer::token_type::right_bracket, ")" });
	return;
    } else if (sym == '\'') {
	_emit_token(tokens, _current_token);
	tokens.push_back({ tokenizer::token_type::apostrophe, "\'" });
	return;
    } else if (sym == ';') {
	_emit_token(tokens, _current_token);
	_parsing_comment = true;
	return;
    } else if (sym == '\"') {
	_emit_token(tokens, _current_token);
	_parsing_string = true;
	_current_token.type = tokenizer::token_type::string;
	return;
    } else if (std::isspace(sym)) {
	_emit_token(tokens, _current_token);
	return;
    }

    _current_token.type = tokenizer::token_type::symbols;
    _current_token.content += sym;
}
    
const tokenizer::token_stream_t& tokenizer::tokenize(std::string line) {
    _token_stream.clear();
    // @todo: handle multiline tokens
    for (char sym : line) {
	add_char(_token_stream, sym);
    }
    add_char(_token_stream, '\n');
    return _token_stream;
}

}
