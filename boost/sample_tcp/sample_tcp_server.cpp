#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

//#include "type_erasure.hpp"
#include "core.hpp"
#include "session.hpp"
#include "session_impl.hpp"

/*
 * build command: g++ sample_tcp_server.cpp -o sample_tcp_server -lboost_system
 */

using boost::asio::ip::tcp;

class server
{
public:
    server(core& c, boost::asio::io_service& io_service)
        :
        core_(c),
        io_service_(io_service),
        acceptor_(io_service,
                  tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 31400)) {
        std::shared_ptr<session> session_ptr = std::make_shared<session>(core_, io_service_);
        acceptor_.async_accept(session_ptr->socket(),
                               [&, session_ptr]
                               (const boost::system::error_code& e) {
                                   handle_accept(session_ptr, e);
                               });
    }

    void handle_accept(std::shared_ptr<session> const& session_ptr,
                       boost::system::error_code const& error) {
        if (!error) {
            core_.set_list(session_ptr);
            session_ptr->start();
            std::shared_ptr<session> session_ptr2 = std::make_shared<session>(core_, io_service_);
            acceptor_.async_accept(session_ptr2->socket(),
                                   [&, session_ptr2]
                                   (const boost::system::error_code& e) {
                                       handle_accept(session_ptr2, e);
                                   });
        }
        else {
            std::cout << "accept error!" << std::endl;
        }
    }

private:
    core& core_;
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;

};

int main() {
    boost::asio::io_service io_service;
    core c;
    server s(c, io_service);

    io_service.run();

    return 0;
}
