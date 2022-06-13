/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserManager
*/

#ifndef CONTACTMANAGER_HPP_
#define CONTACTMANAGER_HPP_

#include <map>
#include <QWidget>
#include <QListWidget>

#include "UserItem.hpp"
#include "UserObject.hpp"
#include "events/UserEvent.hpp"
#include "events/SelfEvent.hpp"

namespace Ui {
    class UserButton;
}

namespace babel {

    ///
    /// \brief Contact Manager class
    ///
    class UserManager : public QObject {
        Q_OBJECT

        public:
            /// \brief Constructor
            UserManager(QObject *parent = nullptr);

            /// \brief Destructor
            ~UserManager();

            /// \brief Get the self UserObject
            const User &getSelf(void) const;

            /// \brief Get the call status for a user
            UserObject::CallStatus getUserCallStatus(User::Id id) const;

            /// \brief Set the call status for a user
            void setUserCallStatus(User::Id id, UserObject::CallStatus status);

            /// \brief Get the remote audio ip for a user
            const net::Ip &getUserAudioIp(User::Id id) const;

            /// \brief Get the remote audio ip for a user
            const net::Port &getUserAudioPort(User::Id id) const;

            /// \brief Get the remote audio password for a user
            const std::string &getUserAudioPassword(User::Id id) const;

            /// \brief Set the remote audio ip for a user
            void setUserAudioNetwork(User::Id id,
                    const net::Ip &ip, const net::Port &port,
                    const std::string &password);

            /// \brief Add message
            void addMessage(User::Id into, User::Id from,
                    const Message &message);

        public slots:
            void slotUserEvent(const events::UserEvent &);
            void slotSelfEvent(const events::SelfEvent &);

        signals:
            void signalSelfUpdated(const User &user);
            void signalSelfAvatarUpdated(const QPixmap &pixmap);
            void signalUserCreated(const UserObject *userObject);

        protected:
        private:
            /// \brief Set data for a user (created or not)
            void setContactInfo(const User &user, User::Status status);

            std::map<User::Id, UserObject *> _users;
            UserObject *_selfObject;
    };
}

#endif /* !CONTACTMANAGER_HPP_ */
