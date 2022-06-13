/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserManager
*/

#include "Log.hpp"
#include "Error.hpp"
#include "UserManager.hpp"
#include "UserItem.hpp"

babel::UserManager::UserManager(QObject *parent)
    : QObject(parent), _selfObject(nullptr)
{ /* ignore */ }

babel::UserManager::~UserManager()
{ /* ignore */ }

void babel::UserManager::slotUserEvent(const events::UserEvent &e)
{
    User user = User(e.getUserId(), e.getUsername(),
            e.getAvatar(), e.getVisibility());

    if (_users.count(user.getId())) {
        _users[user.getId()]->updateUser(user, e.getStatus());
    } else {
        _users[user.getId()] = new UserObject(this, user, e.getStatus());
        emit signalUserCreated(_users[user.getId()]);
    }
}

void babel::UserManager::slotSelfEvent(const events::SelfEvent &e)
{
    // TODO: Remove selfObject from _users (cf. addMessage)
    User user = User(e.getUserId(), e.getUsername(),
            e.getAvatar(), e.getVisibility());

    if (_users.count(user.getId())) {
        _users[user.getId()]->updateUser(user, User::Status::Online);
    } else {
        _users[user.getId()] = new UserObject(this, user, User::Status::Online);

        emit signalSelfAvatarUpdated(_users[user.getId()]->getAvatarPixmap());
        QObject::connect(this->_users[user.getId()], &babel::UserObject::avatarChanged,
                this, &babel::UserManager::signalSelfAvatarUpdated);
    }

    _selfObject = _users[e.getUserId()];
    emit signalSelfUpdated(_selfObject->getUser());
}

const babel::User &babel::UserManager::getSelf(void) const
{
    if (_selfObject == nullptr)
        throw Error("No self user!");
    return (_selfObject->getUser());
}

babel::UserObject::CallStatus babel::UserManager::getUserCallStatus(User::Id id) const
{
    try {
        return (_users.at(id)->getCallStatus());
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::getUserCallStatus");
    }
}

void babel::UserManager::setUserCallStatus(User::Id id, UserObject::CallStatus status)
{
    try {
        _users.at(id)->setCallStatus(status);
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::setUserCallStatus");
    }
}

const babel::net::Ip &babel::UserManager::getUserAudioIp(User::Id id) const
{
    try {
        return (_users.at(id)->getAudioIp());
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::getUserAudioIp");
    }
}

const babel::net::Port &babel::UserManager::getUserAudioPort(User::Id id) const
{
    try {
        return (_users.at(id)->getAudioPort());
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::getUserAudioPort");
    }
}

const std::string &babel::UserManager::getUserAudioPassword(User::Id id) const
{
    try {
        return (_users.at(id)->getAudioPassword());
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::getUserAudioPassword");
    }
}

void babel::UserManager::setUserAudioNetwork(User::Id id,
                    const net::Ip &ip, const net::Port &port,
                    const std::string &password)
{
    try {
        _users.at(id)->setAudioIp(ip);
        _users.at(id)->setAudioPort(port);
        _users.at(id)->setAudioPassword(password);
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::setUserIp");
    }
}

void babel::UserManager::addMessage(User::Id into, User::Id from,
                    const Message &message)
{
    try {
        MessageObject *messageObject = new MessageObject(this,
                this->_users.at(from), message);
        _users.at(into)->addMessage(messageObject);
    } catch (std::out_of_range &) {
        throw Error("User don't exist", "UserManager::setUserIp");
    }
}
