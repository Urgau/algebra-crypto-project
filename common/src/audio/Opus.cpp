/*
** EPITECH PROJECT, 2020
** Codec.cpp
** File description:
** Interfacing the opus library
*/

#include "audio/Opus.hpp"
#include "Error.hpp"
#include <sstream>

namespace babel {

    Opus::Opus(const int sampleRate, const int channel, const int frameSize) :
        _channels(0), _frameSize(0), _encoder(nullptr), _decoder(nullptr)
    {
        int errEncode = 0;
        int errDecode = 0;

        if (sampleRate != 8000 && sampleRate != 12000 && sampleRate != 16000 && sampleRate != 24000 && sampleRate != 48000)
            throw Error("Given value must be equal to 8000, 12000, 16000, 24000 or 48000", "Opus");

        if (channel != 1 && channel != 2)
            throw Error("Given value must be equal to 1 or 2", "Opus");
        this->_channels = channel;

        this->_frameSize = frameSize;
        this->_encoder = opus_encoder_create(sampleRate, this->_channels, OPUS_APPLICATION_VOIP, &errEncode);
        this->_decoder = opus_decoder_create(sampleRate, this->_channels, &errDecode);
        if (errEncode || errDecode)
            throw Error("Impossible to create encode/decoder", "Opus");
    }

    Opus::~Opus()
    {
        opus_encoder_destroy(this->_encoder);
        opus_decoder_destroy(this->_decoder);
    }

    void Opus::encode(const PortAudio::AudioFrame& in, ICodec::CodecFrame& out)
    {
        int ret = 0;

        out.resize(in.size());
        if ((ret = opus_encode_float(this->_encoder,
                        in.data(), this->_frameSize,
                        out.data(), out.size())) < 0)
            throw Error(opus_strerror(ret), "Opus::encode");
        out.resize(ret);
    }

    void Opus::decode(const ICodec::CodecFrame& in, PortAudio::AudioFrame& out)
    {
        int ret = 0;

        out.resize(this->_frameSize * this->_channels);
        if ((ret = opus_decode_float(this->_decoder,
                        in.data(), in.size(),
                        out.data(), this->_frameSize, 0)) < 0)
            throw Error(opus_strerror(ret), "Opus::decode");
    }
}
