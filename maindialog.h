#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <Windows.h>

#include <QtGui/QCloseEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QDialog>
#include <vector>

#include "hotkey_item.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainDialog;
}
QT_END_NAMESPACE

class MainDialog : public QDialog {
    Q_OBJECT

   public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

   private Q_SLOTS:
    void closeEvent(QCloseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void onBtnHideClicked();
    void onBtnExitClicked();
    void onLstHotkeysCurrentRowChanged(int);
    void onLstHotkeysDoubleClicked();

   protected:
    bool nativeEvent(const QByteArray &, void *, long *) override;

   private:
    void setupEvents();
    void loadData();

   private:
    Ui::MainDialog *ui;
    std::vector<HotkeyItem> hotkey_items;
    const int SHOWME_HOTKEY = 0;
    bool sure_to_exit = false;
};

#endif  // MAINDIALOG_H
