/*
** EPITECH PROJECT, 2020
** opus_lib.hpp
** File description:
** Interfacing the opus library
*/

#ifndef ICODEC_HPP_
#define ICODEC_HPP_

#include <vector>
#include "IAudio.hpp"

namespace babel {

    ///
    /// \brief ICodec Interface for the encoder or decoder
    ///
    template <class C, typename T>
    class ICodec {
    public:
        /// \brief Typedef of a CodecFrame
        using CodecFrame = std::vector<T>;

        /// \brief destructor
        virtual ~ICodec() = default;

        /// \brief Encode audio data.
        virtual void encode(const typename C::AudioFrame &in, ICodec::CodecFrame& out) = 0;

        /// \brief Decode audio data.
        virtual void decode(const ICodec::CodecFrame& in, typename C::AudioFrame& out) = 0;
    };

}

#endif
