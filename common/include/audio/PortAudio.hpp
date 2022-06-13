/*
** EPITECH PROJECT, 2020
** common/include/Portaudio
** File description:
** 01100010 01101001 01101110 01100001 01110010 01111001
*/

#ifndef PORTAUDIO_HPP_
#define PORTAUDIO_HPP_

#include "IAudio.hpp"
#include <portaudio.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <memory>

namespace babel {

    ///
    /// \brief Portaudio interface
    ///
    class PortAudio : public IAudio<float> {
        public:
            /// \brief Default number of channels
            static const int CHANNELS_COUNT = 2;

            /// \brief Default sample rate
            static const int SAMPLE_RATE = 48000;

            /// \brief Default frames per buffer
            static const int FRAMES_PER_BUFFER = 480;

            /// \brief Default sample format
            static const int SAMPLE_FORMAT = paFloat32;

            ///
            /// \brief PortAudio input stream
            ///
            class InputStream : public IAudio::IAudioStream {
                public:
                    /// \brief Contructor
                    explicit InputStream(const IAudio::Device &device) noexcept(false);

                    /// \brief Destructor
                    ~InputStream() noexcept(false);

                    /// \brief Add sound to the stream
                    IAudioStream &operator <<(const PortAudio::AudioFrame &sound) override;

                    /// \brief Retrive sound from the stream
                    IAudioStream &operator >>(PortAudio::AudioFrame &sound) override;

                    /// \brief Set callback for this InputStream
                    void setCallback(std::function<void()> fn) override;

                    /// \brief Set activeness of this InputStream
                    void setActive(bool active) override;
                private:
                    static int callback(
                            const void *input, void *output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData);

                    bool _active;
                    bool _destroyed;
                    boost::lockfree::spsc_queue<PortAudio::AudioFrame> _sounds;
                    std::function<void()> _fn;
                    PaStream *_stream;
            };

            ///
            /// \brief PortAudio output stream
            ///
            class OutputStream : public IAudio::IAudioStream {
                public:
                    /// \brief Contructor
                    explicit OutputStream(const IAudio::Device &device) noexcept(false);

                    /// \brief Destructor
                    ~OutputStream() noexcept(false);

                    /// \brief Add sound to the stream
                    IAudioStream &operator <<(const PortAudio::AudioFrame &sound) override;

                    /// \brief Retrive sound from the stream
                    IAudioStream &operator >>(PortAudio::AudioFrame &sound) override;

                    /// \brief Set callback
                    void setCallback(std::function<void()> fn) override;

                    /// \brief Set activeness of this OutputStream
                    void setActive(bool active) override;
                private:
                    static int callback(
                            const void *input, void *output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData);

                    bool _active;
                    bool _destroyed;
                    boost::lockfree::spsc_queue<PortAudio::AudioFrame> _sounds;
                    PaStream *_stream;
            };

            /// \brief Constructor
            PortAudio() noexcept(false);

            /// \brief Destructor
            virtual ~PortAudio() noexcept(false);

            /// \brief Get all the input devices avalaible
            std::vector<IAudio::Device> getInputDevices() const override;

            /// \brief Get all the output devices avalaible
            std::vector<IAudio::Device> getOutputDevices() const override;
    };
}

#endif
