/*
** EPITECH PROJECT, 2020
** common/include/commands/CallCommand
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#ifndef CALLCOMMAND_HPP_
#define CALLCOMMAND_HPP_

#include "AMessage.hpp"
#include "User.hpp"

namespace babel {
    namespace commands {
        /// \brief CallCommand
        class CallCommand : public AMessage {
            public:
                /// \brief Dummy constructor (for serialize)
                CallCommand();

                /// \brief Constructor of CallCommand class
                CallCommand(babel::User::Id userId);

                /// \brief Deconstructor of CallCommand
                ~CallCommand() = default;

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