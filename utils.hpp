#include <Windows.h>

#include <QtGui/QFont>
#include <QtGui/QKeySequence>
#include <QtWidgets/QApplication>
#include <string>

#ifndef UTILS_HPP
#define UTILS_HPP

inline QFont getSystemMessageFont() {
    return QApplication::font("QMessageBox");
    // NONCLIENTMETRICS ncm;
    // ncm.cbSize = sizeof(NONCLIENTMETRICS);
    // SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
    // QFont f = QApplication::font();
    // f.setFamily(QString::fromStdWString((std::wstring) ncm.lfMessageFont.lfFaceName));
    // return f;
}

#endif  // UTILS_HPP