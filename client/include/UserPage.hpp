/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserPage
*/

#ifndef USERPAGE_HPP_
#define USERPAGE_HPP_

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MessageItem.hpp"
#include "UserObject.hpp"

namespace Ui {
    // \brief UserPage class, used to handle the userpage in QT
    class UserPage: public QWidget
    {
        Q_OBJECT


        public:
            // \brief Constructor
            UserPage(QWidget *parent, const babel::UserObject *userObject);

            /// \brief Destructor
            ~UserPage() = default;

        signals:
            /// \brief send this to call an user
            void signalCallUser(babel::User::Id);
            /// \brief send this to hangup
            void signalHangUpUser(babel::User::Id);
            /// \brief send this to pickup
            void signalPickUpUser(babel::User::Id);
            /// \brief send this to send a message
            void signalMessageUser(babel::User::Id, babel::Message::Body);

        private slots:
            /// \brief Event when the User is updated
            void slotUserUpdated(void);
            /// \brief Event when the call button is pushed
            void slotCallButtonPush(void);
            /// \brief Event when the hangup button is pushed
            void slotHangUpButtonPush(void);
            /// \brief Event when a message is sent
            void slotSendMessage(void);
            /// \brief Event when a new message is sent
            void slotNewMessage(const babel::MessageObject *messageObject);

        private:
            const babel::UserObject *_userObject;
            QVBoxLayout *_contactCallLayout;
            QHBoxLayout *_topButtonsLayout;
            QPushButton *_button2;
            QPushButton *_button1;
            QListWidget *_messageList;
            QHBoxLayout *_newMessageLayout;
            QLineEdit *_messageContent;
            QPushButton *_sendButton;

            void setupUi(QWidget *UserPage);
    };
}

#endif // UI_USERPAGE_H
