/*
** EPITECH PROJECT, 2020
** nanotekspace
** File description:
** Error
*/

#include "Error.hpp"

Error::Error(const std::string &message, const std::string &component)
    : _message(message), _componant(component)
{ /* ignore */ }

const std::string &Error::getMessage() const
{
    return (this->_message);
}

const std::string &Error::getComponent() const
{
    return (this->_componant);
}

const char *Error::what() const noexcept
{
    return (this->_message.c_str());
}
