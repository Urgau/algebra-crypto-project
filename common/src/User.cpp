/*
** EPITECH PROJECT, 2020
** common/src/User
** File description:
** csfml is the best thing ever
*/

#include "User.hpp"
#include "events/SelfEvent.hpp"
#include "events/UserEvent.hpp"

namespace babel {
    User::User() : _id(USER_ID_NONE), _name(""),
        _avatar(""), _visibility(Visible)
    { /* ignore */ }

    User::User(User::Id id, User::Name name,
            User::Avatar avatar, User::Visibility visibility)
        : _id(id), _name(name), _avatar(avatar), _visibility(visibility)
    { /* ignore */ }

    User::Id User::getId() const
    {
        return (this->_id);
    }

    User::Name User::getName() const
    {
        return (this->_name);
    }

    User::Avatar User::getAvatar() const
    {
        return (this->_avatar);
    }

    User::Visibility User::getVisibility() const
    {
        return (this->_visibility);
    }

    void User::setName(const User::Name &name)
    {
        this->_name = name;
    }

    void User::setAvatar(const User::Avatar &avatar)
    {
        this->_avatar = avatar;
    }

    void User::setVisibility(const User::Visibility visibility)
    {
        this->_visibility = visibility;
    }

    bool User::operator ==(const User &other) const
    {
        return (this->_id == other.getId());
    }

    bool User::operator ==(const User::Id &id) const
    {
        return (this->_id == id);
    }

    const events::SelfEvent User::toSelfEvent() const
    {
        return (events::SelfEvent(this->_id, this->_name,
                    this->_avatar, this->_visibility));
    }

    const events::UserEvent User::toUserEvent(Status status) const
    {
        return (events::UserEvent(this->_id, this->_name,
                    this->_avatar, status, this->_visibility));
    }
}
