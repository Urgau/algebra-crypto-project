/*
** EPITECH PROJECT, 2020
** common/include/events/HangUpEvent
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#ifndef HANGUPEVENT_HPP_
#define HANGUPEVENT_HPP_

#include "AMessage.hpp"
#include "User.hpp"
#include "User.hpp"

namespace babel {
    namespace events {
        /// \brief HangUpEvent
        class HangUpEvent : public AMessage {
            public:
                /// \brief Dummy constructor (for serialize)
                HangUpEvent();

                /// \brief Constructor of HangUpEvent class
                HangUpEvent(babel::User::Id fromUserId,babel::User::Id toUserId);

                /// \brief Deconstructor of HangUpEvent
                ~HangUpEvent() = default;

                /// \brief Getter for FromUserId
                babel::User::Id getFromUserId() const;

                /// \brief Getter for ToUserId
                babel::User::Id getToUserId() const;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int)
                {
                    ar & _fromUserId;
                    ar & _toUserId;
                }

            private:
                babel::User::Id _fromUserId;
                babel::User::Id _toUserId;
        };
    }
}

#endif
