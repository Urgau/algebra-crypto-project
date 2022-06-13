/*
** EPITECH PROJECT, 2020
** client/include/ContactList
** File description:
** check your malloc!
*/

#ifndef UI_CONTACTLIST_HPP_
#define UI_CONTACTLIST_HPP_

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QWidget>
#include <QListWidget>

#include "UserItem.hpp"
#include "UserObject.hpp"

namespace Ui {
    /// \brief ContactList class
    class ContactList: public QGroupBox
    {
        Q_OBJECT

        public:
            // \brief Constructor
            ContactList(QWidget *mother);

            /// \brief Destructor
            ~ContactList() = default;

        public slots:
            /// \brief event User Created
            void slotUserCreated(const babel::UserObject *userObject);

        signals:
            /// \brief who tells when user is clicked
            void signalUserClicked(const babel::UserObject *userObject);

        private:
            /// \brief setup UI
            void setupUi(QWidget *parent);
            /// \brief retrlanslate UI
            void retranslateUi(QWidget *parent);

        private:
            QGridLayout *_gridLayout;
            QListWidget *_listWidget;
    };
}

#endif // UI_CONTACTLIST_HPP_
