/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** contactBtn
*/

#include "UserItem.hpp"
#include "UserManager.hpp"

Ui::UserItem::UserItem(QWidget *parent, const babel::UserObject *userObject)
    : QWidget(parent), _userObject(userObject)
{
    _nameLabel = new QLabel(this);
    _nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _onlineStatusLabel = new QLabel(this);

    _horizontalLayout = new QHBoxLayout(this);

    _avatarLabel = new QLabel(this);

    _horizontalLayout->addWidget(_avatarLabel);
    _horizontalLayout->addWidget(_nameLabel);
    _horizontalLayout->addWidget(_onlineStatusLabel);


    this->setLayout(_horizontalLayout);
    this->slotUserUpdated();
    this->slotAvatarUpdated(userObject->getAvatarPixmap());
    QObject::connect(userObject, &babel::UserObject::updated,
            this, &Ui::UserItem::slotUserUpdated);
    QObject::connect(userObject, &babel::UserObject::avatarChanged,
            this, &Ui::UserItem::slotAvatarUpdated);
}

Ui::UserItem::~UserItem()
{ /* ignore */ }

void Ui::UserItem::slotUserUpdated()
{
    _nameLabel->setText(QString::fromStdString(_userObject->getName()));

    if (_userObject->getStatus() == babel::User::Offline ||
        _userObject->getVisibility() == babel::User::Visibility::Invisible) {
        _pix.load(QString(":Invisible.svg"));
        this->setToolTip(QString::fromStdString(_userObject->getName()) + _separator + tr("Offline"));
    } else if (_userObject->getVisibility() == babel::User::Visibility::Busy) {
        _pix.load(QString(":Busy.svg"));
        this->setToolTip(QString::fromStdString(_userObject->getName()) + _separator + tr("Busy"));
    } else if (_userObject->getVisibility() == babel::User::Visibility::Unavailable) {
        _pix.load(QString(":Unavailable.svg"));
        this->setToolTip(QString::fromStdString(_userObject->getName()) + _separator + tr("Unavailable"));
    } else if (_userObject->getVisibility() == babel::User::Visibility::Visible) {
        _pix.load(QString(":Visible.svg"));
        this->setToolTip(QString::fromStdString(_userObject->getName()) + _separator + tr("Online"));
    }
    _pix = _pix.scaled(_onlineStatusLabel->size(), Qt::KeepAspectRatio);
    _onlineStatusLabel->setPixmap(_pix);

    // if (_userObject->getCallStatus() ==
    //         babel::UserObject::CallStatus::InCall) {
    //     this->setStyleSheet("background-color:green;");
    // } else if (_userObject->getCallStatus() ==
    //         babel::UserObject::CallStatus::Ringing) {
    //     this->setStyleSheet("background-color:yellow;");
    // } else if (_userObject->getCallStatus() ==
    //         babel::UserObject::CallStatus::NoCall) {
    //     this->setStyleSheet("background-color:white;");
    // }
}

const babel::UserObject *Ui::UserItem::getUserObject() const
{
    return (this->_userObject);
}

void Ui::UserItem::slotAvatarUpdated(const QPixmap &pixmap)
{
    _avatarLabel->setPixmap(
        pixmap.scaled(
            _avatarLabel->size(),
            Qt::KeepAspectRatio));
}