
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <thread>

using boost::asio::ip::udp;
using boost::asio::ip::address;

class UDPServer
{
public:
    UDPServer(boost::asio::io_context& io_context, unsigned short port)
        : _socket(io_context, udp::endpoint(udp::v4(), port))
    {
        startReceive();
    }

private:
    void startReceive()
    {
        _socket.async_receive_from(boost::asio::buffer(_recvBuffer), _remoteEndpoint,
            boost::bind(&UDPServer::handleReceive, this,
                boost::placeholders::_1,
                boost::placeholders::_2));
    }

    void handleReceive(const boost::system::error_code& error,
        std::size_t bytes_transferred)
    {
        if (!error || error == boost::asio::error::message_size)
        {
            std::cout << _recvBuffer.data() << "\n";
            auto message = std::make_shared<std::string>("Hello , World\n");

            _socket.async_send_to(boost::asio::buffer(*message), _remoteEndpoint,
                boost::bind(&UDPServer::handleSend, this, message,
                    boost::placeholders::_1,
                    boost::placeholders::_2));
        }
    }

    void handleSend(std::shared_ptr<std::string> message,
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        startReceive();
    }

    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    std::array<char, 1024> _recvBuffer;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;

        UDPServer server(io_context, 8090);
        io_context.run();

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
