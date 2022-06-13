/*
** EPITECH PROJECT, 2020
** server/include/Proxy
** File description:
** r/ProgrammerHumor
*/

#ifndef PROXY_HPP_
#define PROXY_HPP_

#include "asio.hpp"
#include <set>

namespace babel {

    /// \brief Proxy-echo class
    class Proxy {
        public:
            /// \brief Proxy constructor (give context and endpoint)
            Proxy(asio::io_context &context,
                    const asio::ip::udp::endpoint &endpoint);

            /// \brief Proxy deconstructor
            ~Proxy() = default;

            /// \brief Proxy endpoint
            asio::ip::udp::endpoint endpoint();

        private:
            /// \brief Start receiving
            void doAsyncReceive();

        private:
            asio::ip::udp::socket _socket;
            std::array<char, 1024> _buffer;
            asio::ip::udp::endpoint _endpoint;
            asio::ip::udp::endpoint _rendpoint;
            std::set<asio::ip::udp::endpoint> _endpoints;
    };

    /// \brief Type alias
    typedef std::shared_ptr<Proxy> ProxyPtr;
}

#endif
