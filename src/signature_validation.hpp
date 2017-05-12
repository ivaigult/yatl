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
#include "error.hpp"

#include <type_traits>
#include <functional>

namespace yatl {
namespace utility {

template<typename rest_t>
struct rest_arguments {
    rest_t args;
};

template<typename... args_t>
struct is_variadic_arglist;
template<typename head_t, typename... tail_t>
struct is_variadic_arglist<head_t, tail_t...> : public is_variadic_arglist<tail_t...> {};
template<typename rest_t>
struct is_variadic_arglist<rest_arguments<rest_t> > : public std::true_type {};
template<>
struct is_variadic_arglist<> : public std::false_type {};

namespace detail {

template<typename>
struct match_list;

template<typename value_t>
struct match_list<std::reference_wrapper<value_t> > : public match_list<value_t&> {};

template<typename value_t, lisp_abi::object::object_type type>
struct match_list<lisp_abi::custom_object<value_t, type>*>
{
    typedef lisp_abi::custom_object<value_t, type>  lisp_object_type;
    typedef lisp_object_type*                       result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator) const {
        lisp_abi::object* current_obj = *it++;
        if (!current_obj)
            return nullptr;
        // force type check
        return &lisp_abi::object_cast<lisp_object_type&>(*current_obj); 
    }
};

template<typename value_t, lisp_abi::object::object_type type>
struct match_list<lisp_abi::custom_object<value_t, type>&>
{
    typedef lisp_abi::custom_object<value_t, type>  lisp_object_type;
    typedef lisp_object_type&                       result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator) const {
        lisp_abi::object* current_obj = *it++;
        return lisp_abi::derefference_object_cast<result_type>(current_obj);
    }
};

template<>
struct match_list<lisp_abi::object*>
{
    typedef lisp_abi::object* result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator) const {
        return lisp_abi::object_cast<result_type>(*it++);
    }
};

template<>
struct match_list<lisp_abi::object&>
{
    typedef lisp_abi::object& result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator) const {
        return lisp_abi::derefference_object_cast<result_type>(*it++);
    }
};

template<typename... args_t>
struct list2tuple;

template<typename... args_t>
struct match_list<std::tuple<args_t...> > {
    typedef std::tuple<args_t...> result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator end) const {
        constant_list_view list_view(&lisp_abi::object_cast<lisp_abi::pair&>(**it++));
        // @todo: check tuple size
        detail::list2tuple<args_t...> list2tuple;
        return list2tuple.convert(list_view.begin(), list_view.end());
    }
};

// @todo: enalbe_if only if container_t is actualy libstdcxx like container (at least check for push_back method)
template<template<typename, typename> class container_t, template<typename> class alloc_t, typename element_t>
struct match_list<container_t<element_t, alloc_t<element_t> > > {
    typedef container_t<element_t, alloc_t<element_t> > result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator end) const {
        constant_list_view list_view(&lisp_abi::derefference_object_cast<lisp_abi::pair&>(*it++));
        result_type result;
        for (constant_list_view::iterator list_it = list_view.begin(); list_it != list_view.end(); ) {
            match_list<element_t> converter;
            result.push_back(converter(list_it, list_view.end()));
        }
        return std::move(result);
    }
};

// @todo: enalbe_if only if container_t is actualy libstdcxx like container (at least check for push_back method)
template<template<typename, typename> class container_t, template<typename> class alloc_t, typename element_t>
struct match_list<rest_arguments<container_t<element_t, alloc_t<element_t> > > >
{
    typedef container_t<element_t, alloc_t<element_t> > container_type;
    typedef rest_arguments<container_type> result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator end) const {
        result_type result;
        for (; it != end; ) {
            match_list<element_t> converter;
            result.args.push_back(converter(it, end));
        }
        return std::move(result);
    }
};

template<>
struct match_list<rest_arguments<lisp_abi::pair*> >
{
    typedef rest_arguments<lisp_abi::pair*> result_type;
    result_type operator()(constant_list_view::iterator& it, constant_list_view::iterator end) const {
        result_type result;
        result.args = it;
        return std::move(result);
    }
};

template<typename head_t, typename... tail_t>
struct list2tuple<head_t, tail_t...> {
    std::tuple<head_t, tail_t...> convert(constant_list_view::iterator begin, constant_list_view::iterator end) {
        const match_list<head_t> func = {};
        typename match_list<head_t>::result_type this_iteration = func(begin, end);
        list2tuple<tail_t...> next_iteration;
        return std::tuple_cat(std::forward_as_tuple(this_iteration), next_iteration.convert(begin, end));
    }
};

template<>
struct list2tuple<> {
    std::tuple<> convert(constant_list_view::iterator, constant_list_view::iterator) {
        return std::tuple<>();
    }
};


template<bool is_variadic, typename tuple_t>
struct validate_signature;

template<typename... args_t>
struct validate_signature<true, std::tuple<args_t...> > {
    std::tuple<args_t...> validate(lisp_abi::pair* list) {
        size_t args_required = sizeof...(args_t) - 1;
        constant_list_view list_view(list);
        // @todo: args_required could be zero, and list_view::size_type is unsigned
        if (args_required > list_view.size()) {
            throw error::error().format("too few arguments: ", args_required, " expected, ", list_view.size(), " provided");
        }
        detail::list2tuple<args_t...> list2tuple;
        return list2tuple.convert(list_view.begin(), list_view.end());
    }
};

template<typename... args_t>
struct validate_signature<false, std::tuple<args_t...> > {
    std::tuple<args_t...> validate(lisp_abi::pair* list) {
        size_t args_required = sizeof...(args_t);
        constant_list_view list_view(list);
        if (args_required < list_view.size()) {
            throw error::error().format("too many arguments: ", args_required, " expected, ", list_view.size(), " provided");
        } else if (args_required > list_view.size()) {
            throw error::error().format("too few arguments: ", args_required, " expected, ", list_view.size(), " provided");
        }

        detail::list2tuple<args_t...> list2tuple;
        return list2tuple.convert(list_view.begin(), list_view.end());
    }
};
}

template<typename tuple_t>
struct validate_signature;

template<typename... args_t>
struct validate_signature<std::tuple<args_t...> > : public detail::validate_signature<is_variadic_arglist<args_t...>::value, std::tuple<args_t...> > {};

}
}
