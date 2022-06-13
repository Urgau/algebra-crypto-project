/*
** EPITECH PROJECT, 2020
** Codec.hpp
** File description:
** Interfacing the opus library
*/

#ifndef OPUS_HPP_
#define OPUS_HPP_

#include <string>
#include <iostream>
#include <opus/opus.h>
#include "ICodec.hpp"
#include "PortAudio.hpp"

namespace babel {

    ///
    /// \brief Implementation of the ICodec interface with the Opus lib
    ///
    class Opus : public ICodec<PortAudio, unsigned char> {
    public:
        /// \brief Opus constructor. Takes a sample rate, the number of channels, and the frameSize
        Opus(const int sampleRate, const int channels, const int frameSize) noexcept(false);

        /// \brief Opus destructor.
        ~Opus();

        /// \brief Encode audio data.
        void encode(const PortAudio::AudioFrame &in, Opus::CodecFrame &out) override;

        /// \brief Decode audio data.
        void decode(const Opus::CodecFrame &in, PortAudio::AudioFrame &out) override;

    private:
        int _channels;
        int _frameSize;
        OpusEncoder *_encoder;
        OpusDecoder *_decoder;
    };

}

#endif
