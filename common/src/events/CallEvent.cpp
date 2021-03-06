/*
** EPITECH PROJECT, 2020
** common/include/events/CallEvent
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#include "events/CallEvent.hpp"

namespace babel {
    namespace events {
        CallEvent::CallEvent() : AMessage("CallEvent")
        { /* ignore */ }

        CallEvent::CallEvent(babel::User::Id fromUserId,babel::User::Id toUserId,babel::net::Ip ip,babel::net::Port port,std::string password) :
            AMessage("CallEvent"), _fromUserId(fromUserId), _toUserId(toUserId), _ip(ip), _port(port), _password(password)
        { /* ignore */ }

        babel::User::Id CallEvent::getFromUserId() const
        {
            return (this->_fromUserId);
        }

        babel::User::Id CallEvent::getToUserId() const
        {
            return (this->_toUserId);
        }

        babel::net::Ip CallEvent::getIp() const
        {
            return (this->_ip);
        }

        babel::net::Port CallEvent::getPort() const
        {
            return (this->_port);
        }

        std::string CallEvent::getPassword() const
        {
            return (this->_password);
        }
    }
}
