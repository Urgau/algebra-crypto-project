/*
** EPITECH PROJECT, 2020
** common/src/PortAudio
** File description:
** don't forget to free at the end
*/

#include "audio/PortAudio.hpp"
#include "Error.hpp"

#define try(code) \
    do { \
        int err = code; \
        if (err != paNoError && \
                err != paInputOverflowed && \
                err != paOutputUnderflowed) \
            throw Error(Pa_GetErrorText(err), "PortAudio"); \
    } while (0)

namespace babel {
    PortAudio::PortAudio() noexcept(false)
    {
        try(Pa_Initialize());
    }

    std::vector<PortAudio::Device> PortAudio::getInputDevices() const
    {
        std::vector<PortAudio::Device> devices;
        int count = Pa_GetDeviceCount();

        for (int i = 0; i < count; i++) {
            const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
            if (deviceInfo == nullptr)
                continue;

            if (!(deviceInfo->maxInputChannels >= PortAudio::CHANNELS_COUNT))
                continue;

            devices.push_back({ std::string(deviceInfo->name), i });
        }
        return (devices);
    }

    std::vector<PortAudio::Device> PortAudio::getOutputDevices() const
    {
        std::vector<PortAudio::Device> devices;
        int count = Pa_GetDeviceCount();

        for (int i = 0; i < count; i++) {
            const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
            if (deviceInfo == nullptr)
                continue;

            if (!(deviceInfo->maxOutputChannels >= PortAudio::CHANNELS_COUNT))
                continue;

            devices.push_back({ std::string(deviceInfo->name), i });
        }
        return (devices);
    }

    PortAudio::~PortAudio() noexcept(false)
    {
        try(Pa_Terminate());
    }

    PortAudio::InputStream::InputStream(const PortAudio::Device &device)
        noexcept(false) :  _active(true), _destroyed(false), _sounds(8)
    {
        const PaDeviceInfo *inputDevice = Pa_GetDeviceInfo(device.id);

        PaStreamParameters inputStreamParameters;
        inputStreamParameters.device = device.id;
        inputStreamParameters.channelCount = PortAudio::CHANNELS_COUNT;
        inputStreamParameters.sampleFormat = PortAudio::SAMPLE_FORMAT;
        inputStreamParameters.suggestedLatency = inputDevice->defaultHighInputLatency;
        inputStreamParameters.hostApiSpecificStreamInfo = nullptr;

        try(Pa_OpenStream(&_stream,
                    &inputStreamParameters,
                    nullptr,
                    PortAudio::SAMPLE_RATE,
                    PortAudio::FRAMES_PER_BUFFER,
                    paClipOff,
                    InputStream::callback,
                    this));

        try(Pa_StartStream(_stream));
    }

    PortAudio::IAudioStream &PortAudio::InputStream::operator <<(const PortAudio::AudioFrame &)
    {
        return (*this);
    }

    PortAudio::IAudioStream &PortAudio::InputStream::operator >>(PortAudio::AudioFrame &sound)
    {
        PortAudio::AudioFrame audioFrame;
        this->_sounds.pop(audioFrame);

        sound = audioFrame;
        return (*this);
    }

    int PortAudio::InputStream::callback(
                            const void *input,
                            [[maybe_unused]] void *output,
                            [[maybe_unused]] unsigned long frameCount,
                            [[maybe_unused]] const PaStreamCallbackTimeInfo *timeInfo,
                            [[maybe_unused]] PaStreamCallbackFlags statusFlags,
                            void *userData)
    {
        const std::size_t size = PortAudio::FRAMES_PER_BUFFER * PortAudio::CHANNELS_COUNT;
        PortAudio::InputStream *inputStream =
            reinterpret_cast<PortAudio::InputStream *>(userData);

        if (inputStream->_active) {
            PortAudio::AudioFrame sound;
            sound.assign(
                    reinterpret_cast<const float *>(input),
                    reinterpret_cast<const float *>(input) + size);
            inputStream->_sounds.push(sound);

            if (!inputStream->_destroyed && inputStream->_fn)
                inputStream->_fn();
        }

        if (inputStream->_destroyed)
            return (paComplete);
        return (paContinue);
    }

    void PortAudio::InputStream::setCallback(std::function<void()> fn)
    {
        this->_fn = fn;
    }

    void PortAudio::InputStream::setActive(bool active)
    {
        this->_active = active;
    }

    PortAudio::InputStream::~InputStream() noexcept(false)
    {
        this->_destroyed = true;
        try(Pa_StopStream(_stream));
        try(Pa_CloseStream(_stream));
    }

    PortAudio::OutputStream::OutputStream(const PortAudio::Device &device) noexcept(false)
        : _active(true), _destroyed(false), _sounds(8)
    {
        const PaDeviceInfo *outputDevice = Pa_GetDeviceInfo(device.id);

        PaStreamParameters outputStreamParameters;
        outputStreamParameters.device = device.id;
        outputStreamParameters.channelCount = PortAudio::CHANNELS_COUNT;
        outputStreamParameters.sampleFormat = PortAudio::SAMPLE_FORMAT;
        outputStreamParameters.suggestedLatency = outputDevice->defaultHighOutputLatency;
        outputStreamParameters.hostApiSpecificStreamInfo = nullptr;

        try(Pa_OpenStream(&_stream,
                    nullptr,
                    &outputStreamParameters,
                    PortAudio::SAMPLE_RATE,
                    PortAudio::FRAMES_PER_BUFFER,
                    paClipOff,
                    OutputStream::callback,
                    this));

        try(Pa_StartStream(_stream));
    }

    PortAudio::IAudioStream &PortAudio::OutputStream::operator <<(const PortAudio::AudioFrame &sound)
    {
        if (sound.size() != 0) {
            PortAudio::AudioFrame lSound;
            lSound = sound;

            this->_sounds.push(lSound);
        }
        return (*this);
    }

    PortAudio::IAudioStream &PortAudio::OutputStream::operator >>(PortAudio::AudioFrame &)
    {
        return (*this);
    }

    int PortAudio::OutputStream::callback(
                            [[maybe_unused]] const void *input,
                            void *output,
                            [[maybe_unused]] unsigned long frameCount,
                            [[maybe_unused]] const PaStreamCallbackTimeInfo *timeInfo,
                            [[maybe_unused]] PaStreamCallbackFlags statusFlags,
                            void *userData)
    {
        const std::size_t size = PortAudio::FRAMES_PER_BUFFER * PortAudio::CHANNELS_COUNT;
        PortAudio::OutputStream *outputStream =
            reinterpret_cast<PortAudio::OutputStream *>(userData);

        {
            float *wptr = reinterpret_cast<float *>(output);
            PortAudio::AudioFrame audioFrame;

            if (outputStream->_active && outputStream->_sounds.pop(audioFrame)) {
                for (std::size_t i = 0; i < audioFrame.size(); i++)
                    *wptr++ = audioFrame[i];
            } else {
                for (std::size_t i = 0; i < size; i++)
                    *wptr++ = 0.0;
            }
        }

        if (outputStream->_destroyed)
            return (paComplete);
        return (paContinue);
    }

    void PortAudio::OutputStream::setCallback(std::function<void()>)
    {
        throw Error("Not implemented", "PortAudio");
    }

    void PortAudio::OutputStream::setActive(bool active)
    {
        this->_active = active;
    }

    PortAudio::OutputStream::~OutputStream() noexcept(false)
    {
        this->_destroyed = true;
        try(Pa_StopStream(_stream));
        try(Pa_CloseStream(_stream));
    }
}
