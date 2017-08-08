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

namespace yatl {
namespace utility {
namespace detail {
template<typename tuple_t>
struct tuple_cdr;

template<typename head_t, typename... tail_t>
struct tuple_cdr<std::tuple<head_t, tail_t...> > {
    typedef std::tuple<tail_t...> type;
};
}

template<typename functor_t>
struct functor_traits;

template<typename result_t, typename... args_t>
struct functor_traits<result_t(args_t...)> {
    typedef std::tuple<args_t...> args_type;
    typedef result_t              result_type;
};

template<typename result_t, typename... args_t>
struct functor_traits<result_t(*)(args_t...)> : public functor_traits<result_t(args_t...)> {
    typedef result_t(*callable_type)(args_t...);
};

template<typename class_t, typename result_t, typename... args_t>
struct functor_traits<result_t(class_t::*)(args_t...)> : public functor_traits<result_t(class_t&, args_t...)>
{
    typedef result_t(class_t::*callable_type)(args_t...);
};

template<typename class_t, typename result_t, typename... args_t>
struct functor_traits<result_t(class_t::*)(args_t...) const> : public functor_traits<result_t(class_t&, args_t...)>
{
    typedef result_t(class_t::*callable_type)(args_t...) const;
};

template<typename functor_t>
struct functor_traits {
private:
    typedef functor_traits<decltype(&functor_t::operator())> opertor_traits;
public:
    typedef functor_t callable_type;
    typedef typename detail::tuple_cdr<typename opertor_traits::args_type>::type args_type;
};

}
}
