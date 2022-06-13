/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** BabelWindow
*/

#include "ui_mainwindow.h"
#include "BabelWindow.hpp"
#include <QMessageBox>

void Ui::BabelWindow::openCallWith(const babel::User::Id id,
        const babel::net::Ip &ip, const babel::net::Port &port,
        const std::string &password)
{
    try {
        DEBUG("Trying to open a call with", id);
        if (_networkVoice)
            throw Error("Already in call !");

        _networkVoice = new babel::NetworkVoice(this,
                _userWidget->getSelectedInputDevice(),
                _userWidget->getSelectedOutputDevice(),
                QHostAddress(ip == "remote" ?
                    QCoreApplication::arguments().at(1) :
                    QString::fromStdString(ip)),
                port, password);
        _networkVoice->setInputActive(!_userWidget->getButtonMuteState());
        _networkVoice->setOutputActive(!_userWidget->getButtonDeafenState());
        _contactManager->setUserCallStatus(id, babel::UserObject::InCall);
    } catch (const std::exception &ex) {
        this->_networkVoice = nullptr;

        ERROR("openCallWith", ex.what());
        this->_clientConnection->serializeAndSend(
                babel::commands::HangUpCommand(id));

        QMessageBox::critical(this,
                QString::fromUtf8("Babel - An error occured"),
                QString(ex.what()));
    }
}

void Ui::BabelWindow::clientConnectionConnected(void)
{
    babel::User::Id id = USER_ID_NONE;
    std::ifstream ifs(".babel");

    if (ifs.good())
        ifs >> id;
    _statusbar->showMessage(tr("Connected"));
    _clientConnection->serializeAndSend(babel::commands::IdentifyCommand(id));
}

void Ui::BabelWindow::clientConnectionDisconnected(void)
{
    _statusbar->showMessage(tr("Disonnected") +
            " (" + _clientConnection->errorString() + ")");
}

void Ui::BabelWindow::clientConnectionErrorOccurred(void)
{
    QMessageBox::critical(this,
            QString::fromUtf8("Babel - An error occured with the Server Connection"),
            _clientConnection->errorString());
}

void Ui::BabelWindow::slotSelfEvent(const babel::events::SelfEvent &e)
{
    std::ofstream ofs(".babel");

    if (ofs.good())
        ofs << e.getUserId();
    ofs.close();
}

void Ui::BabelWindow::slotCallEvent(const babel::events::CallEvent &e)
{
    if (e.getFromUserId() != _contactManager->getSelf().getId()) {
        _contactManager->setUserCallStatus(e.getFromUserId(), babel::UserObject::Ringing);
        _contactManager->setUserAudioNetwork(e.getFromUserId(), e.getIp(), e.getPort(), e.getPassword());
    } else {
        this->openCallWith(e.getToUserId(), e.getIp(), e.getPort(), e.getPassword());
    }
}

void Ui::BabelWindow::slotHangUpEvent(const babel::events::HangUpEvent &e)
{
    babel::User::Id contact = e.getFromUserId();

    if (e.getFromUserId() == _contactManager->getSelf().getId())
        contact = e.getToUserId();
    DEBUG("Hangup");
    if (_contactManager->getUserCallStatus(contact) == babel::UserObject::InCall) {
        delete _networkVoice;
        _networkVoice = nullptr;
    }
    _contactManager->setUserCallStatus(contact, babel::UserObject::NoCall);
}

void Ui::BabelWindow::slotMessageEvent(const babel::events::MessageEvent &e)
{
    babel::User::Id fromId = e.getFromUserId();
    babel::User::Id intoId = e.getToUserId();

    if (intoId == this->_contactManager->getSelf().getId())
        intoId = fromId;
    this->_contactManager->addMessage(intoId, fromId,
            babel::Message(e.getTimestamp(), e.getBody()));
}

void Ui::BabelWindow::slotErrorEvent(const babel::events::ErrorEvent &e)
{
    QMessageBox::critical(this, QString::fromUtf8("Babel - An error occured"),
            QString::fromStdString(e.getMessage()));
}

Ui::BabelWindow::BabelWindow(const QString &serverIp, bool unsafeCert) {
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("this"));
    _centralwidget = new QWidget(this);
    _centralwidget->setObjectName(QString::fromUtf8("_centralwidget"));
    _horizontalLayout = new QHBoxLayout(_centralwidget);
    _horizontalLayout->setObjectName(QString::fromUtf8("_horizontalLayout"));


    _verticalLayout = new QVBoxLayout();
    _verticalLayout->setObjectName(QString::fromUtf8("_verticalLayout"));

    _menubar = new QMenuBar(this);
    _menubar->setObjectName(QString::fromUtf8("_menubar"));
    _menubar->setGeometry(QRect(0, 0, 1001, 25));
    _statusbar = new QStatusBar(this);
    _statusbar->setObjectName(QString::fromUtf8("_statusbar"));
    _statusbar->showMessage(QString::fromUtf8("Disonnected"));


    _pa = std::make_shared<babel::PortAudio>();

    _contacts = new Ui::ContactList(_centralwidget);
    _contactManager = new babel::UserManager(this);
    _userWidget = new Ui::UserWidget(_centralwidget, _pa);
    _verticalLayout->addWidget(_userWidget, 1);
    _verticalLayout->addWidget(_contacts, 3);
    _horizontalLayout->addLayout(_verticalLayout, 1);
    _centralTab = new babel::TabManager(_centralwidget, _horizontalLayout);

    _clientConnection = new babel::ClientConnection(_statusbar);
    _clientConnection->setUnsafeCert(unsafeCert);
    _clientConnection->connectToHostEncrypted(serverIp, 5000);

    this->resize(1001, 611);
    this->setMenuBar(_menubar);
    this->setStatusBar(_statusbar);
    this->setCentralWidget(_centralwidget);
    this->setWindowTitle(QCoreApplication::translate("BabelWindow", "Babel", nullptr));

    QObject::connect(_contactManager, &babel::UserManager::signalSelfUpdated,
            _userWidget, &Ui::UserWidget::slotSelfUpdated);
    QObject::connect(_contactManager, &babel::UserManager::signalSelfAvatarUpdated,
            _userWidget, &Ui::UserWidget::slotSelfAvatarUpdated);
    QObject::connect(_contactManager, &babel::UserManager::signalUserCreated,
            _contacts, &Ui::ContactList::slotUserCreated);
    QObject::connect(_contacts, &Ui::ContactList::signalUserClicked,
            _centralTab, &babel::TabManager::slotOpenUser);

    QObject::connect(_clientConnection, &babel::ClientConnection::handleUserEvent,
            _contactManager, &babel::UserManager::slotUserEvent);
    QObject::connect(_clientConnection, &babel::ClientConnection::handleSelfEvent,
            _contactManager, &babel::UserManager::slotSelfEvent);

    QObject::connect(_clientConnection, &babel::ClientConnection::handleSelfEvent,
        this, &Ui::BabelWindow::slotSelfEvent);
    QObject::connect(_clientConnection, &babel::ClientConnection::handleCallEvent,
        this, &Ui::BabelWindow::slotCallEvent);
    QObject::connect(_clientConnection, &babel::ClientConnection::handleHangUpEvent,
        this, &Ui::BabelWindow::slotHangUpEvent);
    QObject::connect(_clientConnection, &babel::ClientConnection::handleMessageEvent,
        this, &Ui::BabelWindow::slotMessageEvent);
    QObject::connect(_clientConnection, &babel::ClientConnection::handleErrorEvent,
        this, &Ui::BabelWindow::slotErrorEvent);
    QObject::connect(_clientConnection, &babel::ClientConnection::connected,
        this, &Ui::BabelWindow::clientConnectionConnected);
    QObject::connect(_clientConnection, &babel::ClientConnection::disconnected,
        this, &Ui::BabelWindow::clientConnectionDisconnected);
    QObject::connect(_clientConnection, &babel::ClientConnection::errorOccurred,
        this, &Ui::BabelWindow::clientConnectionErrorOccurred);

    QObject::connect(_centralTab, &babel::TabManager::signalCallUser,
            [this] (babel::User::Id userId) {
                this->_clientConnection->serializeAndSend(
                        babel::commands::CallCommand(userId));
            });
    QObject::connect(_centralTab, &babel::TabManager::signalPickUpUser,
            [this] (babel::User::Id userId) {
                openCallWith(userId,
                        _contactManager->getUserAudioIp(userId),
                        _contactManager->getUserAudioPort(userId),
                        _contactManager->getUserAudioPassword(userId));
            });
    QObject::connect(_centralTab, &babel::TabManager::signalHangUpUser,
            [this] (babel::User::Id userId) {
                this->_clientConnection->serializeAndSend(
                        babel::commands::HangUpCommand(userId));
            });
    QObject::connect(_centralTab, &babel::TabManager::signalMessageUser,
            [this] (babel::User::Id userId, babel::Message::Body body) {
                this->_clientConnection->serializeAndSend(
                        babel::commands::MessageCommand(userId, body));
            });
    QObject::connect(_userWidget, &Ui::UserWidget::signalSelfRenamed,
            [this] (babel::User::Name newName) {
                this->_clientConnection->serializeAndSend(
                        babel::commands::UpdateCommand(
                            newName,
                            _contactManager->getSelf().getAvatar(),
                            _contactManager->getSelf().getVisibility()));
            });
    QObject::connect(_userWidget, &Ui::UserWidget::signalSelfAvatarChanged,
            [this] (babel::User::Avatar newAvatar) {
                this->_clientConnection->serializeAndSend(
                        babel::commands::UpdateCommand(
                            _contactManager->getSelf().getName(),
                            newAvatar,
                            _contactManager->getSelf().getVisibility()));
            });
    QObject::connect(_userWidget, &Ui::UserWidget::signalSelfVisibilityChanged,
            [this] (babel::User::Visibility newVisibility) {
                this->_clientConnection->serializeAndSend(
                        babel::commands::UpdateCommand(
                            _contactManager->getSelf().getName(),
                            _contactManager->getSelf().getAvatar(),
                            newVisibility));
            });
    QObject::connect(_userWidget, &Ui::UserWidget::signalMuteStateChanged,
            [this] (bool checked) {
                if (this->_networkVoice)
                    this->_networkVoice->setInputActive(!checked);
            });
    QObject::connect(_userWidget, &Ui::UserWidget::signalDeafenStateChanged,
            [this] (bool checked) {
                if (this->_networkVoice)
                    this->_networkVoice->setOutputActive(!checked);
            });

    QMetaObject::connectSlotsByName(this);
}
