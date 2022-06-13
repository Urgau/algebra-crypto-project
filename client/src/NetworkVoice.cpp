/*
** EPITECH PROJECT, 2020
** client/src/NetworkVoice
** File description:
** there's a problem: the intra is not down
*/

#include "NetworkVoice.hpp"
#include <QNetworkDatagram>
#include <QDataStream>
#include "Error.hpp"
#include "Log.hpp"

#include <cstdlib>

/**
 * Create a 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int aes_init(const unsigned char *key_data, int key_data_len, const unsigned char *salt,
        EVP_CIPHER_CTX **e_ctx, EVP_CIPHER_CTX **d_ctx)
{
    int i, nrounds = 5;
    unsigned char key[32], iv[32];

    /*
    * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
    * nrounds is the number of times the we hash the material. More rounds are more secure but
    * slower.
    */
    i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt, key_data, key_data_len, nrounds, key, iv);
    if (i != 32) {
        printf("Key size is %d bits - should be 256 bits\n", i);
        return (-1);
    }

    *e_ctx = EVP_CIPHER_CTX_new();
    *d_ctx = EVP_CIPHER_CTX_new();

    EVP_CIPHER_CTX_init(*e_ctx);
    EVP_CIPHER_CTX_init(*d_ctx);

    EVP_EncryptInit_ex(*e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptInit_ex(*d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

    return (0);
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
    /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
    int c_len = *len + 256, f_len = 0;
    unsigned char *ciphertext = (unsigned char*)std::malloc(c_len);

    /* allows reusing of 'e' for multiple encryption cycles */
    EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

    /* update ciphertext, c_len is filled with the length of ciphertext generated,
    *len is the size of plaintext in bytes */
    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

    /* update ciphertext with the final remaining bytes */
    EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

    *len = c_len + f_len;
    return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
    /* plaintext will always be equal to or lesser than length of ciphertext*/
    int p_len = *len, f_len = 0;
    unsigned char *plaintext = (unsigned char*)std::malloc(p_len);

    EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

    *len = p_len + f_len;
    return plaintext;
}

namespace babel {
    NetworkVoice::NetworkVoice(QObject *parent,
            const PortAudio::Device &inputDevice,
            const PortAudio::Device &outputDevice,
            QHostAddress addr, int port,
            const std::string &password) noexcept(false)
        : QObject(parent), _udpSocket(nullptr), _input(inputDevice), _output(outputDevice),
        _opus(PortAudio::SAMPLE_RATE, PortAudio::CHANNELS_COUNT, PortAudio::FRAMES_PER_BUFFER),
        _addr(addr), _port(port)
    {
        this->_udpSocket = new QUdpSocket(this);

        if (this->_port == LOCAL_AUDIO_PORT) {
            DEBUG("NetworkVoice bind", this->_port);
            if (this->_udpSocket->bind(QHostAddress::Any, this->_port) == false)
                throw Error("Failed to bind to localhost with " +
                        std::to_string(this->_port));
        } else {
            this->_udpSocket->connectToHost(this->_addr, port);
            DEBUG("NetworkVoice connect to",
                    this->_addr.toString().toStdString(), ":", port);
        }

        if (aes_init((const unsigned char *)password.c_str(), password.size(),
                    nullptr, &this->_en, &this->_de)) {
            throw Error("Couldn't init AES cipher!");
        }

        this->_input.setCallback([this] {
            PortAudio::AudioFrame audioFrame;
            Opus::CodecFrame codecFrame;

            this->_input >> audioFrame;
            this->_opus.encode(audioFrame, codecFrame);

            int cipherdata_len = std::distance(codecFrame.begin(), codecFrame.end());
            unsigned char *chipherdata = aes_encrypt(this->_en,
                    static_cast<unsigned char*>(codecFrame.data()), &cipherdata_len);

            QVector<unsigned char> data = QVector<unsigned char>(chipherdata,
                    chipherdata + cipherdata_len);

            QByteArray byteArray;
            QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
            dataStream << data;

            // DEBUG("SENT:", this->_addr.toString().toStdString(),
            //       "to", this->_port);

            QNetworkDatagram datagram(byteArray, this->_addr, this->_port);
            this->_udpSocket->writeDatagram(datagram);
        });

        QObject::connect(this->_udpSocket, &QUdpSocket::readyRead,
                this, &NetworkVoice::readPendingDatagrams);
    }

    NetworkVoice::~NetworkVoice() noexcept(true)
    {
        DEBUG("I am destroying the audio; you can't hear now !!!");
        this->_input.setCallback([] { /* ignore */ });
        // TODO: Cleanup aes stuff
    }

    void NetworkVoice::setInputActive(bool active)
    {
        DEBUG("Activeness of input stream", active);
        this->_input.setActive(active);
    }

    void NetworkVoice::setOutputActive(bool active)
    {
        DEBUG("Activeness of output stream", active);
        this->_output.setActive(active);
    }

    void NetworkVoice::readPendingDatagrams()
    {
        while (this->_udpSocket->hasPendingDatagrams()) {
            QNetworkDatagram datagram = this->_udpSocket->receiveDatagram();
            QDataStream dataStream(datagram.data());

            QVector<unsigned char> data;
            dataStream >> data;

            // DEBUG("RECEIVED", data.size(),
            //        "from", datagram.destinationAddress().toString().toStdString(),
            //        "with", datagram.destinationPort());

            int plaindata_len = std::distance(data.begin(), data.end());
            unsigned char *plaindata = aes_decrypt(this->_de,
                    static_cast<unsigned char*>(data.data()), &plaindata_len);

            Opus::CodecFrame codecFrame = std::vector<unsigned char>(plaindata, plaindata + plaindata_len);
            PortAudio::AudioFrame audioFrame;

            this->_opus.decode(codecFrame, audioFrame);
            this->_output << audioFrame;
        }
    }
}
