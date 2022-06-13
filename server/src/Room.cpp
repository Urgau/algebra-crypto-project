/*
** EPITECH PROJECT, 2022
** server/src/Room
** File description:
** code on paper!
*/

#include <random>
#include "Room.hpp"

namespace babel {

    Room::Room(ProxyPtr proxy) : _proxy(proxy)
    { /* ignore */ }

    const ProxyPtr Room::proxy() const
    {
        return (this->_proxy);
    }

    void Room::join(SessionPtr session)
    {
        this->_sessions.insert(session);
        session->setRoom(*this);
    }

    void Room::leave(SessionPtr session)
    {
        this->_sessions.erase(session);
        session->clearRoom();
    }

    const std::set<SessionPtr> &Room::sessions() const
    {
        return (this->_sessions);
    }

    void Room::salt(int salt)
    {
        static auto& chrs = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::mt19937 mt{std::random_device{}()};
        std::uniform_int_distribution<unsigned short int> dist(0, sizeof(chrs) - 2);

        this->_password.clear();
        this->_password.reserve(32);

        for (int i = 0; i < 32; i++)
            this->_password += chrs[dist(mt)];
        (void)salt;
    }

    const std::string &Room::password() const
    {
        return (this->_password);
    }

}
