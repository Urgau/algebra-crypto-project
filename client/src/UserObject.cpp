/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserObject
*/

#include "UserObject.hpp"

babel::UserObject::UserObject(QObject *parent, const User &user,
        User::Status status, CallStatus callStatus)
    : QObject(parent), _user(user), _status(status), _callStatus(callStatus)
{
    this->_networkAccessManager = new QNetworkAccessManager(this);

    QObject::connect(this->_networkAccessManager, &QNetworkAccessManager::finished,
            this, &babel::UserObject::slotNetworkManagerFinished);

    this->loadAvatar();
}

void babel::UserObject::loadAvatar(void)
{
    if (this->_lastAvatar == this->_user.getAvatar())
        return;

    this->_lastAvatar = this->_user.getAvatar();
    if (this->_user.getAvatar() == "default") {
        this->_pixmap.load(QString(":Avatar.svg"));
        emit avatarChanged(this->_pixmap);
    } else {
        QUrl url(QString::fromStdString(this->_user.getAvatar()));

        if (url.isValid()) {
            QNetworkRequest request(url);
            this->_networkAccessManager->get(request);

            this->_pixmap.load(QString(":Loading.svg"));
            emit avatarChanged(this->_pixmap);
        } else {
            this->_pixmap.load(QString(":Avatar.svg"));
            emit avatarChanged(this->_pixmap);
        }
    }
}

void babel::UserObject::slotNetworkManagerFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        this->_pixmap.load(QString(":Avatar.svg"));
    } else {
        QByteArray avatarData = reply->readAll();

        if (!this->_pixmap.loadFromData(avatarData)) {
            this->_pixmap.load(QString(":Avatar.svg"));
        }
    }

    emit avatarChanged(this->_pixmap);
}

void babel::UserObject::updateUser(const User &user, User::Status status)
{
    _user = user;
    _status = status;
    this->loadAvatar();

    emit updated();
}

const babel::User &babel::UserObject::getUser(void) const
{
    return (_user);
}

babel::User::Status babel::UserObject::getStatus(void) const
{
    return (_status);
}

babel::UserObject::CallStatus babel::UserObject::getCallStatus(void) const
{
    return (_callStatus);
}

babel::User::Id babel::UserObject::getId(void) const
{
    return (_user.getId());
}

babel::User::Name babel::UserObject::getName(void) const
{
    return (_user.getName());
}

babel::User::Visibility babel::UserObject::getVisibility(void) const
{
    return (_user.getVisibility());
}

void babel::UserObject::setCallStatus(CallStatus status)
{
    _callStatus = status;
    emit updated();
}

const babel::net::Ip &babel::UserObject::getAudioIp(void) const
{
    return (_audioIp);
}

const babel::net::Port &babel::UserObject::getAudioPort(void) const
{
    return (_audioPort);
}

const std::string &babel::UserObject::getAudioPassword(void) const
{
    return (_audioPassword);
}

void babel::UserObject::setAudioIp(const net::Ip &ip)
{
    _audioIp = ip;
}

void babel::UserObject::setAudioPort(const net::Port &port)
{
    _audioPort = port;
}

void babel::UserObject::setAudioPassword(const std::string &password)
{
    _audioPassword = password;
}

void babel::UserObject::addMessage(const MessageObject *messageObject)
{
    this->_messages.push_back(messageObject);
    emit newMessage(messageObject);
}

const std::vector<const babel::MessageObject *> &babel::UserObject::getMessages() const
{
    return (this->_messages);
}

const QPixmap &babel::UserObject::getAvatarPixmap(void) const
{
    return (this->_pixmap);
}
