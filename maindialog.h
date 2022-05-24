#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <Windows.h>

#include <QtCore/QDebug>
#include <QtCore/QStringListModel>
#include <QtCore/QTimer>
#include <QtGui/QCloseEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <vector>

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

   protected:
    bool nativeEvent(const QByteArray &, void *, long *) override;

   private:
    void setupEvents();

   private:
    Ui::MainDialog *ui;
    const int SHOWME_HOTKEY = 0;
    bool sure_to_exit = false;
};

#endif  // MAINDIALOG_H
