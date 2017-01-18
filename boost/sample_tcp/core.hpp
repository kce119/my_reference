#ifndef INCLUDE_GUARD_CORE_HPP
#define INCLUDE_GUARD_CORE_HPP

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <set>
#include "type_erasure.hpp"

class core
{
public:
    void set_list(ns::abstract_type const& s_sp) {
        sp_set_.insert(s_sp);
    }

    void handle_delete_session(ns::abstract_type const& s_sp) {
        //session search
        for(auto itr = sp_set_.begin(); itr != sp_set_.end(); ++itr) {
            if (itr->get_address() == s_sp.get_address()) { // itr->get() = &*(*itr)
                sp_set_.erase(itr);
                return;
            }
        }
        std::cout << "[ERROR] not exsit session." << std::endl;
    }

    void handle_req_list_num(ns::abstract_type const& s_sp) {
        //session search
        for(auto itr = sp_set_.begin(); itr != sp_set_.end(); ++itr) {
            if (itr->get_address() == s_sp.get_address()) { // itr->get() = &*(*itr)
                itr->send_list_num(sp_set_.size());
                return;
            }
        }
        std::cout << "[ERROR] not exsit session." << std::endl;
    }

private:
    std::set<ns::abstract_type> sp_set_;
};

#endif // INCLUDE_GUARD_CORE_HPP
