﻿#include "UDPServer.hpp"

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
