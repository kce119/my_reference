#ifndef INCLUDE_GUARD_SESSION_HPP
#define INCLUDE_GUARD_SESSION_HPP

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

#include "type_erasure.hpp"
#include "core.hpp"

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
    session(core& c, boost::asio::io_service& io_service)
        :
        core_(c),
        socket_(io_service)
    {
    }

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        //reaad
        start_read();
    }

    void start_read() {
        boost::asio::async_read_until(
            socket_,
            receive_buffer_,
            "\n",
            [&, self = shared_from_this()]
                                (const boost::system::error_code& e, std::size_t s) {
                                    handle_read(e, s);
                                });
    }

    void send_list_num(std::size_t s) {
        std::string send_data = "connect client: " + std::to_string(s) + "\n";
        std::cout << "send_data: " << send_data;
        boost::system::error_code ec;
        boost::asio::write(socket_, boost::asio::buffer(send_data), ec);
        if (ec) {
            std::cout << "write error code: " << ec.message() << std::endl;
        }
    }

    void handle_read(const boost::system::error_code& error,
                     std::size_t bytes_transferred);

private:
    core& core_;
    tcp::socket socket_;
    boost::asio::streambuf receive_buffer_;
    std::string send_data_;
};

#endif // INCLUDE_GUARD_SESSION_HPP
