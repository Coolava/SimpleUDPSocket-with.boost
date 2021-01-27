#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

class UDPClient
{
public:
    UDPClient(std::shared_ptr< boost::asio::io_context > io_context, unsigned short port)
        :_socket(*io_context, udp::endpoint(udp::v4(), port))
    {
        _ptr_io_context = io_context;

        _socket.async_receive(boost::asio::buffer(_recvBuffer),
            boost::bind(&UDPClient::receive, this,
                boost::placeholders::_1,
                boost::placeholders::_2));
    }

    void send(std::string address, std::string port, std::string message)
    {
        udp::resolver resolver(*_ptr_io_context);
        udp::resolver::results_type endpoints =
            resolver.resolve(udp::v4(), address, port);

        size_t request_length = message.length();
        _socket.send_to(boost::asio::buffer(message, request_length), *endpoints.begin());
    }

    void receive(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (!error || error == boost::asio::error::message_size)
        {
            std::cout << _recvBuffer.data() << "\n";

        }

        _socket.async_receive(boost::asio::buffer(_recvBuffer),
            boost::bind(&UDPClient::receive, this,
                boost::placeholders::_1,
                boost::placeholders::_2));
        /*char reply[max_length];
        udp::endpoint sender_endpoint;
        size_t reply_length = _socket.receive_from(
            boost::asio::buffer(reply, max_length), sender_endpoint);

        std::string received(reply, reply_length);*/

    }
private:
    udp::socket _socket;
    std::shared_ptr<boost::asio::io_context> _ptr_io_context;
    std::array<char, max_length> _recvBuffer;
    udp::endpoint _remoteEndpoint;
};