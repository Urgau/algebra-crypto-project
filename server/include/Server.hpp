/*
** EPITECH PROJECT, 2020
** server/include/server
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "asio.hpp"
#include "Instance.hpp"

namespace babel {

    /// \brief Server class
    class Server {
        public:
            /// \brief Server contructor
            Server(babel::Instance &instance,
                    asio::io_context &context,
                    const asio::ip::tcp::endpoint &endpoint,
                    const std::string &tls_key, const std::string &tls_cert);

            /// \brief Server destructor
            ~Server() = default;

        private:
            /// \brief Accept async method
            void doAsyncAccept();

        private:
            Instance &_instance;
            asio::io_context &_io_context;
            asio::ssl::context _ssl_context;

            asio::ip::tcp::acceptor _acceptor;
    };
}

#endif
