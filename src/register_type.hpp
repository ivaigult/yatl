#include "object.hpp"

#include <cassert>

namespace yatl
{

class machine;

template<typename implementation_t, typename... interfaces_t>
struct user_data_type_impl : public implementation_t, public lisp_abi::user_data_type
{
public:
    virtual bool query_interface(uint32_t type_id, void** obj) {
        return qi_impl<interfaces_t>(type_id, obj);
    }
private:
    template<typename first_t>
    bool qi_impl(uint32_t type_id, void** obj) {
        if (first_t::type_id == type_id) {
            *obj = this;
            return true;
        }
        return false;
    }

    template<typename first_t, typename second_t, typename... tail_t>
    bool qi_impl(uint32_t type_id, void** obj) {
        if (first_t::type_id == type_id) {
            *obj = this;
            return true;
        } else {
            return qi_impl<second_t, tail_t...>(type_id, obj);
        }
    }
};

template<typename user_type_t>
void register_type(machine& m) {
    assert(!user_type_t::type_id && "Type is already registred!");
    user_type_t::type_id = m.alloc_type_id();
}

}