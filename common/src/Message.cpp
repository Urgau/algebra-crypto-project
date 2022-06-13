/*
** EPITECH PROJECT, 2020
** common/src/Message
** File description:
** echo $?
*/

#include "Message.hpp"

namespace babel {
    Message::Message(const Message::Timestamp &timestamp,
            const Message::Body &body)
        : _timestamp(timestamp), _body(body)
    { /* ignore */ }

    const Message::Timestamp &Message::getTimestamp() const
    {
        return (this->_timestamp);
    }

    const Message::Body &Message::getBody() const
    {
        return (this->_body);
    }
}
