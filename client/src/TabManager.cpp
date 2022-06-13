/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** TabManager
*/

#include <QObject>

#include "TabManager.hpp"
#include "UserPage.hpp"

babel::TabManager::TabManager(QWidget *mother, QHBoxLayout *layout) : QObject(mother)
{
    _tab = new QTabWidget(mother);
    _tab->setObjectName(QString::fromUtf8("tab"));
    _tab->setTabsClosable(true);
    _tab->setMovable(true);
    layout->addWidget(_tab, 3);

    QObject::connect(_tab, &QTabWidget::tabCloseRequested,
            this, &babel::TabManager::slotCloseTab);
}

void babel::TabManager::addContactTab(const UserObject *userObject)
{
    if (switchIfTabExist(userObject->getId()))
        return;
    Ui::TabElem *tab = new Ui::TabElem(_tab, userObject->getId());
    Ui::UserPage *call = new Ui::UserPage(tab, userObject);

    tab->setContent(call);
    _tab->addTab(tab, QString::fromStdString(userObject->getName()));
    _tab->setCurrentWidget(tab);
    _tab->setTabIcon(_tab->indexOf(tab), QIcon(userObject->getAvatarPixmap()));
    _tabsUserId[userObject->getId()] = tab;

    QObject::connect(call, &Ui::UserPage::signalCallUser,
            this, &babel::TabManager::slotCallUser);
    QObject::connect(call, &Ui::UserPage::signalPickUpUser,
            this, &babel::TabManager::signalPickUpUser);
    QObject::connect(call, &Ui::UserPage::signalHangUpUser,
            this, &babel::TabManager::slotHangUpUser);
    QObject::connect(call, &Ui::UserPage::signalMessageUser,
            this, &babel::TabManager::slotMessageUser);
    QObject::connect(userObject, &babel::UserObject::updated,
            [this, tab, userObject] {
        this->_tab->setTabText(
                this->_tab->indexOf(tab),
                QString::fromStdString(userObject->getName()));
    });
    QObject::connect(userObject, &babel::UserObject::avatarChanged,
            [this, tab] (const QPixmap &pixmap) {
        this->_tab->setTabIcon(
                this->_tab->indexOf(tab),
                QIcon(pixmap));
    });
}

void babel::TabManager::slotCloseTab(int index)
{
    Ui::TabElem *tab;

    if ((tab = dynamic_cast<Ui::TabElem *>(_tab->widget(index)))) {
        _tabsUserId.erase(tab->getId());
        _tab->removeTab(index);
    }
}

void babel::TabManager::slotOpenUser(const UserObject *userData)
{
    this->addContactTab(userData);
}

bool babel::TabManager::switchIfTabExist(User::Id id)
{
    if (_tabsUserId.count(id)) {
        _tab->setCurrentWidget(_tabsUserId[id]);
        return (true);
    }
    return (false);
}

void babel::TabManager::slotCallUser(User::Id id)
{
    emit signalCallUser(id);
}

void babel::TabManager::slotPickUpUser(User::Id id)
{
    emit signalPickUpUser(id);
}

void babel::TabManager::slotHangUpUser(User::Id id)
{
    emit signalHangUpUser(id);
}

void babel::TabManager::slotMessageUser(User::Id id, Message::Body body)
{
    emit signalMessageUser(id, body);
}
