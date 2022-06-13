/*
** EPITECH PROJECT, 2020
** client/src/MessageItem
** File description:
** :wq
*/

#include <QDateTime>
#include "MessageItem.hpp"
#include "Log.hpp"

Ui::MessageItem::MessageItem(QWidget *parent,
        const babel::MessageObject *messageObject)
    : QWidget(parent), _messageObject(messageObject)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(nullptr);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(nullptr);
    _hBoxLayout2 = new QHBoxLayout(nullptr);

    QFont fontTimestamp;
    fontTimestamp.setBold(false);
    fontTimestamp.setPointSize(9);

    QFont fontUsername;
    fontUsername.setBold(true);
    fontUsername.setWeight(80);
    fontUsername.setPointSize(13);

    _timestampLabel = new QLabel(
            QDateTime::fromTime_t(messageObject->getMessage()
                .getTimestamp()).toString("hh:mm"));
    _timestampLabel->setFont(fontTimestamp);

    _usernameLabel = new QLabel(
            QString::fromStdString(
                messageObject->getUserObject()->getName()));
    _usernameLabel->setFont(fontUsername);

    _bodyLabel = new QLabel(
            QString::fromStdString(
                messageObject->getMessage().getBody()));
    _bodyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _bodyLabel->setTextFormat(Qt::RichText);
    _bodyLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    _bodyLabel->setOpenExternalLinks(true);

    hBoxLayout->addWidget(_timestampLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
    hBoxLayout->addWidget(_usernameLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
    hBoxLayout->addWidget(_bodyLabel);


    _imageLabel = new QLabel(QString(""));
    _imageLabel->setVisible(false);

    _hBoxLayout2->addWidget(_imageLabel);

    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addLayout(_hBoxLayout2);

    this->setLayout(vBoxLayout);

    QUrl url(QString::fromStdString(messageObject->getMessage().getBody()));
    if (url.isValid()) {
        QNetworkAccessManager *m_netwManager = new QNetworkAccessManager(this);
        QObject::connect(m_netwManager, &QNetworkAccessManager::finished, this, &Ui::MessageItem::slotnetwManagerFinished);

        QNetworkRequest request(url);
        m_netwManager->get(request);
    }

    QObject::connect(messageObject->getUserObject(), &babel::UserObject::updated,
            this, &Ui::MessageItem::slotUserUpdated);
}

void Ui::MessageItem::slotUserUpdated()
{
    _usernameLabel->setText(
        QString::fromStdString(
            this->_messageObject->getUserObject()->getName()));
}

void Ui::MessageItem::slotnetwManagerFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    if (pixmap.loadFromData(jpegData)) {
        pixmap = pixmap.scaled(_imageLabel->width(), std::min(300, pixmap.height()), Qt::KeepAspectRatio);
        _imageLabel->setPixmap(pixmap);
        _imageLabel->setVisible(true);
        _hBoxLayout2->setContentsMargins(15, 8, 0, 0);
        emit sizeHintChanged();
    }
}

