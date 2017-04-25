#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>

#include <cctype>
#include <cassert>

namespace yatl 
{
class machine;
namespace lisp_abi
{

}

using namespace lisp_abi;

namespace builtins {

struct car : public lisp_abi::native_function_type {
    virtual lisp_abi::object* eval(machine &m, lisp_abi::object* object) {
        if (!object)
            return nullptr;
        if (object::object_type::list == object->type)
            return static_cast<list*>(object)->head;
        return nullptr;
    }
};
struct cdr : public lisp_abi::native_function_type {
    virtual lisp_abi::object* eval(machine &m, lisp_abi::object* object) {
        if (!object)
            return nullptr;
        if (object::object_type::list == object->type)
            return static_cast<list*>(object)->tail;
        return nullptr;
    }
};
struct cons : public lisp_abi::native_function_type {
    virtual lisp_abi::object* eval(machine &m, lisp_abi::object* object) {
        if (!object)
            return nullptr;
        lisp_abi::list* result = m.alloc<lisp_abi::list>();
        result->head = object
        return result;
    }
};

}
    

    
class repl;



    


}

int main()
{
    return yatl::repl(std::cin, std::cout).exec();
}
