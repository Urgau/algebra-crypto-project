/*
** EPITECH PROJECT, 2020
** server/src/Session
** File description:
** echo $?
*/

#include <iostream>
#include "Log.hpp"
#include "Error.hpp"
#include "Session.hpp"
#include "Instance.hpp"

namespace babel {

    Session::Session(Instance &instance, asio::io_context &io_context, asio::ssl::context &ssl_context) :
        _instance(instance), _stream(io_context, ssl_context)
    { /* ignore */ }

    void Session::operator ()()
    {
        _instance.join(this->shared_from_this());
        _stream.async_handshake(boost::asio::ssl::stream_base::server,
                [this](const boost::system::error_code &ec) {
                    this->handleHandshake(ec);
                });
    }

    User &Session::getUser() const
    {
        if (!this->_user.has_value())
            throw Error("This session doesn't containt any User");
        return (this->_user.value());
    }

    void Session::setUser(User &user)
    {
        this->_user = user;
    }

    bool Session::hasUser() const
    {
        return (this->_user.has_value());
    }

    void Session::clearUser()
    {
        this->_user.reset();
    }

    Room &Session::getRoom() const
    {
        if (!this->_room.has_value())
            throw Error("This session doesn't containt any Room");
        return (this->_room.value());
    }

    void Session::setRoom(Room &user)
    {
        this->_room = user;
    }

    bool Session::hasRoom() const
    {
        return (this->_room.has_value());
    }

    void Session::clearRoom()
    {
        this->_room.reset();
    }

    asio::ssl::stream<asio::ip::tcp::socket>::lowest_layer_type &Session::socket()
    {
        return (this->_stream.lowest_layer());
    }

    babel::net::Ip Session::getRemoteIp() const
    {
        return (this->_stream.lowest_layer().remote_endpoint().address().to_string());
    }

    void Session::deliver(const std::string &content)
    {
        bool writeInProgress = !this->_writeQueue.empty();

        this->_writeQueue.push_back(content);
        if (!writeInProgress)
            doWriteAsync();
    }

    void Session::handleHandshake(const boost::system::error_code& ec)
    {
        if (!ec){
            doReadAsync();
        } else {
            WARN("Handshake: ", ec.message());
            // delete this;
        }
    }

    void Session::doReadAsync()
    {
        auto self(shared_from_this());
        asio::async_read_until(this->_stream, this->_streambuf, '\n',
                [this, self](const std::error_code &ec, std::size_t size)
                {
                    if (!ec) {
                        {
                            // Setup input stream from the stream buffer
                            std::istream is(&this->_streambuf);

                            // Pass to the Instance the command
                            this->_instance.handleCommand(self, is);

                            // Discard everthing until new-line
                            is.ignore(size, '\n');
                        }

                        doReadAsync();
                    } else {
                        this->_instance.leave(self);
                    }
                });
    }

    void Session::doWriteAsync()
    {
        auto self(shared_from_this());
        asio::async_write(this->_stream, asio::buffer(this->_writeQueue.front()),
                [this, self](const std::error_code &ec, std::size_t)
                {
                    if (!ec) {
                        this->_writeQueue.pop_front();
                        if (!this->_writeQueue.empty())
                            doWriteAsync();
                    } else {
                        this->_instance.leave(self);
                    }
                });
    }
}
