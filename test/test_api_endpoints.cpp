#include <gtest/gtest.h>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Function to send an HTTP GET request using Boost.Beast
std::string sendGetRequest(const std::string& host, const std::string& port, const std::string& target) {
    try {
        net::io_context ioc;
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        // Resolve the hostname
        auto const results = resolver.resolve(host, port);
        stream.connect(results);

        // Create the HTTP request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "Boost.Beast Client");

        // Send the request
        http::write(stream, req);

        // Receive response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        // Close the connection
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        if (ec && ec != beast::errc::not_connected) {
            throw beast::system_error{ec};
        }

        return res.body();
    } catch (const std::exception& e) {
        std::cerr << "Request failed: " << e.what() << std::endl;
        return "";
    }
}

// Google Test case
TEST(ApiTest, FiltersEndpoint) {
    std::string response = sendGetRequest("localhost", "8080", "/filters");

    // Check that the response is not empty
    EXPECT_FALSE(response.empty());

    // Check that the response contains expected content
    EXPECT_NE(response.find("expected_data"), std::string::npos);
}

// Main function to run the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
