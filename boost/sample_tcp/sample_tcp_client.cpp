#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

/*
 * build command: g++ sample_tcp_client.cpp -o sample_tcp_client -lboost_system -lpthread
 */

using boost::asio::ip::tcp;

class client
{
public:
    client(boost::asio::io_service& io_service)
        : io_service_(io_service), socket_(io_service)
    {
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", "31400");
        tcp::resolver::iterator iterator = resolver.resolve(query);
        boost::asio::connect(socket_, iterator);
    }

    void start() {
        // send first message
        boost::system::error_code ec;
        const std::string msg = "check\n";
        boost::asio::write(socket_, boost::asio::buffer(msg), ec);
        if (ec) {
            std::cout << "write error code: " << ec.message() << std::endl;
        }
        start_read();
    }

    void start_read() {
        boost::asio::async_read_until(socket_,
                                      receive_buffer_,
                                      "\n",
                                      [&]
                                      (const boost::system::error_code& ec,
                                       std::size_t s) {
                                          handle_read(ec, s);
                                      });
    }

    void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred) {
        if (ec && ec != boost::asio::error::eof) {
            std::cout << "receive failed: " << ec.message() << std::endl;
        }
        else {
            const char* data = boost::asio::buffer_cast<const char*>(receive_buffer_.data());
            std::cout << "receive data: " << data;
            receive_buffer_.consume(receive_buffer_.size());
            start_read();
        }
    }

public:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    boost::asio::streambuf receive_buffer_;
};

int main()
{
    boost::asio::io_service io_service;
    client c(io_service);
    c.start();
    io_service.run();

    return 0;
}
