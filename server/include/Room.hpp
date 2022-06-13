/*
** EPITECH PROJECT, 2022
** server/include/Room
** File description:
** 01100010 01101001 01101110 01100001 01110010 01111001
*/

#ifndef CALL_HPP_
#define CALL_HPP_

#include <set>
#include "Session.hpp"
#include "Proxy.hpp"

namespace babel {

    class Room : public std::enable_shared_from_this<Room>
    {
        public:
            Room(ProxyPtr proxy);

            ~Room() = default;

            void join(SessionPtr session);

            void leave(SessionPtr session);

            void salt(int salt);

            const ProxyPtr proxy() const;

            const std::string &password() const;

            const std::set<SessionPtr> &sessions() const;

        private:
            ProxyPtr _proxy;
            std::string _password;
            std::set<SessionPtr> _sessions;
    };

    typedef std::shared_ptr<Room> RoomPtr;

}

#endif
