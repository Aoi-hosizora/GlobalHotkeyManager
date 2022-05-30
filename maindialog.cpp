#include "maindialog.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QTimer>
#include <QtWidgets/QMessageBox>

#include "ui_maindialog.h"
#include "utils.hpp"

MainDialog::MainDialog(const ManagerConfig *config, QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog), config(config) {
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    bindEvents();

    connect(this, &MainDialog::shownFirstly, this, [this]() {
        QTimer::singleShot(0, this, [this]() {
            loadConfig();
            loadItems();
        });
    });
}

MainDialog::~MainDialog() {
    delete ui;
}

void MainDialog::bindEvents() {
    connect(ui->btnHide, SIGNAL(clicked()), this, SLOT(onBtnHideClicked()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(onBtnExitClicked()));
    connect(ui->btnEdit, SIGNAL(clicked()), this, SLOT(onBtnEditClicked()));
    connect(ui->btnInvoke, SIGNAL(clicked()), this, SLOT(onBtnInvokeClicked()));
    connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(onBtnRefreshClicked()));
    connect(ui->lstHotkeys, SIGNAL(currentRowChanged(int)), this, SLOT(onLstHotkeysCurrentRowChanged(int)));
}

bool MainDialog::loadConfig() {
    int keyId;
    if (!utils::registerHotkey(winId(), config->hotkey(), &keyId)) {
        QMessageBox::critical(this, TITLE, tr("Failed to register hotkey %0 for showing window, exiting...").arg(config->hotkey().toString()));
        close(true);
        return false;
    }
    ui->lblHint->setText(tr("Tip: You can press %0 to open this window.").arg(config->hotkey().toString()));
    showWindowKeyId = keyId;
    return true;
}

bool MainDialog::loadItems(bool force) {
    // 1. read hotkeys from registry
    std::vector<HotkeyItem> newItems;
    if (!HotkeyItem::readItemsFromRegistry(&newItems)) {
        if (force) {
            QMessageBox::critical(this, TITLE, tr("Failed to load hotkeys from registry (%0), exiting...").arg(ManagerConfig::registryPath()));
            close(true);
            return false;
        }
        QMessageBox::critical(this, TITLE, tr("Failed to load hotkeys from registry, ignored."));
        return false;
    }

    // 2. unregister old hotkeys
    ui->lstHotkeys->clear();
    idToHotkeyItem.clear();
    for (auto &item : hotkeyItems) {
        utils::unregisterHotkey(winId(), item.hotkey());
    }

    // 3. check and register new hotkeys
    hotkeyItems = newItems;
    std::vector<HotkeyItem *> failedItems;
    for (auto &item : hotkeyItems) {
        int keyId;
        if (utils::registerHotkey(winId(), item.hotkey(), &keyId)) {
            idToHotkeyItem.insert(std::make_pair(keyId, &item));
            auto line = item.toString().replace(QRegularExpression("^|$"), "\u2009").replace("\n", "\u2009\n\u2009");  // /u+2009 => Thin Space
            ui->lstHotkeys->addItem(line);
        } else {
            failedItems.push_back(&item);
            auto line = item.toString().replace(QRegularExpression("^|$"), "\u2009").replace("\n", QString(" %0\u2009\n\u2009").arg(tr("(conflicted)")));
            ui->lstHotkeys->addItem(line);
        }
    }
    ui->lblListTitle->setText(tr("&Global hotkeys: (All %0)").arg(ui->lstHotkeys->count()));
    if (ui->lstHotkeys->count() > 0) {
        ui->lstHotkeys->item(0)->setSelected(true);
        onLstHotkeysCurrentRowChanged(0);  // <<<
    } else {
        onLstHotkeysCurrentRowChanged(-1);  // <<<
    }

    // 4. show alert for failed hotkeys
    if (failedItems.size() > 0) {
        QStringList l;
        for (auto item : failedItems) {
            l << QString("\"%0\" (%1)").arg(item->title(), item->hotkey().toString());
        }
        QMessageBox::warning(this, TITLE,
            QString("%0\n\n%1").arg(tr("Warning: Following hotkey(s) are conflicted and cannot be registered. Please check and try to refresh the list."), l.join("\n")));
    }
    return true;
}

void MainDialog::close(bool sureToExit) {
    this->sureToExit = sureToExit;
    QTimer::singleShot(0, this, [this]() { QDialog::close(); });
}

void MainDialog::showEvent(QShowEvent *e) {
    QDialog::showEvent(e);
    static bool first = true;
    if (first) {
        first = false;
        emit shownFirstly();
    }
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
        msgBox.setWindowTitle(TITLE);
        msgBox.setText(QString("%0\n\n%1")
                           .arg(tr("Sure to exit Global Hotkey Manager?"), tr("All hotkeys registered will be unregistered and cannot be invoked if you choose to exit.")));
        auto btnExit = msgBox.addButton(tr("E&xit"), QMessageBox::YesRole);
        auto btnHide = msgBox.addButton(tr("&Hide"), QMessageBox::NoRole);
        msgBox.addButton(QMessageBox::Cancel);
        msgBox.setDefaultButton(btnHide);
        msgBox.exec();
        action = msgBox.clickedButton() == btnExit ? EXIT : (msgBox.clickedButton() == btnHide ? HIDE : CANCEL);
    }

    if (action == EXIT) {
        for (auto &item : hotkeyItems) {
            utils::unregisterHotkey(winId(), item.hotkey());
        }
        utils::unregisterHotkey(winId(), config->hotkey());
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
        if (id == showWindowKeyId) {
            show();
            activateWindow();
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
    if (result <= 32 && result != SE_ERR_ACCESSDENIED) {
        auto errMsg = utils::getLastErrorAsString();
        QMessageBox::critical(this, TITLE, QString("%0\n%1").arg(tr("Failed to execute command of \"%0\":").arg(item->title()), QString::fromStdWString(errMsg)));
    }
}

void MainDialog::onBtnHideClicked() {
    hide();
}

void MainDialog::onBtnExitClicked() {
    close(false);
}

void MainDialog::onBtnEditClicked() {
    auto ok = QMessageBox::question(this, TITLE,
        tr("Allow to use regjump tool to edit the hotkeys?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if (ok == QMessageBox::Yes) {
        ShellExecuteW(nullptr, L"runas", L"cmd.exe", (L"/C regjump " + ManagerConfig::registryPath().toStdWString()).c_str(), nullptr, SW_HIDE);
    }
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
    std::map<QString, HotkeyItem> oldItems;
    for (auto &item : hotkeyItems) {
        oldItems.insert(std::make_pair(item.title(), item));
    }
    if (!loadItems(false)) {
        return;
    }

    int added = 0, updated = 0, unmodified = 0;
    for (auto &newItem : hotkeyItems) {
        if (oldItems.find(newItem.title()) == oldItems.end()) {
            added++;
            continue;
        }
        auto oldItem = oldItems.at(newItem.title());
        if (newItem != oldItem) {
            updated++;
        } else {
            unmodified++;
        }
    }
    int deleted = oldItems.size() - unmodified - updated;
    if (added == 0 && updated == 0 && deleted == 0) {
        QMessageBox::information(this, TITLE, tr("Done! Nothing changed."));
    } else {
        QMessageBox::information(this, TITLE,
            tr("Done! %0 hotkey(s) added, %1 hotkey(s) updated, %2 hotkey(s) deleted.").arg(QString::number(added), QString::number(updated), QString::number(deleted)));
    }
}

void MainDialog::onLstHotkeysCurrentRowChanged(int index) {
    if (index < 0 || index >= (int) hotkeyItems.size()) {
        ui->btnInvoke->setEnabled(false);
        ui->grpDetails->setTitle(tr("Details"));
        ui->lblHotkey->setText(tr("<unknown>"));
        ui->lblFile->setText(tr("<unknown>"));
        ui->lblParam->setText(tr("<unknown>"));
        ui->lblDir->setText(tr("<unknown>"));
        ui->lblOpStyle->setText(tr("<unknown>"));
        return;
    }

    ui->btnInvoke->setEnabled(true);
    auto item = hotkeyItems.at(index);

    auto setLabel = [](QLabel *lbl, QString str) {
        if (str.isEmpty()) {
            lbl->setText(tr("<empty>"));
            return;
        }
        QFontMetrics metrics(lbl->font());
        QString elidedText = metrics.elidedText(str, Qt::ElideRight, lbl->width());
        lbl->setText(elidedText);
        if (elidedText.contains("…")) {
            lbl->setToolTip(str);
        }
    };
    ui->grpDetails->setTitle(tr("Details of \"%0\"").arg(item.title()));
    setLabel(ui->lblHotkey, item.hotkey().toString());
    setLabel(ui->lblFile, item.file());
    setLabel(ui->lblParam, item.param());
    setLabel(ui->lblDir, item.dir());
    setLabel(ui->lblOpStyle, QString("%0, %1").arg(item.op(), QString::fromStdWString(utils::formatStyleToString(item.style()))));
}
