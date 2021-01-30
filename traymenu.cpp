#include "traymenu.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

//! [0]
TrayMenu::TrayMenu()
{
    //createIconGroupBox();
    //createMessageGroupBox();

    //iconLabel->setMinimumWidth(durationLabel->sizeHint().width());
    trayIcon->setIcon(QIcon(":/images/AppIcon.png"));
    trayIcon->setToolTip("OmniEdge");
    createActions();
    createTrayIcon();

//    connect(showMessageButton, &QAbstractButton::clicked, this, &Widget::showMessage);
//    connect(showIconCheckBox, &QAbstractButton::toggled, trayIcon, &QSystemTrayIcon::setVisible);
//    connect(iconComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &Widget::setIcon);
//    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Widget::messageClicked);
//    connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::iconActivated);

//    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(iconGroupBox);
//    mainLayout->addWidget(messageGroupBox);
//    setLayout(mainLayout);

//    iconComboBox->setCurrentIndex(1);
    trayIcon->show();

//    setWidgetTitle(tr("Systray"));
//    resize(400, 300);
}
//! [0]

//! [1]
void TrayMenu::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}
//! [1]

//! [2]
void TrayMenu::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void TrayMenu::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        //iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void TrayMenu::showMessage()
{
//    showIconCheckBox->setChecked(true);
//    int selectedIcon = typeComboBox->itemData(typeComboBox->currentIndex()).toInt();
//    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(selectedIcon);

//    if (selectedIcon == -1) { // custom icon
//        QIcon icon(iconComboBox->itemIcon(iconComboBox->currentIndex()));
//        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
//                          durationSpinBox->value() * 1000);
//    } else {
//        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), msgIcon,
//                          durationSpinBox->value() * 1000);
//    }
}
//! [5]

//! [6]
void TrayMenu::messageClicked()
{
    QMessageBox::information(0, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}

void TrayMenu::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void TrayMenu::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

#endif

