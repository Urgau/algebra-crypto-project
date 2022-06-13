/*
** EPITECH PROJECT, 2020
** common/include/events/SelfEvent
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#include "events/SelfEvent.hpp"

namespace babel {
    namespace events {
        SelfEvent::SelfEvent() : AMessage("SelfEvent")
        { /* ignore */ }

        SelfEvent::SelfEvent(babel::User::Id userId,babel::User::Name username,babel::User::Avatar avatar,babel::User::Visibility visibility) :
            AMessage("SelfEvent"), _userId(userId), _username(username), _avatar(avatar), _visibility(visibility)
        { /* ignore */ }

        babel::User::Id SelfEvent::getUserId() const
        {
            return (this->_userId);
        }

        babel::User::Name SelfEvent::getUsername() const
        {
            return (this->_username);
        }

        babel::User::Avatar SelfEvent::getAvatar() const
        {
            return (this->_avatar);
        }

        babel::User::Visibility SelfEvent::getVisibility() const
        {
            return (this->_visibility);
        }
    }
}
