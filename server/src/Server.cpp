/*
** EPITECH PROJECT, 2020
** server/src/Server
** File description:
** C-x C-c
*/

#include <iostream>
#include "Server.hpp"
#include "Log.hpp"

namespace babel {

    Server::Server(babel::Instance &instance, asio::io_context &io_context,
            const asio::ip::tcp::endpoint &endpoint,
            const std::string &tls_key, const std::string &tls_cert) :
        _instance(instance), _io_context(io_context),
        _ssl_context(asio::ssl::context::tlsv12),
        _acceptor(io_context, endpoint)
    {
        _ssl_context.set_options(asio::ssl::context::no_sslv2 |
                asio::ssl::context::single_dh_use);
        _ssl_context.use_certificate_chain_file(tls_cert);
        _ssl_context.use_private_key_file(tls_key, boost::asio::ssl::context::pem);

        doAsyncAccept();
    }

    void Server::doAsyncAccept()
    {
        auto session = std::make_shared<Session>(_instance, this->_io_context, this->_ssl_context);
        this->_acceptor.async_accept(session->socket(),
                [this, session](std::error_code ec)
                {
                    if (!ec) {
                        asio::ip::tcp::endpoint r_endpoint = session->socket().remote_endpoint();

                        INFO("Connection from ", r_endpoint, "...");
                        (*session)();
                    } else {
                        ERROR("Server accept: ", ec);
                    }
                    doAsyncAccept();
                });
    }
}
