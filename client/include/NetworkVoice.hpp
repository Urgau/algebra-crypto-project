/*
** EPITECH PROJECT, 2020
** client/include/NetworkVoice
** File description:
** check your malloc!
*/

#ifndef NETWORKVOICE_HPP_
#define NETWORKVOICE_HPP_

#include <QObject>
#include <QUdpSocket>
#include "audio/PortAudio.hpp"
#include "audio/Opus.hpp"
#include <openssl/evp.h>

// NOTE: need to be sync with Instance.hpp
#define LOCAL_AUDIO_PORT 7755

namespace babel {
    /// \brief Network Voice class
    class NetworkVoice : public QObject {

        Q_OBJECT

        public:
            /// \brief Constructor
            NetworkVoice(QObject *parent,
                    const PortAudio::Device &inputDevice,
                    const PortAudio::Device &outputDevice,
                    QHostAddress addr, int port,
                    const std::string &password) noexcept(false);

            /// \brief Destructor
            virtual ~NetworkVoice() noexcept(true);

            /// \brief Set activeness of the input stream
            void setInputActive(bool active);

            /// \brief Set activeness of the output stream
            void setOutputActive(bool active);

        private slots:
            /// \brief Read Pending Datagrams
            void readPendingDatagrams();

        private:
            QUdpSocket *_udpSocket;
            PortAudio::InputStream _input;
            PortAudio::OutputStream _output;
            Opus _opus;

            QHostAddress _addr;
            int _port;

            EVP_CIPHER_CTX *_en, *_de;
    };
}

#endif
