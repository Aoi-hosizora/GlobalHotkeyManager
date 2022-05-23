#include "maindialog.h"

#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MainDialog) {
    // QTimer::singleShot(1, this, [this]() { this->hide(); });
    setWindowOpacity(0);  // prevent slow show when startup
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    // setFixedSize(440, 237);
    // connect(ui->btnTest, SIGNAL(clicked()), this, SLOT(on_btnTest_clicked()));

    if (!RegisterHotKey((HWND) this->winId(), 1, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F12)) {
        QMessageBox::critical(this, tr("Error"), tr("RegisterHotKey failed"));
        close();
        exit(1);
    }
}

MainDialog::~MainDialog() {
    delete ui;
}

void MainDialog::on_btnTest_clicked() {
    QMessageBox::information(this, tr("GlobalHotkeyManager"), tr("hide window"));
    this->hide();
}

bool MainDialog::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    auto msg = reinterpret_cast<MSG *>(message);
    if (msg->message == WM_HOTKEY && msg->wParam == 1) {
        setWindowOpacity(1);
        this->show();
        return true;
    }
    return false;
}
