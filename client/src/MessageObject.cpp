/*
** EPITECH PROJECT, 2020
** client/src/MessageObject
** File description:
** man man
*/

#include "MessageObject.hpp"

babel::MessageObject::MessageObject(QObject *parent,
        const UserObject *userObject, const Message &message)
    : QObject(parent), _userObject(userObject), _message(message)
{ /* ignore */ }

const babel::UserObject *babel::MessageObject::getUserObject() const
{
    return (this->_userObject);
}

const babel::Message &babel::MessageObject::getMessage() const
{
    return (this->_message);
}
