/*
** EPITECH PROJECT, 2020
** client/include/MessageItem
** File description:
** hello world?
*/

#ifndef MESSAGEITEM_HPP_
#define MESSAGEITEM_HPP_

#include <QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "UserObject.hpp"

namespace Ui {
    /// \brief A message Item class
    class MessageItem : public QWidget {
        Q_OBJECT

        public:
            /// \brief Constructor
            MessageItem(QWidget *parent, const babel::MessageObject *messageObject);
            /// \brief Destructor
            ~MessageItem() = default;

        private slots:
            /// \brief Update the message user
            void slotUserUpdated();
            /// \brief Put an image from an URL into the message
            void slotnetwManagerFinished(QNetworkReply *reply);

        signals:
            /// \brief Even when the size of the window is changed
            void sizeHintChanged();

        private:
            const babel::MessageObject *_messageObject;

            QLabel *_timestampLabel;
            QLabel *_usernameLabel;
            QLabel *_bodyLabel;
            QLabel *_imageLabel;
            QHBoxLayout *_hBoxLayout2;
    };
}

#endif
