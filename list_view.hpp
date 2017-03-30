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

#include "lisp_abi.hpp"
#include "machine.hpp"

namespace yatl {
namespace utility {

class list_view {
public:
    typedef lisp_abi::object* value_type;
    typedef lisp_abi::pair*   pair_type;

    list_view(machine& m, lisp_abi::pair* front = nullptr, lisp_abi::pair* back = nullptr)
        : _m(m)
        , _front(front)
        , _back(back)
    { _adjust_back(); }

    list_view(const list_view& lv) 
        : _m(lv._m)
        , _front(lv._front)
        , _back(lv._back)
	{}

    value_type front()                  { return _front->value.head; }
    value_type back()                   { return _back->value.head;  }
    pair_type  front_pair()             { return _front;             }
    pair_type  back_pair()              { return _back;              }
    const value_type front()      const { return _front->value.head; }
    const value_type back()       const { return _back->value.head;  }
    const pair_type  front_pair() const { return _front;             }
    const pair_type  back_pair()  const { return _back;              }

    void push_front(value_type o) { _front = _m.alloc<lisp_abi::pair>((lisp_abi::object*)o, (lisp_abi::object*)_front); }
    void push_back(value_type  o) { 
        lisp_abi::pair* new_item = _m.alloc<lisp_abi::pair>((lisp_abi::object*)o, (lisp_abi::object*)nullptr);

		if (!_front) {
            _front = _back = new_item;
        } else {
            _back->value.tail = new_item;
            _back = new_item;
        }
	}

    template<typename object_t, typename... args_t>
    void emplace_front(args_t&&... args) { push_front(m.alloc<object_t>(std::forward<args_t>(args)...)) }

    template<typename object_t, typename... args_t>
    void emplace_back(args_t&&... args) { push_back(m.alloc<object_t>(std::forward<args_t>(args)...)) }

    struct iterator {
        iterator(lisp_abi::pair* node) : _node(node)       {}
        iterator&    operator=(const iterator& it)         { _node = it._node; return *this; }
        bool         operator==(const iterator& it) const  { return _node == it._node; }
        bool         operator!=(const iterator& it) const  { return _node != it._node; }
        iterator&    operator++()                          { _node = static_cast<lisp_abi::pair*>(_node->value.tail); return *this; }
        value_type   operator*() const                     { return _node->value.head; }
    private:
        lisp_abi::pair* _node;
    };

    typedef const iterator const_iterator;

    iterator begin()             { return _front;  }
    iterator end()               { return nullptr; }
    const_iterator begin() const { return _front; }
    const_iterator end()   const { return nullptr; }

private:
	
    void _adjust_back() {
        if (!_back)
            _back = _front;
        if (!_back)
            return;
        for (; _back->value.tail; ) {
            _back = lisp_abi::object_cast<lisp_abi::pair>(_back->value.tail);
        }
    }
    
    lisp_abi::pair* _front;
    lisp_abi::pair* _back;
    machine& _m;
};

}
}