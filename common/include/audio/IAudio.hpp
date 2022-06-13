/*
** EPITECH PROJECT, 2020
** ../../include/IAudio
** File description:
** IAudio
*/

#ifndef IAUDIO_HPP_
#define IAUDIO_HPP_

#include <string>
#include <vector>
#include <memory>
#include <functional>

///
/// \brief Babel namespace
///
namespace babel {

    ///
    /// \brief Interface for Audio
    ///
    template <typename T>
    class IAudio {
        public:
            ///
            /// \brief Typedef of audio data
            ///
            using AudioFrame = std::vector<T>;

            ///
            /// \brief Device struct
            ///
            struct Device {
                std::string name;
                int id;

                inline bool operator ==(const std::string &name) const
                { return (this->name == name); }
            };

            ///
            /// \brief Interface for the audio stream
            ///
            class IAudioStream {
                public:
                    /// \brief Add sound to the stream
                    virtual IAudioStream &operator <<(const IAudio::AudioFrame &sound) = 0;

                    /// \brief Retrive sound from the stream
                    virtual IAudioStream &operator >>(IAudio::AudioFrame &sound) = 0;

                    /// \brief Set if the IAudioStream is active
                    virtual void setActive(bool active) = 0;

                    /// \brief Set callback
                    virtual void setCallback(std::function<void()> fn) = 0;
            };

            /// \brief Get all the input devices avalaible
            virtual std::vector<Device> getInputDevices() const = 0;

            /// \brief Get all the output devices avalaible
            virtual std::vector<Device> getOutputDevices() const = 0;
    };
}

#endif
