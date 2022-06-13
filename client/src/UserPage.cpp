/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserPage
*/

#include "UserPage.hpp"
#include "Log.hpp"

void Ui::UserPage::setupUi(QWidget *UserPage)
{
    if (UserPage->objectName().isEmpty())
        UserPage->setObjectName(QString::fromUtf8("UserPage"));
    UserPage->resize(983, 524);
    _contactCallLayout = new QVBoxLayout(UserPage);
    _contactCallLayout->setObjectName(QString::fromUtf8("_contactCallLayout"));
    _topButtonsLayout = new QHBoxLayout();
    _topButtonsLayout->setObjectName(QString::fromUtf8("_topButtonsLayout"));
    _button2 = new QPushButton(UserPage);
    _button2->setObjectName(QString::fromUtf8("_button2"));

    _topButtonsLayout->addWidget(_button2);

    _button1 = new QPushButton(UserPage);
    _button1->setObjectName(QString::fromUtf8("_button1"));

    _topButtonsLayout->addWidget(_button1);

    _contactCallLayout->addLayout(_topButtonsLayout);

    _messageList = new QListWidget(UserPage);
    _messageList->setObjectName(QString::fromUtf8("_messageList"));
    _messageList->setSelectionMode(QAbstractItemView::NoSelection);

    _contactCallLayout->addWidget(_messageList);

    _newMessageLayout = new QHBoxLayout();
    _newMessageLayout->setObjectName(QString::fromUtf8("_newMessageLayout"));

    _messageContent = new QLineEdit(UserPage);
    _messageContent->setObjectName(QString::fromUtf8("_messageContent"));

    _newMessageLayout->addWidget(_messageContent);

    _sendButton = new QPushButton(UserPage);
    _sendButton->setObjectName(QString::fromUtf8("_sendButton"));
    _sendButton->setIcon(QIcon(QString(":Send.svg")));
    _sendButton->setToolTip(tr("Send"));

    _newMessageLayout->addWidget(_sendButton);

    _contactCallLayout->addLayout(_newMessageLayout);

    _contactCallLayout->setStretch(0, 1);
    _contactCallLayout->setStretch(1, 14);
    _contactCallLayout->setStretch(2, 1);

    _button2->setStyleSheet("background-color:red;");
    _button2->setText(QString("Refuse"));

    QMetaObject::connectSlotsByName(UserPage);
}

void Ui::UserPage::slotUserUpdated()
{
    _button1->setDisabled(false);
    _button2->setVisible(false);
    if (this->_userObject->getCallStatus() ==
            babel::UserObject::CallStatus::NoCall) {
        _button1->setText(tr("Call"));
        _button1->setStyleSheet("background-color:green;");
        if (this->_userObject->getVisibility() ==
                babel::User::Visibility::Unavailable)
            _button1->setDisabled(true);
    } else if (this->_userObject->getCallStatus() ==
            babel::UserObject::CallStatus::Ringing) {
        _button1->setStyleSheet("background-color:green;");
        _button1->setText(tr("Accept"));
        _button2->setVisible(true);
    } else if (this->_userObject->getCallStatus() ==
            babel::UserObject::CallStatus::InCall) {
        _button1->setStyleSheet("background-color:red;");
        _button1->setText(tr("Hang Up"));
    }
}

Ui::UserPage::UserPage(QWidget *parent,
        const babel::UserObject *userObject)
    : QWidget(parent), _userObject(userObject)
{
    setupUi(this);
    slotUserUpdated();

    for (const babel::MessageObject *messageObject :
            this->_userObject->getMessages())
        this->slotNewMessage(messageObject);

    QObject::connect(_button1, &QPushButton::clicked,
            this, &UserPage::slotCallButtonPush);
    QObject::connect(_button2, &QPushButton::clicked,
            this, &UserPage::slotHangUpButtonPush);
    QObject::connect(_sendButton, &QPushButton::clicked,
            this, &UserPage::slotSendMessage);
    QObject::connect(_messageContent, &QLineEdit::returnPressed,
            this, &UserPage::slotSendMessage);
    QObject::connect(_userObject, &babel::UserObject::updated,
            this, &UserPage::slotUserUpdated);
    QObject::connect(_userObject, &babel::UserObject::newMessage,
            this, &UserPage::slotNewMessage);
}

void Ui::UserPage::slotNewMessage(const babel::MessageObject *messageObject)
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem(this->_messageList);
    Ui::MessageItem *messageItem = new Ui::MessageItem(this->_messageList, messageObject);

    listWidgetItem->setSizeHint(messageItem->sizeHint());
    QObject::connect(messageItem, &Ui::MessageItem::sizeHintChanged, [listWidgetItem, messageItem, this] {
        listWidgetItem->setSizeHint(messageItem->sizeHint());
        _messageList->scrollToItem(listWidgetItem);
    });

    this->_messageList->addItem(listWidgetItem);
    this->_messageList->setItemWidget(listWidgetItem, messageItem);
    this->_messageList->scrollToItem(listWidgetItem);
}

void Ui::UserPage::slotCallButtonPush(void)
{
    if (this->_userObject->getCallStatus() ==
            babel::UserObject::CallStatus::NoCall)
        emit signalCallUser(this->_userObject->getId());
    else if (this->_userObject->getCallStatus() ==
            babel::UserObject::CallStatus::Ringing)
        emit signalPickUpUser(this->_userObject->getId());
    else if (this->_userObject->getCallStatus() ==
            babel::UserObject::CallStatus::InCall)
        emit signalHangUpUser(this->_userObject->getId());
}

void Ui::UserPage::slotHangUpButtonPush(void)
{
    emit signalHangUpUser(this->_userObject->getId());
}

void Ui::UserPage::slotSendMessage(void)
{
    if (!this->_messageContent->text().isEmpty()) {
        emit signalMessageUser(this->_userObject->getId(),
                this->_messageContent->text().toStdString());
        this->_messageContent->setText("");
    }
}
