# yatl: yet another tiny lisp
[![Travis Status](https://travis-ci.org/ivaigult/yatl.svg?branch=master)](https://travis-ci.org/ivaigult/yatl)
[![Appveyor Status](https://ci.appveyor.com/api/projects/status/github/ivaigult/yatl?svg=true)](https://ci.appveyor.com/project/ivaigult/yatl/branch/master)

_yatl_ is _Scheme_ like lisp interpreter with a user friendly FFI. Any given c++ functor can be bound to the language by one line of code. Consider `car` implementation example:
```c++
utility::bind_function(m, "car",  [&m](lisp_abi::pair&   l) { return l.value.head; });
```

This single line checks that the first parameter of function is non `nil` list, returs its head and binds this function to the interpreter. Variadic functions are supported aswell:
```c++
utility::bind_function(m, "+", [&m](utility::rest_arguments<std::vector<std::reference_wrapper<lisp_abi::number> > > numbers) {
    lisp_abi::number* result = m.alloc<lisp_abi::number>(0.0f);
    std::for_each(numbers.args.begin(), numbers.args.end(), [&result](const lisp_abi::number& n) { result->value += n.value; });
    return result;
});
```
Those lines check if argument list contains non nil numbers and does accumulation using standart libstdcxx functions.

The project is under heavy development, almost nothing is supported. For more details see: `STATUS.md`.

## Build
```
mkdir _build
cd _build
cmake ..
cmake --build .
```
