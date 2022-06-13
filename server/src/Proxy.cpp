/*
** EPITECH PROJECT, 2020
** server/src/Proxy
** File description:
** hello world?
*/

#include "Proxy.hpp"
#include "Log.hpp"

namespace babel {

    Proxy::Proxy(asio::io_context &context,
            const asio::ip::udp::endpoint &endpoint)
        : _socket(context, endpoint), _endpoint(endpoint)
    {
        doAsyncReceive();
    }

    void Proxy::doAsyncReceive()
    {
        this->_socket.async_receive_from(
                asio::buffer(this->_buffer), this->_rendpoint,
                [this](const std::error_code &ec, std::size_t size)
                {
                    if (!ec) {
                        for (auto &ep : this->_endpoints) {
                            if (ep == this->_rendpoint)
                                continue;
                            _socket.async_send_to(asio::buffer(this->_buffer, size), ep,
                                    [this, ep](const std::error_code &ec, std::size_t)
                                    {
                                        if (ec)
                                            this->_endpoints.erase(ep);
                                    });
                        }
                        this->_endpoints.insert(this->_rendpoint);
                        doAsyncReceive();
                    }
                });
    }

    asio::ip::udp::endpoint Proxy::endpoint()
    {
        return (this->_endpoint);
    }
}
