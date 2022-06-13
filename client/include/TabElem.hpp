/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** TabElem
*/

#ifndef TABELEM_HPP
#define TABELEM_HPP

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>
#include <string>

#include "User.hpp"

namespace Ui {

    ///
    /// \brief TabElem class
    ///
    class TabElem : public QWidget {
        public:
            /// \brief Constructor
            TabElem(QWidget *mother, babel::User::Id id);
            /// \brief Destructor
            ~TabElem();

            /// \brief Set Scroll Area Content
            void setContent(QWidget *content);
            /// \brief Get Scroll Area Content
            QWidget *getContent(void);
            /// \brief Get User ID
            babel::User::Id getId(void) const;


        private:
            babel::User::Id _id;
            QWidget *_content;
            QVBoxLayout *_mainLayout;
    };
};

#endif /* !TABELEM_HPP */
