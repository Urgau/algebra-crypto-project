/*
** EPITECH PROJECT, 2020
** common/include/events/UserEvent
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#include "events/UserEvent.hpp"

namespace babel {
    namespace events {
        UserEvent::UserEvent() : AMessage("UserEvent")
        { /* ignore */ }

        UserEvent::UserEvent(babel::User::Id userId,babel::User::Name username,babel::User::Avatar avatar,babel::User::Status status,babel::User::Visibility visibility) :
            AMessage("UserEvent"), _userId(userId), _username(username), _avatar(avatar), _status(status), _visibility(visibility)
        { /* ignore */ }

        babel::User::Id UserEvent::getUserId() const
        {
            return (this->_userId);
        }

        babel::User::Name UserEvent::getUsername() const
        {
            return (this->_username);
        }

        babel::User::Avatar UserEvent::getAvatar() const
        {
            return (this->_avatar);
        }

        babel::User::Status UserEvent::getStatus() const
        {
            return (this->_status);
        }

        babel::User::Visibility UserEvent::getVisibility() const
        {
            return (this->_visibility);
        }
    }
}
