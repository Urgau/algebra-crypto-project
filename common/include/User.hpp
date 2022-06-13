/*
** EPITECH PROJECT, 2020
** common/include/User
** File description:
** well, real programmers use ed
*/

#ifndef USER_HPP_
#define USER_HPP_

#define USER_ID_NONE 0
#define USER_ID_START 1
#define USER_ID_END 65535

#include <string>

namespace babel {
    namespace events {
        class SelfEvent;
        class UserEvent;
    }

    class User {
        public:
            /// \brief Id type for User
            using Id = unsigned short int;

            /// \brief Name type for User
            using Name = std::string;

            /// \brief Avatar type for User
            using Avatar = std::string;

            /// \brief Status enum for User
            enum Status {
                Online = 0,
                Offline = 1
            };

            /// \brief Visibility enum for User
            enum Visibility {
                Visible = 0,
                Unavailable = 1,
                Busy = 2,
                Invisible = 3
            };

            /// \brief Empty
            User();

            /// \brief User constructor
            User(User::Id id,
                    User::Name name,
                    User::Avatar avater,
                    User::Visibility visibility);

            /// \brief Destructor
            ~User() = default;

            /// \brief Get id
            User::Id getId() const;

            /// \brief Get name
            User::Name getName() const;

            /// \brief Get name
            User::Avatar getAvatar() const;

            // \brief Get visibility
            User::Visibility getVisibility() const;

            /// \brief Set name
            void setName(const User::Name &name);

            /// \brief Set avatar
            void setAvatar(const User::Avatar &avatar);

            /// \brief Set status
            void setVisibility(const User::Visibility visibility);

            /// \brief Transform User to SelfEvent
            const events::SelfEvent toSelfEvent() const;

            /// \brief Transform User to UserEvent
            const events::UserEvent toUserEvent(Status status) const;

            /// \brief Operate ==
            bool operator ==(const User &other) const;

            /// \brief Operate ==
            bool operator ==(const User::Id &id) const;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int)
            {
                ar & _id;
                ar & _name;
                ar & _avatar;
                ar & _visibility;
            }
        private:
            User::Id _id;
            User::Name _name;
            User::Avatar _avatar;
            User::Visibility _visibility;
    };
}

#endif
