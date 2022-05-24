#include "maindialog.h"

#include "ui_maindialog.h"
#include "utils.hpp"

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog) {
    setWindowOpacity(0);  // prevent showing window when program startups
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setupEvents();
}

MainDialog::~MainDialog() {
    delete ui;
}

void MainDialog::setupEvents() {
    connect(ui->btnHide, SIGNAL(clicked()), this, SLOT(onBtnHideClicked()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(onBtnExitClicked()));
    for (int i = 0; i < 20; i++) {
        ui->lstHotkeys->addItem("item_" + QString::number(i));
    }
    connect(ui->lstHotkeys, SIGNAL(currentRowChanged(int)), this, SLOT(onLstHotkeysCurrentRowChanged(int)));

    if (!RegisterHotKey((HWND) winId(), SHOWME_HOTKEY, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F12)) {
        QMessageBox::critical(this, tr("Error"), tr("RegisterHotKey failed"));
        sure_to_exit = true;
        close();
    }
}

void MainDialog::onLstHotkeysCurrentRowChanged(int index) {
    qDebug() << ui->lstHotkeys->item(index)->data(Qt::DisplayRole).toString();
}

void MainDialog::closeEvent(QCloseEvent *e) {
    enum { EXIT,
           HIDE,
           CANCEL } action;
    if (sure_to_exit) {
        action = EXIT;
    } else {
        QMessageBox msgBox;
        msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("Global Hotkey Manager"));
        msgBox.setText(tr("Sure to exit Global Hotkey Manager?") + "\n\n" +
                       tr("All hotkeys registered will be unregistered and cannot be invoked if you choose to exit."));
        auto btnExit = msgBox.addButton(tr("Exit"), QMessageBox::YesRole);
        auto btnHide = msgBox.addButton(tr("Hide"), QMessageBox::NoRole);
        msgBox.addButton(QMessageBox::Cancel);
        msgBox.setDefaultButton(btnHide);
        msgBox.exec();
        action = msgBox.clickedButton() == btnExit ? EXIT : (msgBox.clickedButton() == btnHide ? HIDE : CANCEL);
    }

    if (action == EXIT) {
        UnregisterHotKey((HWND) this->winId(), SHOWME_HOTKEY);
        e->accept();
    } else {
        if (action == HIDE) {
            hide();
        }
        e->ignore();
    }
}

void MainDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        hide();
        return;
    }
    QDialog::keyPressEvent(e);
}

bool MainDialog::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    auto msg = reinterpret_cast<MSG *>(message);
    if (msg->message == WM_HOTKEY) {
        if ((int) msg->wParam == SHOWME_HOTKEY) {
            setWindowOpacity(1);
            show();
            return true;
        }
    }
    return false;
}

void MainDialog::onBtnHideClicked() {
    hide();
}

void MainDialog::onBtnExitClicked() {
    sure_to_exit = false;
    close();
}
