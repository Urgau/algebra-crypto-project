/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** BabelWindow
*/

#ifndef UI_BABELWINDOW_H
#define UI_BABELWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

#include <iostream>

#include "TabManager.hpp"
#include "ContactList.hpp"
#include "UserManager.hpp"
#include "UserWidget.hpp"
#include "ClientConnection.hpp"
#include "NetworkVoice.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Net.hpp"
#include "Log.hpp"

namespace Ui {

    /// \brief BabelWindow class
    class BabelWindow : public QMainWindow
    {
        Q_OBJECT

        public :
            /// \brief Constructor
            BabelWindow(const QString &serverIp, bool unsafeCert);
            /// \brief Destructor
            ~BabelWindow() = default;

        private slots:
            /// \brief Connect the client
            void clientConnectionConnected(void);
            /// \brief Disconnect the client
            void clientConnectionDisconnected(void);
            /// \brief Disconnect the client
            void clientConnectionErrorOccurred(void);
            /// \brief Wait for an event affecting itself
            void slotSelfEvent(const babel::events::SelfEvent &e);
            /// \brief Wait for a call event
            void slotCallEvent(const babel::events::CallEvent &e);
            /// \brief Wait for a hangup event
            void slotHangUpEvent(const babel::events::HangUpEvent &e);
            /// \brief Wait for a message event
            void slotMessageEvent(const babel::events::MessageEvent &e);
            /// \brief Wait for a error event
            void slotErrorEvent(const babel::events::ErrorEvent &e);

        private:
            /// \brief Open a call with someone
            void openCallWith(const babel::User::Id id,
                    const babel::net::Ip &ip, const babel::net::Port &port,
                    const std::string &password);

        private:
            QWidget *_centralwidget;
            QHBoxLayout *_horizontalLayout;
            QVBoxLayout *_verticalLayout;
            QMenuBar *_menubar;
            QStatusBar *_statusbar;
            Ui::ContactList *_contacts;
            Ui::UserWidget *_userWidget;
            babel::TabManager * _centralTab;
            babel::UserManager *_contactManager;
            babel::ClientConnection *_clientConnection;
            babel::NetworkVoice *_networkVoice = nullptr;
            std::shared_ptr<babel::PortAudio> _pa;
    };
}

#endif // UI_BABELWINDOW_H
