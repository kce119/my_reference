#ifndef INCLUDE_GUARD_SESSION_IMPL_HPP
#define INCLUDE_GUARD_SESSION_IMPL_HPP

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

//#include "type_erasure.hpp"
#include "session.hpp"

void session::handle_read(const boost::system::error_code& error,
                          std::size_t bytes_transferred) {
    if (!error) {
        std::string result;
        std::string tmp = boost::asio::buffer_cast<const char *>(receive_buffer_.data());
        result += tmp.substr(0, receive_buffer_.size());
        std::cout << "result: " << result;
        if (result == "check\n") {
            core_.handle_req_list_num(shared_from_this());
            receive_buffer_.consume(receive_buffer_.size());
            start_read();
        }
        else {
            // TODO send message all(chat function)
        }
    }
    else {
        if (error == boost::asio::error::eof) {
        }
        else {
            std::cout << "handle_read error!" << error.message() << std::endl;
        }
        //session delete
        core_.handle_delete_session(shared_from_this());
    }
}

#endif // INCLUDE_GUARD_SESSION_IMPL_HPP
