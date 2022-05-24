#include "hotkey_item.h"

#include <Windows.h>

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <string>

#include "utils.hpp"

QString HotkeyItem::toString() {
    if (title_ == "" || hotkey_.isEmpty() || file_ == "") {
        return "<invalid hotkey setting>";
    }

    QStringList l;
    if (op_ != "") {
        l << QString("(%0)").arg(op_);
    }
    l << file_;
    if (param_ != "") {
        l << param_;
    }

    return QString("%0 (%1)\n%2").arg(title_).arg(hotkey_.toString()).arg(l.join(" "));
    // return QString().asprintf("%s (%s) - %s", title, hotkey.toString(), l.join(" "));
}

const int MAX_REGKEY_LENGTH = 512;

bool HotkeyItem::readConfigsFromRegistry(std::vector<HotkeyItem> *out) {
    // get root key and sub keys
    std::wstring reg_root = L"SOFTWARE\\AoiHosizora\\GlobalHotkeyManager";
    HKEY root_key;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_root.c_str(), 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &root_key) != ERROR_SUCCESS) {
        *out = {};
        return false;
    }
    std::vector<std::wstring> sub_keys;
    wchar_t key_name[MAX_REGKEY_LENGTH];
    DWORD key_name_size = sizeof(key_name) / sizeof(key_name[0]);
    DWORD key_name_cch = key_name_size;
    int index = 0;
    while (RegEnumKeyExW(root_key, index, key_name, &key_name_cch, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
        index++;
        key_name_cch = key_name_size;
        sub_keys.push_back(key_name);
    }
    RegCloseKey(root_key);

    // check each sub keys
    *out = {};
    for (auto key : sub_keys) {
        std::wstring reg_sub = reg_root + L"\\" + key;
        qDebug() << QString::fromStdWString(reg_sub);
        HKEY sub_key;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_sub.c_str(), 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &sub_key) != ERROR_SUCCESS) {
            continue;
        }

        // get key's values
        auto title = readRegSz(sub_key, L"", L"");
        auto hotkey = readRegSz(sub_key, L"Hotkey", L"");
        auto op = readRegSz(sub_key, L"Operation", L"open");
        auto file = readRegSz(sub_key, L"File", L"");
        auto param = readRegSz(sub_key, L"Parameter", L"");
        auto dir = readRegSz(sub_key, L"Directory", L"%V");
        title = trimWstring(title, {L' '});
        hotkey = trimWstring(hotkey, {L' '});
        op = trimWstring(op, {L' '});
        file = trimWstring(file, {L' ', L'"'});
        param = trimWstring(param, {L' '});
        dir = trimWstring(dir, {L' ', L'"'});

        // save config
        auto q_title = QString::fromStdWString(title);
        auto q_hotkey = QKeySequence::fromString(QString::fromStdWString(hotkey));
        qDebug() << q_title << q_hotkey.toString();
        auto q_op = QString::fromStdWString(op);
        auto q_file = QString::fromStdWString(file);
        auto q_param = QString::fromStdWString(param);
        auto q_dir = QString::fromStdWString(dir);
        if (!q_title.isEmpty() && !q_hotkey.isEmpty() && !q_file.isEmpty()) {
            out->push_back(HotkeyItem(q_title, q_hotkey, q_op, q_file, q_param, q_dir));
        }

        RegCloseKey(sub_key);
    }

    return true;
}
