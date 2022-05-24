QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += debug console

SOURCES += \
    main.cpp \
    maindialog.cpp \
    hotkey_item.cpp

HEADERS += \
    maindialog.h \
    hotkey_item.h \
    utils.hpp

FORMS += \
    maindialog.ui

RESOURCES += \
    GlobalHotkeyManager.qrc

TRANSLATIONS += \
    res/translate_en_US.ts \
    res/translate_zh_CN.ts \
    res/translate_ja_JP.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
