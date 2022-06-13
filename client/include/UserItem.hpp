/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** contact_btn
*/

#ifndef USERITEM_HPP_
#define USERITEM_HPP_

#include <QWidget>
#include <QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QPixmap>
#include <string>

#include "User.hpp"
#include "UserObject.hpp"

namespace babel {
    class UserManager;
}

namespace Ui {

    ///
    /// \brief Contact Button class
    ///
    class UserItem : public QWidget
    {
        Q_OBJECT

        public:
            /// \brief Constructor
            UserItem(QWidget *parent, const babel::UserObject *userObject);

            /// \brief Destructor
            ~UserItem();

            /// \brief Get user Object
            const babel::UserObject *getUserObject() const;

        private slots:
            /// \brief User Slot gets updated
            void slotUserUpdated();
            /// \brief Avatar Slot gets updated
            void slotAvatarUpdated(const QPixmap &pixmap);

        private:
            const babel::UserObject *_userObject;

            QLabel *_nameLabel;
            QLabel *_avatarLabel;
            QLabel *_onlineStatusLabel;
            QHBoxLayout *_horizontalLayout;
            QPixmap _pix;
            const QString _separator = " - ";
    };
}

#endif
