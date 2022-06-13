/*
** EPITECH PROJECT, 2020
** nanotekspace
** File description:
** Error
*/

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <string>
#include <exception>

///
/// \brief Error class
///
class Error : public std::exception {
    public:
        /// \brief Constructor
        Error(const std::string &message,
                const std::string &component = "Unknown");

        /// \brief Destructor
        ~Error() = default;

        /// \brief Get the error message
        const std::string &getMessage() const;

        /// \brief Get the error component
        const std::string &getComponent() const;

        /// \brief Get ths error message
        const char *what() const noexcept;
    protected:
    private:
        std::string _message;
        std::string _componant;

};

#endif /* !ERROR_HPP_ */
