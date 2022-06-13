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

#include <map>
#include <sstream>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "events/SelfEvent.hpp"
#include "events/UserEvent.hpp"
#include "events/CallEvent.hpp"
#include "events/HangUpEvent.hpp"
#include "events/MessageEvent.hpp"
#include "events/ErrorEvent.hpp"
#include "commands/IdentifyCommand.hpp"
#include "commands/UpdateCommand.hpp"
#include "commands/UsersCommand.hpp"
#include "commands/CallCommand.hpp"
#include "commands/HangUpCommand.hpp"
#include "commands/MessageCommand.hpp"

namespace babel {
    /// \brief Implement a QSslSocket for the connection with the server
    class ClientConnection : public QSslSocket {
        Q_OBJECT

        public:
            /// \brief Default constructor
            ClientConnection(QObject *parent = nullptr);

            void setUnsafeCert(bool unsafeCert);

        signals:

            /// \brief Signal for handling SelfEvent
            void handleSelfEvent(const events::SelfEvent &);

            /// \brief Signal for handling UserEvent
            void handleUserEvent(const events::UserEvent &);

            /// \brief Signal for handling CallEvent
            void handleCallEvent(const events::CallEvent &);

            /// \brief Signal for handling HangUpEvent
            void handleHangUpEvent(const events::HangUpEvent &);

            /// \brief Signal for handling MessageEvent
            void handleMessageEvent(const events::MessageEvent &);

            /// \brief Signal for handling ErrorEvent
            void handleErrorEvent(const events::ErrorEvent &);

        public:
            /// \brief Serialize an input and send it to the corresponding socket
            template<class M>
            void serializeAndSend(const M &a)
            {
                std::stringbuf stringbuf;

                {
                    std::ostream output(&stringbuf);
                    output << a.getMessageName() << " ";
                    boost::archive::text_oarchive oa(output);
                    oa << a;
                }

                write(stringbuf.str().c_str());
            }

        private:
            /// \brief Deserialize an input and send the corresponding signal
            template<class M, void (babel::ClientConnection::*f)(const M &)>
            void deserializeAndEmitSignal(std::istream &input)
            {
                M a;

                try {
                    boost::archive::text_iarchive ia(input);
                    ia >> a;
                } catch (const std::exception &ex) {
                    throw Error("Failed to deserialize input", "deserializeAndEmitSignal");
                }

                emit (*this.*f)(a);
            }

            /// \brief Map that store the name of the handler and a wrapper function
            /// that parse the serialize data and call the real handler function
            const std::map<std::string, void (babel::ClientConnection::*)(std::istream &)> HANDLERS = {
                { "SelfEvent", &babel::ClientConnection::deserializeAndEmitSignal
                    <babel::events::SelfEvent, &babel::ClientConnection::handleSelfEvent> },
                { "UserEvent", &babel::ClientConnection::deserializeAndEmitSignal
                    <babel::events::UserEvent, &babel::ClientConnection::handleUserEvent> },
                { "CallEvent", &babel::ClientConnection::deserializeAndEmitSignal
                    <babel::events::CallEvent, &babel::ClientConnection::handleCallEvent> },
                { "HangUpEvent", &babel::ClientConnection::deserializeAndEmitSignal
                    <babel::events::HangUpEvent, &babel::ClientConnection::handleHangUpEvent> },
                { "MessageEvent", &babel::ClientConnection::deserializeAndEmitSignal
                    <babel::events::MessageEvent, &babel::ClientConnection::handleMessageEvent> },
                { "ErrorEvent", &babel::ClientConnection::deserializeAndEmitSignal
                    <babel::events::ErrorEvent, &babel::ClientConnection::handleErrorEvent> },
            };
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
