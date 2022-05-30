QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
build_pass: DebugBuild:   CONFIG += console
build_pass: ReleaseBuild: CONFIG -= console

SOURCES += \
    main.cpp \
    maindialog.cpp \
    manager_config.cpp

HEADERS += \
    maindialog.h \
    manager_config.h \
    utils.hpp

FORMS += \
    maindialog.ui

RESOURCES += \
    GlobalHotkeyManager.qrc

TRANSLATIONS += \
    res/translate_zh_CN.ts \
    res/translate_ja_JP.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
