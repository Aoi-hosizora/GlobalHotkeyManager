#include "manager_config.h"

#include <QtCore/QStringList>
#include <qt_windows.h>
#include <string>

#include "utils.hpp"

QString ManagerConfig::registryPath(bool full) {
    if (full) {
        return "HKEY_CURRENT_USER\\SOFTWARE\\AoiHosizora\\GlobalHotkeyManager";
    }
    return "SOFTWARE\\AoiHosizora\\GlobalHotkeyManager";
}

bool ManagerConfig::readConfigFromRegistry(ManagerConfig *out) {
    std::wstring key_path = ManagerConfig::registryPath(false).toStdWString();
    HKEY key;
    if (!utils::openRegKeyForValues(key_path, &key)) {
        return false;
    }

    auto hotkey = QString::fromStdWString(utils::readRegSz(key, L"Hotkey", L"Ctrl+Shift+Alt+F12")).trimmed().replace("Win", "Meta");
    auto lang = QString::fromStdWString(utils::readRegSz(key, L"Lang", L"")).trimmed().toLower();
    auto hotkey_ = utils::ensureQKeySequence(QKeySequence::fromString(hotkey), Qt::ControlModifier + Qt::ShiftModifier + Qt::AltModifier + Qt::Key_F12);
    auto lang_ = ManagerConfig::Language::ENGLISH;
    if (lang == "zh" || lang == "zh_cn" || lang == "zh-cn") {
        lang_ = ManagerConfig::Language::CHINESE;
    }
    if (lang == "ja" || lang == "ja_jp" || lang == "ja-jp") {
        lang_ = ManagerConfig::Language::JAPANESE;
    }

    *out = ManagerConfig(hotkey_, lang_);
    RegCloseKey(key);
    return true;
}

QString HotkeyItem::toString() const {
    if (title_ == "" || hotkey_.isEmpty() || file_ == "") {
        return "<invalid hotkey setting>";
    }

    QStringList l;
    if (op_ != "" && op_ != "open") {
        l << QString("(%0)").arg(op_);
    }
    l << file_;
    if (param_ != "") {
        l << param_;
    }

    return QString("\"%0\" (%1)\n%2").arg(title_, hotkey_.toString(), l.join(" "));
}

bool operator==(const HotkeyItem &lhs, const HotkeyItem &rhs) {
    return lhs.title() == rhs.title() && lhs.hotkey() == rhs.hotkey() &&
           lhs.file() == rhs.file() && lhs.op() == rhs.op() && lhs.param() == rhs.param() && lhs.dir() == rhs.dir() && lhs.style() == rhs.style();
}

bool operator!=(const HotkeyItem &lhs, const HotkeyItem &rhs) {
    return !(lhs == rhs);
}

bool HotkeyItem::readItemsFromRegistry(std::vector<HotkeyItem> *out) {
    std::wstring root_path = ManagerConfig::registryPath(false).toStdWString();
    std::vector<std::wstring> key_names;
    if (!utils::listRegKeyNames(root_path, &key_names)) {
        return false;
    }

    *out = {};
    for (auto key_name : key_names) {
        std::wstring key_path = root_path + L"\\" + key_name;
        HKEY key;
        if (!utils::openRegKeyForValues(key_path, &key)) {
            continue;
        }

        auto title = QString::fromStdWString(utils::readRegSz(key, L"", L"")).trimmed();
        auto hotkey = QString::fromStdWString(utils::readRegSz(key, L"Hotkey", L"")).trimmed().replace("Win", "Meta");
        auto op = QString::fromStdWString(utils::readRegSz(key, L"Operation", L"open")).trimmed();
        auto file = QString::fromStdWString(utils::readRegSz(key, L"File", L"")).trimmed().replace("\"", "");
        auto param = QString::fromStdWString(utils::readRegSz(key, L"Parameter", L"")).trimmed();
        auto dir = QString::fromStdWString(utils::readRegSz(key, L"Directory", L"")).trimmed().replace("\"", "");
        auto style = QString::fromStdWString(utils::readRegSz(key, L"Style", L"SW_NORMAL")).trimmed();
        auto hotkey_ = utils::ensureQKeySequence(QKeySequence::fromString(hotkey));
        auto style_ = utils::parseStyleFromString(style.toStdWString());
        if (!title.isEmpty() && !hotkey_.isEmpty() && !file.isEmpty()) {
            out->push_back(HotkeyItem(title, hotkey_, op, file, param, dir, style_));
        }

        RegCloseKey(key);
    }
    return true;
}
