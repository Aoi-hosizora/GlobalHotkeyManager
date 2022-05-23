#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <Windows.h>

#include <QtCore/QTimer>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>

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
    void on_btnTest_clicked();

   protected:
    bool nativeEvent(const QByteArray &, void *, long *) override;

   private:
    Ui::MainDialog *ui;
};
#endif  // MAINDIALOG_H
