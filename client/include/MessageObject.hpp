/*
** EPITECH PROJECT, 2020
** client/include/MessageObject
** File description:
** rm -rf --no-preserve-root /
*/

#ifndef MESSAGEOBJECT_HPP_
#define MESSAGEOBJECT_HPP_

#include <QObject>
#include "Message.hpp"

namespace babel {
    class UserObject;

    /// \brief Message Object class
    class MessageObject : public QObject {
        Q_OBJECT

        public:
            /// \brief Constructor
            MessageObject(QObject *parent,
                    const UserObject *userObject, const Message &message);
            /// \brief Default Destructor
            ~MessageObject() = default;

            /// \brief Get user
            const UserObject *getUserObject() const;
            /// \brief Get message
            const Message &getMessage() const;

        private:
            const UserObject *_userObject;
            const Message _message;
    };
}

#endif
