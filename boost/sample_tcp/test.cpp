#define BOOST_TEST_MODULE My_Module

#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

#include "type_erasure.hpp"
#include "core.hpp"

/* compile command
 * g++ test.cpp -o test -lboost_system -lboost_unit_test_framework -static
 */

namespace as = boost::asio;

class test_session : public std::enable_shared_from_this<test_session> {
public:
    void send_list_num(std::size_t s) {
        std::string send_data = "connect client: " + std::to_string(s) + "\n";
        BOOST_TEST(send_data == "connect client: 1\n");
    }
};

BOOST_AUTO_TEST_CASE(test)
{
    // main
    core c;
    std::shared_ptr<test_session> session_ptr = std::make_shared<test_session>();
    c.set_list(session_ptr);
    c.handle_req_list_num(session_ptr);
}
