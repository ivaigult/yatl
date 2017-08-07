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

#include <utility>
#include <tuple>

namespace yatl {
namespace apply_workaround {
template<size_t ...>
struct sequence { };

template<size_t index, size_t ...s>
struct gen_index_sequence : gen_index_sequence<index - 1, index - 1, s...> { };

template<size_t ...s>
struct gen_index_sequence<0, s...> {
    typedef sequence<s...> type;
};

template<typename function_t, typename args_tuple_t>
struct apply_helper;

template<typename function_t, typename... args_t>
struct apply_helper<function_t, ::std::tuple<args_t...> > {
    typedef typename gen_index_sequence<sizeof...(args_t)>::type sequence_type;
    typedef decltype(::std::declval<function_t>()(::std::declval<args_t>()...)) result_type;

    template<size_t ...s>
    result_type do_apply(sequence<s...>, function_t func, ::std::tuple<args_t...>& args) {
        return func(::std::forward<args_t>(::std::get<s>(args))...);
    }
};

template<typename class_t, typename result_t, typename... args_t, typename... tuple_args_t>
struct apply_helper<result_t(class_t::*)(args_t...), ::std::tuple<tuple_args_t...> >
{
    typedef typename gen_index_sequence<sizeof...(tuple_args_t)>::type sequence_type;
    typedef result_t result_type;
    typedef result_t(class_t::*function_t)(args_t...);

    template<typename other_class_t, typename... other_args_t>
    result_t call_member_fn(function_t function, other_class_t& obj, other_args_t&&... args)
    {
        return (obj.*function)(::std::forward<other_args_t>(args)...);
    }

    template<size_t... s>
    result_t do_apply(sequence<s...>, function_t func, ::std::tuple<tuple_args_t...>& args)
    {
        return call_member_fn(func, ::std::forward<tuple_args_t>(::std::get<s>(args))...);
    }
};

namespace std {
template<typename function_t, typename args_tuple_t>
typename apply_helper<function_t, args_tuple_t>::result_type apply(function_t func, args_tuple_t& args) {

    typedef apply_helper<function_t, args_tuple_t> apply_helper_t;
    typedef typename apply_helper_t::sequence_type sequence_t;
    return apply_helper_t{}.do_apply(sequence_t{}, func, args);
}
}
}
}
