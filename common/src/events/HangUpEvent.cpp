/*
** EPITECH PROJECT, 2020
** common/include/events/HangUpEvent
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#include "events/HangUpEvent.hpp"

namespace babel {
    namespace events {
        HangUpEvent::HangUpEvent() : AMessage("HangUpEvent")
        { /* ignore */ }

        HangUpEvent::HangUpEvent(babel::User::Id fromUserId,babel::User::Id toUserId) :
            AMessage("HangUpEvent"), _fromUserId(fromUserId), _toUserId(toUserId)
        { /* ignore */ }

        babel::User::Id HangUpEvent::getFromUserId() const
        {
            return (this->_fromUserId);
        }

        babel::User::Id HangUpEvent::getToUserId() const
        {
            return (this->_toUserId);
        }
    }
}
