#include <QtCore/QDebug>
#include <QtGui/QFont>
#include <QtGui/QKeySequence>
#include <QtWidgets/QApplication>
#include <algorithm>
#include <qt_windows.h>
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

inline int generateHotkeyId(UINT fsModifiers, UINT vk) {
    return vk << 4 | fsModifiers;
}

bool qKeySequenceToNative(const QKeySequence &, UINT *, UINT *);
inline int generateHotkeyId(const QKeySequence &shortcut) {
    UINT fsModifiers, vk;
    qDebug() << qKeySequenceToNative(shortcut, &fsModifiers, &vk);
    return generateHotkeyId(fsModifiers, vk);
}

// The following three functions are referred from:
// https://github.com/ddqd/qxtglobalshortcut5/blob/master/gui/qxtglobalshortcut_win.cpp#L39-L242

UINT qKeyboardModifiersToNative(Qt::KeyboardModifiers modifiers);
UINT qKeyCodeToNative(Qt::Key key);
inline bool qKeySequenceToNative(const QKeySequence &shortcut, UINT *fsModifiers, UINT *vk) {
    if (shortcut.isEmpty()) {
        *fsModifiers = 0;
        *vk = 0;
        return false;
    }
    Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    auto mods = Qt::KeyboardModifiers(shortcut[0] & allMods);
    auto key = Qt::Key((shortcut[0] ^ allMods) & shortcut[0]);
    *fsModifiers = qKeyboardModifiersToNative(mods);
    *vk = qKeyCodeToNative(key);
    return true;
}

inline UINT qKeyboardModifiersToNative(Qt::KeyboardModifiers modifiers) {
    UINT native = 0;
    if (modifiers & Qt::AltModifier) {
        native |= MOD_ALT;  // 0x0001
    }
    if (modifiers & Qt::ControlModifier) {
        native |= MOD_CONTROL;  // 0x0002
    }
    if (modifiers & Qt::ShiftModifier) {
        native |= MOD_SHIFT;  // 0x0004
    }
    if (modifiers & Qt::MetaModifier) {
        native |= MOD_WIN;  // 0x0008
    }
    return native;
}

inline UINT qKeyCodeToNative(Qt::Key key) {
    switch (key) {
    case Qt::Key_Escape:
        return VK_ESCAPE;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        return VK_TAB;
    case Qt::Key_Backspace:
        return VK_BACK;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return VK_RETURN;
    case Qt::Key_Insert:
        return VK_INSERT;
    case Qt::Key_Delete:
        return VK_DELETE;
    case Qt::Key_Pause:
        return VK_PAUSE;
    case Qt::Key_Print:
        return VK_PRINT;
    case Qt::Key_Clear:
        return VK_CLEAR;
    case Qt::Key_Home:
        return VK_HOME;
    case Qt::Key_End:
        return VK_END;
    case Qt::Key_Left:
        return VK_LEFT;
    case Qt::Key_Up:
        return VK_UP;
    case Qt::Key_Right:
        return VK_RIGHT;
    case Qt::Key_Down:
        return VK_DOWN;
    case Qt::Key_PageUp:
        return VK_PRIOR;
    case Qt::Key_PageDown:
        return VK_NEXT;
    case Qt::Key_F1:
        return VK_F1;
    case Qt::Key_F2:
        return VK_F2;
    case Qt::Key_F3:
        return VK_F3;
    case Qt::Key_F4:
        return VK_F4;
    case Qt::Key_F5:
        return VK_F5;
    case Qt::Key_F6:
        return VK_F6;
    case Qt::Key_F7:
        return VK_F7;
    case Qt::Key_F8:
        return VK_F8;
    case Qt::Key_F9:
        return VK_F9;
    case Qt::Key_F10:
        return VK_F10;
    case Qt::Key_F11:
        return VK_F11;
    case Qt::Key_F12:
        return VK_F12;
    case Qt::Key_F13:
        return VK_F13;
    case Qt::Key_F14:
        return VK_F14;
    case Qt::Key_F15:
        return VK_F15;
    case Qt::Key_F16:
        return VK_F16;
    case Qt::Key_F17:
        return VK_F17;
    case Qt::Key_F18:
        return VK_F18;
    case Qt::Key_F19:
        return VK_F19;
    case Qt::Key_F20:
        return VK_F20;
    case Qt::Key_F21:
        return VK_F21;
    case Qt::Key_F22:
        return VK_F22;
    case Qt::Key_F23:
        return VK_F23;
    case Qt::Key_F24:
        return VK_F24;
    case Qt::Key_Space:
        return VK_SPACE;
    case Qt::Key_Asterisk:
        return VK_MULTIPLY;
    case Qt::Key_Plus:
        return VK_ADD;
    case Qt::Key_Comma:
        return VK_SEPARATOR;
    case Qt::Key_Minus:
        return VK_SUBTRACT;
    case Qt::Key_Slash:
        return VK_DIVIDE;
    case Qt::Key_MediaNext:
        return VK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious:
        return VK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaPlay:
        return VK_MEDIA_PLAY_PAUSE;
    case Qt::Key_MediaStop:
        return VK_MEDIA_STOP;
    case Qt::Key_VolumeDown:
        return VK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:
        return VK_VOLUME_UP;
    case Qt::Key_VolumeMute:
        return VK_VOLUME_MUTE;

    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    case Qt::Key_A:
    case Qt::Key_B:
    case Qt::Key_C:
    case Qt::Key_D:
    case Qt::Key_E:
    case Qt::Key_F:
    case Qt::Key_G:
    case Qt::Key_H:
    case Qt::Key_I:
    case Qt::Key_J:
    case Qt::Key_K:
    case Qt::Key_L:
    case Qt::Key_M:
    case Qt::Key_N:
    case Qt::Key_O:
    case Qt::Key_P:
    case Qt::Key_Q:
    case Qt::Key_R:
    case Qt::Key_S:
    case Qt::Key_T:
    case Qt::Key_U:
    case Qt::Key_V:
    case Qt::Key_W:
    case Qt::Key_X:
    case Qt::Key_Y:
    case Qt::Key_Z:
        return key;

    default:
        return 0;
    }
}

// The following five functions are copied and modified from:
// https://github.com/Aoi-hosizora/OpenWithMenu/blob/master/OpenWithMenu/Utils.hpp#L229-L274

inline bool listRegKeyNames(const std::wstring &root_path, std::vector<std::wstring> *out) {
    const int MAX_REG_KEY_LENGTH = 512;
    HKEY root_key;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, root_path.c_str(), 0, KEY_ENUMERATE_SUB_KEYS, &root_key) != ERROR_SUCCESS) {
        *out = {};
        return false;
    }

    std::vector<std::wstring> key_names;
    wchar_t key_name[MAX_REG_KEY_LENGTH];
    DWORD key_name_size = sizeof(key_name) / sizeof(key_name[0]);

    int index = 0;
    DWORD cch = key_name_size;
    while (RegEnumKeyExW(root_key, index, key_name, &cch, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
        index++;
        cch = key_name_size;
        key_names.push_back(key_name);
    }

    RegCloseKey(root_key);
    *out = key_names;
    return true;
}

inline bool openRegKeyForValues(const std::wstring &key_path, HKEY *out) {
    return RegOpenKeyEx(HKEY_CURRENT_USER, key_path.c_str(), 0, KEY_QUERY_VALUE, out) == ERROR_SUCCESS;
}

inline std::wstring readRegSz(const HKEY &key, const std::wstring &name, const std::wstring &fallback) {
    const int MAX_REG_SZ_LENGTH = 2048;
    wchar_t value[MAX_REG_SZ_LENGTH];
    DWORD value_size = sizeof(value) / sizeof(value[0]);
    if (RegQueryValueEx(key, name.c_str(), nullptr, nullptr, (LPBYTE) value, &value_size) != ERROR_SUCCESS) {
        return fallback;
    }
    return value;
}

inline std::wstring trimWstring(const std::wstring &str, const std::vector<wchar_t> &chars) {
    std::wstring copy = str;
    auto contains = [](const std::vector<wchar_t> &chars, wchar_t given) -> bool {
        return std::find(chars.begin(), chars.end(), given) != chars.end();
    };
    copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [&](wchar_t ch) {
        return !contains(chars, ch);
    }));  // trim left
    copy.erase(std::find_if(copy.rbegin(), copy.rend(), [&](wchar_t ch) {
        return !contains(chars, ch);
    }).base(),
        copy.end());  // trim right
    return copy;
}

inline int parseStyleFromString(const std::wstring &s) {
    if (s == L"" || s == L"SW_HIDE" || s == L"0") {
        return SW_HIDE;
    }
    if (s == L"SW_SHOWNORMAL" || s == L"SW_NORMAL" || s == L"1") {
        return SW_SHOWNORMAL;
    }
    if (s == L"SW_SHOWMINIMIZED" || s == L"2") {
        return SW_SHOWMINIMIZED;
    }
    if (s == L"SW_SHOWMAXIMIZED" || s == L"SW_MAXIMIZE" || s == L"3") {
        return SW_SHOWMAXIMIZED;
    }
    if (s == L"SW_SHOWNOACTIVATE" || s == L"4") {
        return SW_SHOWNOACTIVATE;
    }
    if (s == L"SW_SHOW" || s == L"5") {
        return SW_SHOW;
    }
    if (s == L"SW_MINIMIZE" || s == L"6") {
        return SW_MINIMIZE;
    }
    if (s == L"SW_SHOWMINNOACTIVE" || s == L"7") {
        return SW_SHOWMINNOACTIVE;
    }
    if (s == L"SW_SHOWNA" || s == L"8") {
        return SW_SHOWNA;
    }
    if (s == L"SW_RESTORE" || s == L"9") {
        return SW_RESTORE;
    }
    if (s == L"SW_SHOWDEFAULT" || s == L"10") {
        return SW_SHOWDEFAULT;
    }
    if (s == L"SW_FORCEMINIMIZE" || s == L"SW_MAX" || s == L"11") {
        return SW_FORCEMINIMIZE;
    }
    return SW_HIDE;
}

#endif  // UTILS_HPP
