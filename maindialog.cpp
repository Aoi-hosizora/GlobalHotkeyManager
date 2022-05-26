#include "maindialog.h"

#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>
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
    delete ui;
}

void MainDialog::setupEvents() {
    connect(ui->btnHide, SIGNAL(clicked()), this, SLOT(onBtnHideClicked()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(onBtnExitClicked()));
    connect(ui->btnEdit, SIGNAL(clicked()), this, SLOT(onBtnEditClicked()));
    connect(ui->btnInvoke, SIGNAL(clicked()), this, SLOT(onBtnInvokeClicked()));
    connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(onBtnRefreshClicked()));
    connect(ui->lstHotkeys, SIGNAL(currentRowChanged(int)), this, SLOT(onLstHotkeysCurrentRowChanged(int)));

    if (!ManagerConfig::readConfigFromRegistry(&config)) {
        QMessageBox::critical(this, TITLE, tr("Failed to load config from registry, exiting..."));
        close(true);
        return;
    }
    int keyId;
    if (!utils::registerHotkey(winId(), config.hotkey(), &keyId)) {
        QMessageBox::critical(this, TITLE, tr("Failed to register hotkey %0 for showing window, exiting..."));
        close(true);
        return;
    }
    ui->lblHint->setText(tr("Note: You can press %0 to open this window.").arg(config.hotkey().toString()));
    config.setKeyId(keyId);
}

void MainDialog::loadData() {
    ui->lstHotkeys->clear();
    idToHotkeyItem.clear();
    hotkeyItems.clear();
    if (!HotkeyItem::readConfigsFromRegistry(&hotkeyItems)) {
        QMessageBox::critical(this, TITLE, tr("Failed to load hotkey items from registry, exiting..."));
        close(true);
        return;
    }

    std::vector<HotkeyItem *> failedItems;
    for (auto &item : hotkeyItems) {
        int keyId;
        if (utils::registerHotkey(winId(), item.hotkey(), &keyId)) {
            idToHotkeyItem.insert(std::make_pair(keyId, &item));
            auto line = item.toString().replace(QRegularExpression("^|$"), "\u2009").replace(QRegularExpression("\n"), "\u2009\n\u2009");  // /u+2009 => Thin Space
            ui->lstHotkeys->addItem(line);
        } else {
            failedItems.push_back(&item);
        }
    }
    ui->lblListTitle->setText(tr("&Global hotkeys: (All %0)").arg(ui->lstHotkeys->count()));

    if (failedItems.size() > 0) {
        QStringList l;
        for (auto item : failedItems) {
            l << QString("\"%0\" (%1)").arg(item->title(), item->hotkey().toString());
        }
        QMessageBox::critical(this, TITLE,
            tr("The following hotkey items cannot be registered, maybe they are already used. Please check and refresh the list.") + "\n\n" + l.join("\n"));
    }
}

void MainDialog::close(bool sureToExit) {
    this->sureToExit = sureToExit;
    QWidget::close();
}

void MainDialog::closeEvent(QCloseEvent *e) {
    enum { EXIT,
        HIDE,
        CANCEL } action;
    if (sureToExit) {
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
        for (auto &item : hotkeyItems) {
            utils::unregisterHotkey(winId(), item.hotkey());
        }
        utils::unregisterHotkey(winId(), config.hotkey());
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
        if (id == config.keyId()) {
            setWindowOpacity(1);
            show();
            return true;
        }
        if (idToHotkeyItem.find(id) != idToHotkeyItem.end()) {
            invokeItem(idToHotkeyItem.at(id));
            return true;
        }
    }
    return false;
}

void MainDialog::invokeItem(const HotkeyItem *item) {
    const wchar_t *op = item->op().toStdWString().c_str();
    const wchar_t *file = item->file().toStdWString().c_str();
    const wchar_t *param = item->param().toStdWString().c_str();
    const wchar_t *dir = item->dir().toStdWString().c_str();
    int style = item->style();
    auto result = (uintptr_t) ShellExecuteW(nullptr, op, file, param, dir, style);
    if (result <= 32) {
        QMessageBox::critical(this, TITLE, QString("Failed to execute the given command \"%0\" (errno = %1)").arg(item->title(), QString::number(result)));
    }
}

void MainDialog::onBtnHideClicked() {
    hide();
}

void MainDialog::onBtnExitClicked() {
    close(false);
}

void MainDialog::onBtnEditClicked() {
    QMessageBox::information(this, TITLE, "TODO");
}

void MainDialog::onBtnInvokeClicked() {
    auto index = ui->lstHotkeys->currentRow();
    if (index < 0 || index >= (int) hotkeyItems.size()) {
        return;
    }
    auto item = hotkeyItems.at(index);
    invokeItem(&item);
}

void MainDialog::onBtnRefreshClicked() {
    for (auto &item : hotkeyItems) {
        utils::unregisterHotkey(winId(), item.hotkey());
    }
    loadData();
    QMessageBox::information(this, TITLE, "Done.");
}

void MainDialog::onLstHotkeysCurrentRowChanged(int index) {
    if (index < 0 || index >= (int) hotkeyItems.size()) {
        return;
    }
    // qDebug() << ui->lstHotkeys->item(index)->data(Qt::DisplayRole).toString();
    auto item = hotkeyItems.at(index);

    auto setLabel = [](QLabel *lbl, QString str) {
        if (str.isEmpty()) {
            str = "<empty>";
        }
        QFontMetrics metrics(lbl->font());
        QString elidedText = metrics.elidedText(str, Qt::ElideRight, lbl->width());
        lbl->setText(elidedText);
        if (elidedText.contains("…")) {
            lbl->setToolTip(str);
        }
    };
    setLabel(ui->lblDetailTitle, tr("\"%0\" (%1)").arg(item.title(), item.hotkey().toString()));
    setLabel(ui->lblFile, item.file());
    setLabel(ui->lblParam, item.param());
    setLabel(ui->lblDir, item.dir());
    setLabel(ui->lblOpStyle, tr("%0, %1").arg(item.op(), QString::fromStdWString(utils::formatStyleToString(item.style()))));
}
