/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserObject
*/

#ifndef USEROBJECT_HPP_
#define USEROBJECT_HPP_

#include <QObject>
#include <QPixmap>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "MessageObject.hpp"
#include "User.hpp"
#include "Net.hpp"

namespace babel {
    /// \brief Generic User holder for the Qt side
    class UserObject : public QObject {
        Q_OBJECT

        public:
            enum CallStatus {
                NoCall = 0,
                Ringing = 1,
                InCall = 2
            };

            /// \brief Constructor
            UserObject(QObject *parent, const User &user,
                    User::Status status, CallStatus callStatus = CallStatus::NoCall);
            /// \brief Default destructor
            ~UserObject() = default;

            /// \brief Update the user given to the status given
            void updateUser(const User &user, User::Status status);
            /// \brief Get the current user
            const User &getUser(void) const;
            /// \brief Get the current user status
            User::Status getStatus(void) const;
            /// \brief Get the current user id
            User::Id getId(void) const;
            /// \brief Get the current user name
            User::Name getName(void) const;
            /// \brief Get the current user visibility
            User::Visibility getVisibility(void) const;
            /// \brief Get the current user call status
            CallStatus getCallStatus(void) const;

            /// \brief Set the call status
            void setCallStatus(CallStatus status);

            /// \brief Set the audio ip
            void setAudioIp(const net::Ip &ip);
            /// \brief Set the audio port
            void setAudioPort(const net::Port &port);
            /// \brief Set the audio port
            void setAudioPassword(const std::string &password);

            /// \brief Get the audio ip
            const net::Ip &getAudioIp(void) const;
            /// \brief Get the audio port
            const net::Port &getAudioPort(void) const;
            /// \brief Get the audio password
            const std::string &getAudioPassword(void) const;
            /// \brief Add the message given as data to the user
            void addMessage(const MessageObject *);
            /// \brief Get the messages
            const std::vector<const MessageObject *> &getMessages() const;
            /// \brief Get the Avatr
            const QPixmap &getAvatarPixmap(void) const;

        signals:
            /// \brief Event when something is updated
            void updated();
            /// \brief Event when the avatar is changed
            void avatarChanged(const QPixmap &);
            /// \brief Event when a new message is sent
            void newMessage(const MessageObject *);

        private slots:
            /// \brief Event when the network manager is finished
            void slotNetworkManagerFinished(QNetworkReply *reply);

        private:
            /// \brief Load the user avatar
            void loadAvatar(void);

        private:
            User _user;
            User::Status _status;
            CallStatus _callStatus;

            net::Ip _audioIp;
            net::Port _audioPort;
            std::string _audioPassword;
            std::vector<const MessageObject *> _messages;

            QPixmap _pixmap;
            User::Avatar _lastAvatar;
            QNetworkAccessManager *_networkAccessManager;
    };
};

#endif /* !USEROBJECT_HPP_ */
