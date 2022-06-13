/*
** EPITECH PROJECT, 2020
** client/src/ClientConnection
** File description:
** hello world?
*/

#include "Log.hpp"
#include "ClientConnection.hpp"

namespace babel {
    ClientConnection::ClientConnection(QObject *parent)
        : QSslSocket(parent)
    {
        QObject::connect(this, &QSslSocket::readyRead,
                this, &ClientConnection::processReadyRead);
        QObject::connect(this, &QSslSocket::connected,
                this, &ClientConnection::processConnected);
        QObject::connect(this, &QSslSocket::disconnected,
                this, &ClientConnection::processDisconnected);
        QObject::connect(this, &QSslSocket::encrypted,
                this, &ClientConnection::processEncrypted);
        QObject::connect(this,
                static_cast<void (QSslSocket::*)(const QList<QSslError> &)>
                    (&QSslSocket::sslErrors),
                this, &ClientConnection::processSslErrors);
    }

    void ClientConnection::setUnsafeCert(bool unsafeCert)
    {
        this->_unsafeCertAllowed = unsafeCert;
    }

    void ClientConnection::processConnected()
    {
        INFO("ClientConnection: connected :-)");
    }

    void ClientConnection::processDisconnected()
    {
        INFO("ClientConnection: disconnected :-(");
    }

    void ClientConnection::processEncrypted()
    {
        INFO("ClientConnection: encrypted :-) %");
    }

    void ClientConnection::processSslErrors(const QList<QSslError> &errors)
    {
        if (this->_unsafeCertAllowed) {
            WARN("ClientConnection: SslErrors :-( Ignoring them...");
            this->ignoreSslErrors(errors);
        } else {
            ERROR("ClientConnection: SslErrors :-(");
            qDebug() << errors;
        }
    }

    void ClientConnection::processReadyRead()
    {
        while (this->canReadLine()) {
            QByteArray data = this->readLine(MAX_LINE_SIZE);
            std::istringstream iss(data.constData());

            this->handleEvent(iss);
        }
    }

    void ClientConnection::handleEvent(std::istream &input) noexcept(true)
    {
        try {
            if (input.eof())
                throw Error("EOF on input stream", "ClientConnection::handleEvent");

            std::string eventName;
            input >> eventName;

            if (eventName.empty())
                throw Error("Empty event name", "ClientConnection::handleEvent");

            DEBUG(eventName);

            try {
                (*this.*HANDLERS.at(eventName))(input);
            } catch (const std::out_of_range &) {
                throw Error("Invalid event name", "ClientConnection::handleEvent");
            }
        } catch (const Error &error) {
            ERROR(error.getComponent(), ":", error.getMessage());
        }
    }
}
