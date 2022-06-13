/*
** EPITECH PROJECT, 2020
** common/include/AMessage
** File description:
** programmers start to count from 0
*/

#ifndef AMESSAGE_HPP_
#define AMESSAGE_HPP_

#include <string>

namespace babel {
    /// \brief Abstract message class
    class AMessage {
        public:
            /// \brief Constructor of AMessage
            AMessage(const std::string name) : _name(name)
            { /* ignore */ }

            /// \brief Default destructor
            ~AMessage() = default;

            /// \brief Get the name of the message
            const std::string getMessageName() const
            {
                return (this->_name);
            }
        private:
            std::string _name;
    };
}

#endif
