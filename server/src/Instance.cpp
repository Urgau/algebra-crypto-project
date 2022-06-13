/*
** EPITECH PROJECT, 2020
** server/src/Instance
** File description:
** M-x doctor
*/

#include <random>
#include <fstream>
#include <iostream>
#include <boost/serialization/map.hpp>
#include "Instance.hpp"
#include "Room.hpp"
#include "Log.hpp"

namespace babel {

    Instance::Instance()
    {
        std::ifstream ifs(DATABASE_PATH);

        if (ifs.good()) {
            INFO("Loading database...");
            boost::archive::text_iarchive ia(ifs);
            ia >> this->_users;
        }
    }

    Instance::~Instance()
    {
        std::ofstream ofs(DATABASE_PATH);

        if (ofs.good()) {
            INFO("Saving database...");
            boost::archive::text_oarchive oa(ofs);
            oa << this->_users;
        } else {
            ERROR("Cannot save the database...");
        }
    }

    void Instance::join(SessionPtr session)
    {
        _sessions.insert(session);
    }

    void Instance::leave(SessionPtr session)
    {
        _sessions.erase(session);

        if (session->hasUser()) {
            const User &selfUser = session->getUser();
            this->notifyUserChangeToUsers(selfUser, User::Status::Offline);

            if (session->hasRoom()) {
                Room &room = session->getRoom();
                room.leave(session);

                auto sessions(room.sessions());
                if (sessions.size() == 1) {
                    for (auto roomSession : sessions) {
                        const User &roomUser = roomSession->getUser();
                        this->sendMessageFromTo(
                            selfUser.getId(), roomUser.getId(),
                            events::HangUpEvent(selfUser.getId(), roomUser.getId()));
                        room.leave(roomSession);
                    }
                }
            }
        }
    }

    void Instance::join(ProxyPtr proxy)
    {
        _rooms.insert(std::make_shared<babel::Room>(proxy));
    }

    void Instance::handleCommand(SessionPtr session, std::istream &input) noexcept(true)
    {
        try {
            if (input.eof())
                throw Error("EOF on input stream", "Instance::handleCommand");

            std::string commandName;
            input >> commandName;

            if (commandName.empty())
                throw Error("Empty command name", "Instance::handleCommand");

            DEBUG("Session (", session, "): ", commandName, "...");

            try {
                (*this.*HANDLERS.at(commandName))(session, input);
            } catch (const std::out_of_range &) {
                throw Error("Invalid command name", "Instance::handleCommand");
            }
        } catch (const Error &error) {
            ERROR("Session (", session, "): ", error.getComponent(), " => ", error.getMessage());
            this->serializeAndSend(session, events::ErrorEvent(error.getMessage()));
        }
    }

    void Instance::notifyUserChangeToUsers(const User &user, User::Status status)
    {
        const events::UserEvent userEvent = user.toUserEvent(status);
        const events::SelfEvent selfEvent = user.toSelfEvent();

        for (const SessionPtr &session : this->_sessions) {
            if (!session->hasUser())
                continue;

            if (session->getUser() == user)
                this->serializeAndSend(session, selfEvent);
            else
                this->serializeAndSend(session, userEvent);
        }
    }

    void Instance::handleIdentifyCommand(SessionPtr session, const commands::IdentifyCommand &identifyCommand)
    {
        if (identifyCommand.getUserId() == USER_ID_NONE) {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<unsigned short int> dist(USER_ID_START, USER_ID_END);

            User::Id userId = dist(mt);
            User user = User(userId,
                    std::string("User#") + std::to_string(userId),
                    std::string("default"),
                    User::Visibility::Visible);

            this->_users[userId] = user;
            session->setUser(this->_users.at(user.getId()));
            this->sendUsersList(session);
            this->notifyUserChangeToUsers(user);
        } else {
            try {
                User &user = this->_users.at(identifyCommand.getUserId());

                session->setUser(user);
                this->sendUsersList(session);
                this->notifyUserChangeToUsers(user);
            } catch (const std::out_of_range &) {
                this->serializeAndSend(session, events::ErrorEvent(
                            std::string("User ") +
                            std::to_string(identifyCommand.getUserId()) +
                            std::string(" doesn't exist; Nit: `.babel` store the id of your current user")));
            }
        }
    }

    void Instance::handleUpdateCommand(SessionPtr session, const commands::UpdateCommand &updateCommand)
    {
        User &user = session->getUser();

        if (!updateCommand.getUsername().empty()) {
            user.setName(updateCommand.getUsername());
            user.setAvatar(updateCommand.getAvatar());
            user.setVisibility(updateCommand.getVisibility());

            this->notifyUserChangeToUsers(user);
        } else {
            this->serializeAndSend(session,
                    events::ErrorEvent("Cannot set an empty username!"));
        }
    }

    void Instance::sendUsersList(SessionPtr session, std::size_t maxUsers)
    {
        const User &selfUser = session->getUser();

        if (maxUsers <= 0)
            maxUsers = 1000;

        std::size_t index = 0;
        for (const auto &[key, otherUser] : this->_users) {
            if (++index == maxUsers)
                break;
            if (otherUser == selfUser)
                continue;

            User::Status status = User::Status::Offline;
            for (const SessionPtr &session : this->_sessions) {
                if (session->hasUser() && session->getUser() == otherUser) {
                    status = User::Status::Online;
                    break;
                }
            }

            this->serializeAndSend(session, otherUser.toUserEvent(status));
        }
    }

    void Instance::handleUsersCommand(SessionPtr session, const commands::UsersCommand &usersCommand)
    {
        this->sendUsersList(session, usersCommand.getMaxUsers());
    }

    static bool isLocalIpV4(const std::string &ip)
    {
        return (ip.rfind("10.", 0) == 0 || ip.rfind("192.168.", 0) == 0);
    }

    void Instance::handleCallCommand(SessionPtr session, const commands::CallCommand &callCommand)
    {
        const User &selfUser = session->getUser();

        if (selfUser.getId() == callCommand.getUserId()) {
            this->serializeAndSend(session,
                    events::ErrorEvent("Cannot make a call to himself"));
        } else {
            bool foundUser = false;
            for (SessionPtr otherSession : this->_sessions) {
                if (otherSession->hasUser() &&
                        otherSession->getUser() == callCommand.getUserId()) {
                    foundUser = true;

                    bool areLocalIps =
                        isLocalIpV4(otherSession->getRemoteIp()) &&
                        isLocalIpV4(session->getRemoteIp());

                    if (areLocalIps && otherSession->getRemoteIp() ==
                            session->getRemoteIp()) {
                        this->serializeAndSend(session,
                                events::ErrorEvent("Cannot make a call local"
                                    "from and to the same PC!"));
                        break;
                    }

                    bool foundRoom = false;
                    for (auto &room : this->_rooms) {
                        if (!room->sessions().empty())
                            continue;

                        foundRoom = true;
                        room->join(session);
                        room->join(otherSession);
                        room->salt(3114);

                        auto audioPort = room->proxy()->endpoint().port();
                        INFO("Call on:", room->proxy()->endpoint(),
                                "(pwd:", room->password(), ")");

                        this->serializeAndSend(session,
                                events::CallEvent(selfUser.getId(), callCommand.getUserId(),
                                    areLocalIps ? otherSession->getRemoteIp() : "remote",
                                    areLocalIps ? LOCAL_AUDIO_PORT : audioPort,
                                    room->password()));
                        this->serializeAndSend(otherSession,
                                events::CallEvent(selfUser.getId(), callCommand.getUserId(),
                                    areLocalIps ? session->getRemoteIp() : "remote",
                                    areLocalIps ? LOCAL_AUDIO_PORT : audioPort,
                                    room->password()));

                        break;
                    }

                    if (!foundRoom) {
                        WARN("No room found, cancelling the call!");
                        break;
                    }
                }
            }

            if (!foundUser) {
                this->serializeAndSend(session,
                        events::ErrorEvent("Unable to make call to user!\nUser not found!"));
            }
        }
    }

    void Instance::handleHangUpCommand(SessionPtr session, const commands::HangUpCommand &hangUpCommand)
    {
        const User &selfUser = session->getUser();

        if (selfUser.getId() == hangUpCommand.getUserId()) {
            this->serializeAndSend(session,
                    events::ErrorEvent("Cannot hang up to himself"));
        } else {
            Room &room = session->getRoom();
            room.leave(session);

            auto sessions(room.sessions());
            if (sessions.size() == 1) {
                for (auto roomSession : sessions) {
                    const User &roomUser = roomSession->getUser();
                    this->sendMessageFromTo(
                        selfUser.getId(), roomUser.getId(),
                        events::HangUpEvent(selfUser.getId(), roomUser.getId()));
                    room.leave(roomSession);
                }
            }
        }
    }

    void Instance::handleMessageCommand(SessionPtr session, const commands::MessageCommand &messageCommand)
    {
        const User &selfUser = session->getUser();

        if (selfUser.getId() == messageCommand.getUserId()) {
            this->serializeAndSend(session,
                    events::ErrorEvent("Cannot send message to himself"));
        } else if (!this->sendMessageFromTo(
                    selfUser.getId(), messageCommand.getUserId(),
                    events::MessageEvent(selfUser.getId(), messageCommand.getUserId(),
                        std::time(nullptr), messageCommand.getBody()))) {
            this->serializeAndSend(session,
                    events::ErrorEvent("Unable to send MessageEvent to id: " +
                        std::to_string(messageCommand.getUserId())));
        }
    }
}
