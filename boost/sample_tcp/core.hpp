#ifndef INCLUDE_GUARD_CORE_HPP
#define INCLUDE_GUARD_CORE_HPP

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "type_erasure.hpp"

class core
{
public:
    void set_list(ns::abstract_type const& s_sp) {
        sp_v_.emplace_back(s_sp);
    }

    void handle_delete_session(ns::abstract_type const& s_sp) {
        //session search
        for(auto itr = sp_v_.begin(); itr != sp_v_.end(); ++itr) {
            if (itr->get_address() == s_sp.get_address()) { // itr->get() = &*(*itr)
                sp_v_.erase(itr);
                return;
            }
        }
        std::cout << "[ERROR] not exsit session." << std::endl;
    }

    void handle_req_list_num(ns::abstract_type const& s_sp) {
    //session search
    for(auto itr = sp_v_.begin(); itr != sp_v_.end(); ++itr) {
        if (itr->get_address() == s_sp.get_address()) { // itr->get() = &*(*itr)
            itr->send_list_num(sp_v_.size());
            return;
        }
    }
    std::cout << "[ERROR] not exsit session." << std::endl;
}

private:
    std::vector<ns::abstract_type> sp_v_;
};

#endif // INCLUDE_GUARD_CORE_HPP
