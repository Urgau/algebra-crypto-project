/*
** EPITECH PROJECT, 2020
** common/include/commands/MessageCommand
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#ifndef MESSAGECOMMAND_HPP_
#define MESSAGECOMMAND_HPP_

#include "AMessage.hpp"
#include "User.hpp"
#include "Message.hpp"

namespace babel {
    namespace commands {
        /// \brief MessageCommand
        class MessageCommand : public AMessage {
            public:
                /// \brief Dummy constructor (for serialize)
                MessageCommand();

                /// \brief Constructor of MessageCommand class
                MessageCommand(babel::User::Id userId,babel::Message::Body body);

                /// \brief Deconstructor of MessageCommand
                ~MessageCommand() = default;

                /// \brief Getter for UserId
                babel::User::Id getUserId() const;

                /// \brief Getter for Body
                babel::Message::Body getBody() const;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int)
                {
                    ar & _userId;
                    ar & _body;
                }

            private:
                babel::User::Id _userId;
                babel::Message::Body _body;
        };
    }
}

#endif