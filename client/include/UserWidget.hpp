/*
** EPITECH PROJECT, 2020
** B-CPP-500-STG-5-1-babel-loic.branstett
** File description:
** UserWidget
*/

#ifndef USERWIDGET_HPP_
#define USERWIDGET_HPP_

#include <QtCore/QVariant>
#include <QStackedWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include "ClickableLabel.hpp"
#include "UserObject.hpp"
#include "audio/PortAudio.hpp"

namespace Ui {

    /// \brief Main 'self' user widget
    class UserWidget : public QGroupBox {
        Q_OBJECT

        public:
            /// \brief Constructor
            UserWidget(QWidget *parent, std::shared_ptr<babel::PortAudio> &pa);
            /// \brief Destructor
            ~UserWidget();
            /// \brief true if mute button activated, false is not
            bool getButtonMuteState(void);
            /// \brief true if deafen button is activated, false is not
            bool getButtonDeafenState(void);

            /// \brief get the selected input device
            const babel::PortAudio::Device getSelectedInputDevice(void);
            /// \brief set the selected input device
            const babel::PortAudio::Device getSelectedOutputDevice(void);

        public slots:
            /// \brief Update self
            void slotSelfUpdated(const babel::User &user);
            /// \brief Update seld avatar
            void slotSelfAvatarUpdated(const QPixmap &pixmap);

        private slots:
            /// \brief Event when setting button is pressed
            void slotButtonSettingPressed(void);
            /// \brief Event when ute button is toggled
            void slotButtonMuteToggled(bool checked);
            /// \brief Event when deafen button is toggled
            void slotButtonDeafenToggled(bool checked);
            /// \brief Event when the name is clicked on
            void slotUserNameLabelClicked(void);
            /// \brief Event when the name edit is finished
            void slotUserNameEditFinished(void);
            /// \brief Event when the name edit is confirmed
            void slotUserNameEditConfirmed(void);
            /// \brief Event when the avatar is clicked
            void slotAvatarLabelClicked(void);
            /// \brief Event when the avatar edit is confirmed
            void slotAvatarEditConfirmed(void);
            /// \brief Event when the avatar edit is finished
            void slotAvatarEditFinished(void);
            /// \brief Event when the user visibility is changed
            void slotUserVisibilityChanged(int index);

        signals:
            /// \brief Signal who tells when to open the setting tab
            void signalOpenSettingTab(void);
            /// \brief Signal who tells when self is renamed
            void signalSelfRenamed(const babel::User::Name &newName);
            /// \brief Signal who tells when avatar self is changed
            void signalSelfAvatarChanged(const babel::User::Avatar &newAvatar);
            /// \brief Signal who tells when self visibility is changed
            void signalSelfVisibilityChanged(const babel::User::Visibility &newVisibility);
            /// \brief Signal to send when mute state is changed
            void signalMuteStateChanged(bool checked);
            /// \brief Signal to send when deafen state is changed
            void signalDeafenStateChanged(bool checked);

        private:
            const QSize _avatarMaxSize = QSize(150, 150);
            QStackedWidget *_usernameStackedWidget;
            ClickableLabel *_userNameLabel;
            QLineEdit *_userNameEditLineEdit;
            QComboBox *_userVisibilityComboBox;
            QPushButton *_settingButton;
            QPushButton *_muteButton;
            QPushButton *_deafenButton;
            QStackedWidget *_avatarStackedWidget;
            ClickableLabel *_avatarLabel;
            QLineEdit *_avatarLineEdit;
            QComboBox *_audioInputComboBox;
            QComboBox *_audioOutputComboBox;
            QLabel *_audioInputLabel;
            QLabel *_audioOutputLabel;
            int _curentVisibility;
    };
}

Q_DECLARE_METATYPE(babel::PortAudio::Device);

#endif /* !USERWIDGET_HPP_ */
