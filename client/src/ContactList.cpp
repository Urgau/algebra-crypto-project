/*
** EPITECH PROJECT, 2020
** client/src/ContactList
** File description:
** epitech > 42
*/

#include "ContactList.hpp"
#include "UserItem.hpp"

Ui::ContactList::ContactList(QWidget *parent) :
    QGroupBox(tr("Users"), parent)
{
    setupUi(this);
    retranslateUi(this);
};

void Ui::ContactList::setupUi(QWidget *parent)
{
    if (parent->objectName().isEmpty())
        parent->setObjectName(QString::fromUtf8("ContactList"));

    _listWidget = new QListWidget(parent);
    _listWidget->setObjectName(QString::fromUtf8("_listWidget"));

    _gridLayout = new QGridLayout(parent);
    _gridLayout->addWidget(_listWidget, 0, 0);

    parent->setLayout(_gridLayout);

    QObject::connect(_listWidget, &QListWidget::itemClicked,
            [this] (QListWidgetItem *item) {
        Ui::UserItem *userItem = dynamic_cast<Ui::UserItem*>(
                this->_listWidget->itemWidget(item));
        emit signalUserClicked(userItem->getUserObject());
    });
}

void Ui::ContactList::slotUserCreated(const babel::UserObject *userObject)
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem(_listWidget);
    Ui::UserItem *userItem = new Ui::UserItem(_listWidget, userObject);

    listWidgetItem->setSizeHint(userItem->sizeHint());

    _listWidget->addItem(listWidgetItem);
    _listWidget->setItemWidget(listWidgetItem, userItem);
}

void Ui::ContactList::retranslateUi(QWidget *parent)
{
    parent->setWindowTitle(QCoreApplication::translate("ContactList", "Form", nullptr));
}
