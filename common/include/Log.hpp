/*
** EPITECH PROJECT, 2018
** Log.hp
** File description:
** Log system
*/

#ifndef LOG_HPP_
#define LOG_HPP_

#include <iostream>
#include <fstream>
#include <sstream>

#define DEFAULT_FG "\e[39m"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/// \brief Define for the INFO log
#define INFO(...) Log::log("\e[48;5;37m INFO \e[0m", __FILENAME__, __LINE__, __FUNCTION__, std::cout, __VA_ARGS__)
/// \brief Define for the ERROR log
#define ERROR(...) Log::log("\e[101m ERR  \e[0m\e[91m", __FILENAME__, __LINE__, __FUNCTION__, std::cerr, __VA_ARGS__)
/// \brief Define for the WARNING log
#define WARN(...) Log::log("\e[48;5;135m WARN \e[0m", __FILENAME__, __LINE__, __FUNCTION__, std::cout, __VA_ARGS__)
#define WARN(...) Log::log("\e[48;5;135m WARN \e[0m", __FILENAME__, __LINE__, __FUNCTION__, std::cout, __VA_ARGS__)

#ifndef NDEBUG
    /// \brief Define for the DEBUG warning
    #define DEBUG(...) Log::log("\e[48;5;242m DBUG \e[0m", __FILENAME__, __LINE__, __FUNCTION__, std::cout, __VA_ARGS__)
#else
    #define DEBUG(...)
#endif

///
/// \brief Log class, used int the macros INFO, ERROR, WARN and DEBUG
///
class Log
{
    public :

        /// \brief log function, used byt the defines
        template <typename... Types>
        static void log(const char *name, const char *file, int line, const char *caller, std::ostream &os, Types... var)
        {
            os << name;
            getInfos(os, file, line, caller);
            insertToSstream(os, var...);
            os << DEFAULT_FG;
            os.flush();
        }

    private :
        /// \brief Gets all of the informations by the defines
        static std::ostream &getInfos(std::ostream &, const char *, int, const char *);
        /// \brief Insert into a stream
        static std::ostream &insertToSstream(std::ostream &ss)
        {
            ss << "\n";
            return ss;
        }

        /// \brief Insert into a stream
        template <typename T, typename... Types>
        static std::ostream &insertToSstream(std::ostream &ss, T var1, Types... var2)
        {
            ss << ' ' << var1;

            return insertToSstream(ss, var2...);
        }
};

#endif
