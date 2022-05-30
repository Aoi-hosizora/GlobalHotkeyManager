#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtGui/QCloseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QShowEvent>
#include <QtWidgets/QDialog>
#include <map>
#include <qt_windows.h>
#include <stdint.h>
#include <vector>

#include "manager_config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainDialog;
}
QT_END_NAMESPACE

class MainDialog : public QDialog {
    Q_OBJECT

   public:
    MainDialog(const ManagerConfig *, QWidget *parent = nullptr);
    ~MainDialog();

   protected:
    void showEvent(QShowEvent *e) override;
    void closeEvent(QCloseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    bool nativeEvent(const QByteArray &, void *, long *) override;

   signals:
    void shownFirstly();

   private slots:
    void onBtnHideClicked();
    void onBtnExitClicked();
    void onBtnEditClicked();
    void onBtnInvokeClicked();
    void onBtnRefreshClicked();
    void onLstHotkeysCurrentRowChanged(int);

   private:
    void bindEvents();
    bool loadConfig();
    bool loadItems(bool = true);
    void invokeItem(const HotkeyItem *);
    void close(bool);

   private:
    Ui::MainDialog *ui;
    const ManagerConfig *config;
    const QString TITLE = tr("Global Hotkey Manager");
    int showWindowKeyId = 0;
    bool sureToExit = false;
    std::vector<HotkeyItem> hotkeyItems;
    std::map<int, const HotkeyItem *> idToHotkeyItem;
};

#endif  // MAINDIALOG_H
