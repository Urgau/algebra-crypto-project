/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserWidget
*/

#include "Log.hpp"
#include "UserWidget.hpp"
#include <QtWidgets/QRadioButton>

Ui::UserWidget::UserWidget(QWidget *parent, std::shared_ptr<babel::PortAudio> &pa)
    : QGroupBox(tr("Personnal informations"), parent), _curentVisibility(0)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(nullptr);
    QHBoxLayout *hBoxLayout2 = new QHBoxLayout(nullptr);
    QHBoxLayout *hBoxLayout3 = new QHBoxLayout(nullptr);

    _usernameStackedWidget = new QStackedWidget();
    _usernameStackedWidget->setObjectName(QString::fromUtf8("_stackedWidget"));

    _userNameLabel = new ClickableLabel();
    _userNameLabel->setObjectName(QString::fromUtf8("_userNameLabel"));
    _userNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _userNameEditLineEdit = new QLineEdit();
    _userNameEditLineEdit->setObjectName(QString::fromUtf8("_userNameEditLineEdit"));
    _userNameEditLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _userVisibilityComboBox = new QComboBox();
    _userVisibilityComboBox->setObjectName(QString::fromUtf8("_userVisibilityComboBox"));
    _userVisibilityComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _settingButton = new QPushButton();
    _settingButton->setObjectName(QString::fromUtf8("_settingButton"));
    _settingButton->setIcon(QIcon(QString(":Settings.svg")));
    _settingButton->setVisible(false);

    _muteButton = new QPushButton();
    _muteButton->setObjectName(QString::fromUtf8("_muteButton"));
    _muteButton->setIcon(QIcon(QString(":AudioInput.svg")));
    _muteButton->setCheckable(true);

    _deafenButton = new QPushButton();
    _deafenButton->setObjectName(QString::fromUtf8("_deafenButton"));
    _deafenButton->setIcon(QIcon(QString(":AudioOutput.svg")));
    _deafenButton->setCheckable(true);

    _avatarStackedWidget = new QStackedWidget();
    _avatarStackedWidget->setObjectName(QString::fromUtf8("_avatarWidget"));

    _avatarLabel = new ClickableLabel();
    _avatarLabel->setObjectName(QString::fromUtf8("_avatarLabel"));
    _avatarLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _avatarLineEdit = new QLineEdit();
    _avatarLineEdit->setObjectName(QString::fromUtf8("_avatarLineEdit"));
    _avatarLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _audioInputComboBox = new QComboBox();
    _audioInputComboBox->setObjectName(QString::fromUtf8("_audioInputComboBox"));
    _audioInputComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _audioInputComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    _audioInputComboBox->setMinimumContentsLength(8);

    _audioOutputComboBox = new QComboBox();
    _audioOutputComboBox->setObjectName(QString::fromUtf8("_audioOutputComboBox"));
    _audioOutputComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _audioOutputComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    _audioOutputComboBox->setMinimumContentsLength(8);

    QPixmap pix;


    _audioInputLabel = new QLabel();
    _audioInputLabel->setObjectName(QString::fromUtf8("_audioInputLabel"));
    _audioInputLabel->setText(tr("Input"));
    if (pix.load(QString(":AudioInput.svg"))) {
        pix = pix.scaled(20, 20, Qt::KeepAspectRatio);
        _audioInputLabel->setPixmap(pix);
    }

    _audioOutputLabel = new QLabel();
    _audioOutputLabel->setObjectName(QString::fromUtf8("_audioOutputLabel"));
    _audioOutputLabel->setText(tr("Output"));
    if (pix.load(QString(":AudioOutput.svg"))) {
        pix = pix.scaled(20, 20, Qt::KeepAspectRatio);
        _audioOutputLabel->setPixmap(pix);
    }

    for (const babel::PortAudio::Device &device : pa->getInputDevices()) {
        _audioInputComboBox->addItem(
                QString::fromStdString(device.name),
                QVariant::fromValue(device));
        if (device.name == "pulse")
            _audioInputComboBox->setCurrentIndex(
                    _audioInputComboBox->count() - 1);
    }

    for (const babel::PortAudio::Device &device : pa->getInputDevices()) {
        _audioOutputComboBox->addItem(
                QString::fromStdString(device.name),
                QVariant::fromValue(device));
        if (device.name == "pulse")
            _audioOutputComboBox->setCurrentIndex(
                    _audioOutputComboBox->count() - 1);
    }

    /*_settingButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _settingButton->setAutoFillBackground(false);
    _settingButton->setText("{@}");*/

    QFont font;
    font.setBold(true);
    font.setWeight(75);
    font.setPointSize(18);

    _userNameLabel->setFont(font);
    _userNameEditLineEdit->setFont(font);

    _userNameLabel->setText("n/a");
    _userNameEditLineEdit->setText("n/a");
    _avatarLabel->setText("n/a");
    _avatarLineEdit->setText("n/a");
    _userVisibilityComboBox->addItem(QIcon(QString(":Visible.svg")), QString("Visible"));
    _userVisibilityComboBox->addItem(QIcon(QString(":Unavailable.svg")), QString("Unavailable"));
    _userVisibilityComboBox->addItem(QIcon(QString(":Busy.svg")), QString("Busy"));
    _userVisibilityComboBox->addItem(QIcon(QString(":Invisible.svg")), QString("Invisible"));

    _usernameStackedWidget->addWidget(_userNameLabel);
    _usernameStackedWidget->addWidget(_userNameEditLineEdit);

    _avatarStackedWidget->addWidget(_avatarLabel);
    _avatarStackedWidget->addWidget(_avatarLineEdit);

    _avatarStackedWidget->setFixedSize(_avatarMaxSize);

    hBoxLayout->addWidget(_avatarStackedWidget);

    hBoxLayout->addWidget(_usernameStackedWidget);
    hBoxLayout2->addWidget(_userVisibilityComboBox);
    hBoxLayout2->addWidget(_muteButton);
    hBoxLayout2->addWidget(_deafenButton);
    hBoxLayout2->addWidget(_settingButton);
    hBoxLayout3->addWidget(_audioInputLabel);
    hBoxLayout3->addWidget(_audioInputComboBox);
    hBoxLayout3->addWidget(_audioOutputLabel);
    hBoxLayout3->addWidget(_audioOutputComboBox);

    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addLayout(hBoxLayout2);
    vBoxLayout->addLayout(hBoxLayout3);

    QObject::connect(_settingButton, &QPushButton::clicked,
            this, &UserWidget::slotButtonSettingPressed);
    QObject::connect(_muteButton, &QPushButton::toggled,
            this, &UserWidget::slotButtonMuteToggled);
    QObject::connect(_deafenButton, &QPushButton::toggled,
            this, &UserWidget::slotButtonDeafenToggled);
    QObject::connect(_userNameEditLineEdit, &QLineEdit::returnPressed,
            this, &UserWidget::slotUserNameEditConfirmed);
    QObject::connect(_userNameEditLineEdit, &QLineEdit::editingFinished,
            this, &UserWidget::slotUserNameEditFinished);
    QObject::connect(_userVisibilityComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &UserWidget::slotUserVisibilityChanged);
    QObject::connect(_userNameLabel, &ClickableLabel::clicked,
            this, &UserWidget::slotUserNameLabelClicked);
    QObject::connect(_avatarLabel, &ClickableLabel::clicked,
            this, &UserWidget::slotAvatarLabelClicked);
    QObject::connect(_avatarLineEdit, &QLineEdit::returnPressed,
            this, &UserWidget::slotAvatarEditConfirmed);
    QObject::connect(_avatarLineEdit, &QLineEdit::editingFinished,
            this, &UserWidget::slotAvatarEditFinished);
}

Ui::UserWidget::~UserWidget()
{ /* ignore */ }

void Ui::UserWidget::slotButtonSettingPressed(void)
{
    emit signalOpenSettingTab();
}

void Ui::UserWidget::slotButtonMuteToggled(bool checked)
{
    if (checked)
        _muteButton->setIcon(QIcon(QString(":Mute.svg")));
    else
        _muteButton->setIcon(QIcon(QString(":AudioInput.svg")));
    emit signalMuteStateChanged(checked);
}

void Ui::UserWidget::slotButtonDeafenToggled(bool checked)
{
    if (checked)
        _deafenButton->setIcon(QIcon(QString(":Deafen.svg")));
    else
        _deafenButton->setIcon(QIcon(QString(":AudioOutput.svg")));
    emit signalDeafenStateChanged(checked);
}

void Ui::UserWidget::slotUserNameEditConfirmed(void)
{
    _usernameStackedWidget->setCurrentIndex(0);
    emit signalSelfRenamed(
            babel::User::Name(
                _userNameEditLineEdit->text().toStdString()));
}

void Ui::UserWidget::slotUserNameEditFinished(void)
{
    _usernameStackedWidget->setCurrentIndex(0);
}

void Ui::UserWidget::slotSelfUpdated(const babel::User &self)
{
    _userNameLabel->setText(QString::fromStdString(self.getName()));
    _userNameEditLineEdit->setText(QString::fromStdString(self.getName()));
    _userVisibilityComboBox->setCurrentIndex(self.getVisibility());
    _avatarLineEdit->setText(QString::fromStdString(self.getAvatar()));
    _curentVisibility = _userVisibilityComboBox->currentIndex();
}

void Ui::UserWidget::slotSelfAvatarUpdated(const QPixmap &pixmap)
{
   QPixmap tmpPixMap = pixmap.scaled(std::min(_avatarMaxSize.width(), pixmap.width()),
        std::min(_avatarMaxSize.height(), pixmap.height()), Qt::KeepAspectRatio);
    _avatarLabel->setPixmap(tmpPixMap);
}

void Ui::UserWidget::slotUserVisibilityChanged(int index)
{
    _userVisibilityComboBox->setCurrentIndex(_curentVisibility);
    emit signalSelfVisibilityChanged(babel::User::Visibility(index));
}

void Ui::UserWidget::slotUserNameLabelClicked(void)
{
    _userNameEditLineEdit->setText(_userNameLabel->text());
    _userNameEditLineEdit->setFocus(Qt::MouseFocusReason);
    _usernameStackedWidget->setCurrentIndex(1);
}

void Ui::UserWidget::slotAvatarLabelClicked(void)
{
    _avatarStackedWidget->setCurrentIndex(1);
    _avatarLineEdit->setFocus(Qt::MouseFocusReason);
}

void Ui::UserWidget::slotAvatarEditConfirmed(void)
{
    _avatarStackedWidget->setCurrentIndex(0);
    if (!_avatarLineEdit->text().isEmpty()) {
        emit signalSelfAvatarChanged(_avatarLineEdit->text().toStdString());
    }
}

void Ui::UserWidget::slotAvatarEditFinished(void)
{
    _avatarStackedWidget->setCurrentIndex(0);
}

bool Ui::UserWidget::getButtonMuteState(void)
{
    return _muteButton->isChecked();
}

bool Ui::UserWidget::getButtonDeafenState(void)
{
    return _deafenButton->isChecked();
}

const babel::PortAudio::Device Ui::UserWidget::getSelectedInputDevice(void)
{
    return (_audioInputComboBox->currentData()
            .value<babel::PortAudio::Device>());
}

const babel::PortAudio::Device Ui::UserWidget::getSelectedOutputDevice(void)
{
    return (_audioOutputComboBox->currentData()
            .value<babel::PortAudio::Device>());
}
