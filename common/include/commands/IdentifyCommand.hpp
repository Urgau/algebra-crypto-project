/*
** EPITECH PROJECT, 2020
** common/include/commands/IdentifyCommand
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#ifndef IDENTIFYCOMMAND_HPP_
#define IDENTIFYCOMMAND_HPP_

#include "AMessage.hpp"
#include "User.hpp"

namespace babel {
    namespace commands {
        /// \brief IdentifyCommand
        class IdentifyCommand : public AMessage {
            public:
                /// \brief Dummy constructor (for serialize)
                IdentifyCommand();

                /// \brief Constructor of IdentifyCommand class
                IdentifyCommand(babel::User::Id userId);

                /// \brief Deconstructor of IdentifyCommand
                ~IdentifyCommand() = default;

                /// \brief Getter for UserId
                babel::User::Id getUserId() const;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int)
                {
                    ar & _userId;
                }

            private:
                babel::User::Id _userId;
        };
    }
}

#endif