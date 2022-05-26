#include "maindialog.h"

#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

#include "ui_maindialog.h"
#include "utils.hpp"

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog) {
    setWindowOpacity(0);  // prevent showing window when program startups
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setupEvents();
    loadData();
}

MainDialog::~MainDialog() {
    qDebug() << "~MainDialog()";
    delete ui;
}

void MainDialog::setupEvents() {
    connect(ui->btnHide, SIGNAL(clicked()), this, SLOT(onBtnHideClicked()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(onBtnExitClicked()));
    connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(onBtnRefreshClicked()));
    connect(ui->lstHotkeys, SIGNAL(currentRowChanged(int)), this, SLOT(onLstHotkeysCurrentRowChanged(int)));
    connect(ui->lstHotkeys, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onLstHotkeysDoubleClicked()));

    if (!RegisterHotKey((HWND) winId(), SHOWME_HOTKEY, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F12)) {
        QMessageBox::critical(this, tr("Error"), tr("RegisterHotKey Ctrl+Shift+Alt+F12 failed"));
        sure_to_exit = true;
        close();
    }
}

void MainDialog::loadData() {
    if (!HotkeyItem::readConfigsFromRegistry(&hotkey_items)) {
        QMessageBox::critical(this, tr("Error"), tr("readConfigsFromRegistry failed"));
        sure_to_exit = true;
        close();
    }

    // ui->lstHotkeys->setSpacing(10);
    ui->lstHotkeys->setStyleSheet("QListWidget::item { padding-left: 2px; padding-top: 3px; padding-bottom: 3px; }");
    for (auto &item : hotkey_items) {
        UINT fsModifiers, vk;
        qKeySequenceToNative(item.hotkey(), &fsModifiers, &vk);
        int id = generateHotkeyId(fsModifiers, vk);
        if (!RegisterHotKey((HWND) winId(), id, fsModifiers, vk)) {
            QMessageBox::critical(this, tr("Error"), tr("RegisterHotKey %0 failed").arg(item.hotkey().toString()));
            ui->lstHotkeys->addItem(item.toString() + " (failed)");
        } else {
            id_to_item.insert(std::make_pair(id, &item));
            ui->lstHotkeys->addItem(item.toString());
        }
    }
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
        for (auto item : hotkey_items) {
            UnregisterHotKey((HWND) winId(), generateHotkeyId(item.hotkey()));
        }
        UnregisterHotKey((HWND) winId(), SHOWME_HOTKEY);
        e->accept();
        qApp->quit();
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
    } else {
        QDialog::keyPressEvent(e);
    }
}

bool MainDialog::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    auto msg = reinterpret_cast<MSG *>(message);
    if (msg->message == WM_HOTKEY) {
        int id = (int) msg->wParam;
        if (id == SHOWME_HOTKEY) {
            setWindowOpacity(1);
            show();
            return true;
        }
        if (id_to_item.find(id) != id_to_item.end()) {
            auto item = id_to_item.at(id);
            auto r = (uintptr_t) ShellExecuteW(nullptr,
                item->op().toStdWString().c_str(), item->file().toStdWString().c_str(),
                item->param().toStdWString().c_str(), item->dir().toStdWString().c_str(), item->style());
            if (r <= 32) {
                QMessageBox::critical(this, "critical", QString("Failed to execute the given command \"%0\" (errno = %1)").arg(item->title(), QString::number(r)));
            }
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

void MainDialog::onBtnRefreshClicked() {
    for (auto &item : hotkey_items) {
        UnregisterHotKey((HWND) winId(), generateHotkeyId(item.hotkey()));
    }
    id_to_item.clear();
    hotkey_items.clear();
    ui->lstHotkeys->clear();
    loadData();
}

void MainDialog::onLstHotkeysCurrentRowChanged(int index) {
    qDebug() << ui->lstHotkeys->item(index)->data(Qt::DisplayRole).toString();
}

void MainDialog::onLstHotkeysDoubleClicked() {
    int index = ui->lstHotkeys->currentRow();
    auto item = hotkey_items.at(index);
    QMessageBox::information(this, tr("Global Hotkey Manager"),
        QString("Title: %0\nHotkey: %1\nOperation: %2\nFile: %3\nParameter: %4\nDirectory: %5").arg(item.title(), item.hotkey().toString(), item.op(), item.file(), item.param(), item.dir()));
}
