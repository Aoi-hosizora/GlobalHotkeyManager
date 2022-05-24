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

inline int generateHotkeyId(UINT fsModifiers, UINT vk) {
    return vk << 3 | fsModifiers;
}

// The following three functions are referred from:
// https://github.com/ddqd/qxtglobalshortcut5/blob/master/gui/qxtglobalshortcut_win.cpp#L39-L242

inline UINT qKeyboardModifiersToNative(Qt::KeyboardModifiers modifiers);
inline UINT qKeyCodeToNative(Qt::Key key);
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

// The following two functions are copied from:
// https://github.com/Aoi-hosizora/OpenWithMenu/blob/master/OpenWithMenu/Utils.hpp#L229-L274

inline std::wstring readRegSz(const HKEY &key, const std::wstring &name, const std::wstring &fallback) {
    const int MAX_REG_SZ_LENGTH = 2048;
    WCHAR buf[MAX_REG_SZ_LENGTH];
    DWORD buf_size = sizeof(buf) / sizeof(buf[0]);
    if (RegQueryValueEx(key, name.c_str(), nullptr, nullptr, (LPBYTE) buf, &buf_size) != ERROR_SUCCESS) {
        return fallback;
    }
    return buf;
}

inline std::wstring trimWstring(const std::wstring &str, const std::vector<wchar_t> &chars) {
    if (str.empty()) {
        return str;
    }
    std::wstring copy = str;

    auto exist_eq = [](const std::vector<wchar_t> chars, wchar_t given) -> bool {
        bool found = false;
        for (auto c : chars) {
            if (c == given) {
                found = true;
                break;
            }
        }
        return found;
    };

    bool need_trim_start = exist_eq(chars, copy.at(0));
    while (need_trim_start) {
        copy = copy.substr(1);
        need_trim_start = exist_eq(chars, copy.at(0));
    }
    bool need_trim_end = exist_eq(chars, copy.at(copy.length() - 1));
    while (need_trim_end) {
        copy = copy.substr(0, copy.length() - 1);
        need_trim_end = exist_eq(chars, copy.at(copy.length() - 1));
    }

    return copy;
}

#endif  // UTILS_HPP