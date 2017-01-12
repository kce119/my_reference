#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

/*
 * build command: g++ sample_tcp_server.cpp -o sample_tcp_server -lboost_system
 */

using boost::asio::ip::tcp;

class core;

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

class core
{
public:
    void set_list(std::shared_ptr<session> s_sp) {
        sp_v_.emplace_back(s_sp);
    }

    void handle_req_list_num(session* sp) {
        //session search
        for(auto itr = sp_v_.begin(); itr != sp_v_.end(); ++itr) {
            if (itr->get() == sp) { // itr->get() = &*(*itr)
                itr->get()->send_list_num(sp_v_.size());
                return;
            }
        }
        std::cout << "[ERROR] not exsit session." << std::endl;
    }

    void handle_delete_session(session* sp) {
        //session search
        for(auto itr = sp_v_.begin(); itr != sp_v_.end(); ++itr) {
            if (itr->get() == sp) { // itr->get() = &*(*itr)
                sp_v_.erase(itr);
                return;
            }
        }
        std::cout << "[ERROR] not exsit session." << std::endl;
    }

private:
    std::vector<std::shared_ptr<session>> sp_v_;
};

void session::handle_read(const boost::system::error_code& error,
                          std::size_t bytes_transferred) {
    if (!error) {
        std::string result;
        std::string tmp = boost::asio::buffer_cast<const char *>(receive_buffer_.data());
        result += tmp.substr(0, receive_buffer_.size());
        std::cout << "result: " << result;
        if (result == "check\n") {
            core_.handle_req_list_num(this);
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
        core_.handle_delete_session(this);
    }
}

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
