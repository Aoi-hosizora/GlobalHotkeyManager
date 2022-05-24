#ifndef HOTKEY_ITEM_H
#define HOTKEY_ITEM_H

#include <QtCore/QString>
#include <QtGui/QKeySequence>
#include <vector>

class HotkeyItem {
   public:
    HotkeyItem() = default;
    HotkeyItem(const HotkeyItem &) = default;
    ~HotkeyItem() = default;

    HotkeyItem(QString title, QKeySequence hotkey, QString op, QString file, QString param, QString dir)
        : title_(title), hotkey_(hotkey), op_(op), file_(file), param_(param), dir_(dir) { }

    QString title() { return title_; };
    QKeySequence hotkey() { return hotkey_; };
    QString op() { return op_; };
    QString file() { return file_; };
    QString param() { return param_; };
    QString dir() { return dir_; };

    QString toString();
    static bool readConfigsFromRegistry(std::vector<HotkeyItem> *);

   private:
    QString title_ = "";
    QKeySequence hotkey_ = 0;
    QString op_ = "";
    QString file_ = "";
    QString param_ = "";
    QString dir_ = "";
};

#endif  // HOTKEY_ITEM_H
