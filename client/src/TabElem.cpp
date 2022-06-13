/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** TabElem
*/

#include "TabElem.hpp"

Ui::TabElem::TabElem(QWidget *mother, babel::User::Id id) : QWidget(mother), _id(id)
{
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setObjectName(QString::fromUtf8("_mainLayout"));
}

Ui::TabElem::~TabElem()
{
}

void Ui::TabElem::setContent(QWidget *content)
{
    _content = content;
    _mainLayout->addWidget(_content);
}

QWidget *Ui::TabElem::getContent(void)
{
    return _content;
}


babel::User::Id Ui::TabElem::getId(void) const
{
    return _id;
}
