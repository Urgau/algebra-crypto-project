/*
** EPITECH PROJECT, 2020
** client/include/ClientConnection
** File description:
** man man
*/

#ifndef CLIENTCONNECTION_HPP_
#define CLIENTCONNECTION_HPP_

#define MAX_LINE_SIZE 256

#include <QString>
#include <QSslSocket>
#include <QHostAddress>
#include "Error.hpp"

##HANDLERS_HEADERS##

namespace babel {
    /// \brief Implement a QSslSocket for the connection with the server
    class ClientConnection : public QSslSocket {
        Q_OBJECT

        public:
            /// \brief Default constructor
            ClientConnection(QObject *parent = nullptr);

            void setUnsafeCert(bool unsafeCert);

        signals:
##HANDLERS_SIGNALS##

        private slots:
            /// \brief Process QSslSocket::readyRead()
            void processReadyRead();

            /// \brief Process QSslSocket::connected()
            void processConnected();

            /// \brief Process QSslSocket::disconnected()
            void processDisconnected();

            /// \brief Process QSslSocket::encrypted()
            void processEncrypted();

            /// \brief Process QSslSocket::sslErrors()
            void processSslErrors(const QList<QSslError> &errors);

        private:
            /// \brief Handle event from an input stream
            void handleEvent(std::istream &input) noexcept(true);

            bool _unsafeCertAllowed = false;
    };
}

#endif
