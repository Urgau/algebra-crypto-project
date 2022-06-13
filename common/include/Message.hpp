/*
** EPITECH PROJECT, 2020
** common/include/Message
** File description:
** programmers start to count from 0
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <ctime>
#include <string>

namespace babel {
    class Message {
        public:
            /// \brief Type alias for describing the Timestamp
            using Timestamp = std::time_t;

            /// \brief Type alias for the body
            using Body = std::string;

            /// \brief Constructor
            Message(const Message::Timestamp &timestamp,
                    const Message::Body &body);

            /// \brief Default constructor
            ~Message() = default;

            /// \brief Get the timestamp
            const Message::Timestamp &getTimestamp() const;

            /// \brief Get the body
            const Message::Body &getBody() const;
        private:
            Message::Timestamp _timestamp;
            Message::Body _body;
    };
}

#endif
