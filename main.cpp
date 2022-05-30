#include <QtCore/QObject>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "maindialog.h"
#include "manager_config.h"

void changeLanguage(ManagerConfig::Language);

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setFont(a.font("QMessageBox"));
    a.setQuitOnLastWindowClosed(false);

    ManagerConfig config;
    if (!ManagerConfig::readConfigFromRegistry(&config)) {
        QMessageBox::critical(0, QObject::tr("Global Hotkey Manager"),
            QObject::tr("Failed to load config from registry (%0), exiting...").arg(ManagerConfig::registryPath()));
        return 0;
    }

    changeLanguage(config.lang());
    MainDialog w(&config);
    w.setWindowOpacity(0);
    w.show();
    w.hide();
    w.setWindowOpacity(1);
    return a.exec();
}

void changeLanguage(ManagerConfig::Language lang) {
    static QTranslator appTranslator, qtTranslator;
    if (lang != ManagerConfig::Language::ENGLISH) {
        QString appQmPath = lang == ManagerConfig::Language::CHINESE ? ":/app_zh_CN.qm" : ":/app_ja_JP.qm";
        QString qtQmPath = lang == ManagerConfig::Language::CHINESE ? ":/qt_zh_CN.qm" : ":/qt_ja_JP.qm";
        if (appTranslator.load(appQmPath)) {
            QApplication::installTranslator(&appTranslator);
        }
        if (qtTranslator.load(qtQmPath)) {
            QApplication::installTranslator(&qtTranslator);
        }
    }
}
