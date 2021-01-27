//
// blocking_udp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include "UDPClient.hpp"

int main()
{
    try
    {
        std::shared_ptr< boost::asio::io_context > io_context = std::make_shared< boost::asio::io_context >();

        UDPClient client(io_context, 8091);

        std::thread th([&]() {

            io_context->run();
            });
        while (1)
        {
            std::cout << "Enter message: ";
            char request[max_length];
            std::cin.getline(request, max_length);

            client.send("192.168.0.10", "8090", request);

        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
