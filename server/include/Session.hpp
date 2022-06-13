/*
** EPITECH PROJECT, 2020
** server/include/Session
** File description:
** epitech > epita
*/

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <deque>
#include <memory>
#include <boost/optional.hpp>
#include "asio.hpp"
#include "User.hpp"
#include "Net.hpp"

namespace babel {

    /// Declaration forward
    class Instance;

    /// Declaration forward
    class Room;

    /// \brief Session class
    class Session : public std::enable_shared_from_this<Session>
    {
        public:
            /// \brief Class constructor (need a tcp socket)
            Session(Instance &, asio::io_context &, asio::ssl::context &);

            /// \brief Destructor
            ~Session() = default;

            /// \brief Executor
            void operator ()();

            asio::ssl::stream<asio::ip::tcp::socket>::lowest_layer_type &socket();

            /// \brief Deliver from streambuf
            void deliver(const std::string &content);

            /// \brief Get user
            User &getUser() const;

            /// \brief Get user
            Room &getRoom() const;

            /// \brief Set user
            void setUser(User &user);

            /// \brief Set room
            void setRoom(Room &room);

            /// \brief Clear the user field
            void clearUser();

            /// \brief Clear the room field
            void clearRoom();

            /// \brief Tell if there is a User
            bool hasUser() const;

            /// \brief Tell if there is a Room
            bool hasRoom() const;

            /// \brief Get the remote ip
            babel::net::Ip getRemoteIp() const;

        private:
            /// \brief Start a async read
            void doReadAsync();

            /// \brief Start a async write
            void doWriteAsync();

            /// \brief Handle the SSL Handshake
            void handleHandshake(const boost::system::error_code& error);

            Instance &_instance;
            asio::streambuf _streambuf;
            boost::optional<User&> _user;
            boost::optional<Room&> _room;
            std::deque<std::string> _writeQueue;
            asio::ssl::stream<asio::ip::tcp::socket> _stream;
    };

    /// \brief Type alias
    typedef std::shared_ptr<Session> SessionPtr;
}

#endif
