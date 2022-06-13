/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** TabManager
*/

#ifndef TABMANAGER_HPP_
#define TABMANAGER_HPP_

#include <QObject>
#include "UserObject.hpp"
#include "Message.hpp"
#include "TabElem.hpp"
#include "User.hpp"

namespace babel {

    ///
    /// \brief Tab Manager class
    ///
    class TabManager : public QObject {

        Q_OBJECT

        public:
            /// \brief Constructor
            TabManager(QWidget *mother, QHBoxLayout *layout);
            /// \brief Destructor
            ~TabManager() = default;

        public slots:
            /// \brief Slot to open a user in the tab
            void slotOpenUser(const UserObject *userObject);

        private slots:
            /// \brief Slot to call user in the tab
            void slotCallUser(User::Id);
            /// \brief Slot to pickup in the tab
            void slotPickUpUser(User::Id);
            /// \brief Slot to hangup in the tab
            void slotHangUpUser(User::Id);
            /// \brief Slot to close the tab
            void slotCloseTab(int index);
            /// \brief Slot to show the messages in the tab
            void slotMessageUser(User::Id, Message::Body);

        signals:
            /// \brief Signal to call User
            void signalCallUser(User::Id);
            /// \brief Signal to pickup call
            void signalPickUpUser(User::Id);
            /// \brief Signal to hangup call
            void signalHangUpUser(User::Id);
            /// \brief Signal message send
            void signalMessageUser(User::Id, Message::Body);

        protected:
        private:
            QTabWidget *_tab;
            std::map<User::Id, Ui::TabElem *> _tabsUserId;

            /// \brief Add a contact tab
            void addContactTab(const UserObject *userData);
            /// \brief switch to an existing tab
            bool switchIfTabExist(User::Id id);
    };
};

#endif /* !TABMANAGER_HPP_ */
