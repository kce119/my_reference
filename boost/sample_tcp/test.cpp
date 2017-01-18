#include <boost/test/unit_test.hpp>

#include "sample_tcp_server.cpp"
#include "core.hpp"

namespace as = boost::asio;

BOOST_AUTO_TEST_SUITE(test)

class test_session : public std::enable_shared_from_this<test_session> {
public:
    session(core& c, boost::asio::io_service& io_service)
        :
        core_(c),
        socket_(io_service)
    {
    }
    void send_list_num(std::size_t s) {
        std::string send_data = "connect client: " + std::to_string(s) + "\n";
        BOOST_TEST(send_data == "");
    }
};

template <typename T>
class test_server {
public:
    server(core& c, boost::asio::io_service& io_service)
        :
        core_(c),
        io_service_(io_service) {
        std::shared_ptr<T> session_ptr = std::make_shared<T>(core_, io_service_);
    }
private:
    core& core_;
    boost::asio::io_service& io_service_;
};

BOOST_AUTO_TEST_CASE(test)
{
    // main
    boost::asio::io_service io_service;
    core c;
    test_server<test_session> s(c, io_service);
    io_service.run();

}

BOOST_AUTO_TEST_SUITE_END()
