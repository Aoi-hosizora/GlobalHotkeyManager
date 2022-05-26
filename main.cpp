#include <QtWidgets/QApplication>
// #include <QtCore/QTranslator>

#include "maindialog.h"
#include "utils.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setFont(getSystemMessageFont());
    a.setQuitOnLastWindowClosed(false);

    // QTranslator translator;
    // translator.load(":/en_US.qm");
    // translator.load(":/zh_CN.qm");
    // translator.load(":/ja_JP.qm");
    // qApp->installTranslator(&translator);

    MainDialog w;
    w.show();
    w.hide();
    return a.exec();
}
