#ifndef HOTKEY_ITEM_H
#define HOTKEY_ITEM_H

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QKeySequence>
#include <vector>

class HotkeyItem {
   public:
    HotkeyItem() = default;
    HotkeyItem(const HotkeyItem &) = default;
    ~HotkeyItem() = default;

    HotkeyItem(QString title, QKeySequence hotkey, QString op, QString file, QString param, QString dir, int style)
        : title_(title), hotkey_(hotkey), op_(op), file_(file), param_(param), dir_(dir), style_(style) { }

    QString title() const { return title_; }
    QKeySequence hotkey() const { return hotkey_; }
    QString op() const { return op_; }
    QString file() const { return file_; }
    QString param() const { return param_; }
    QString dir() const { return dir_; }
    int style() const { return style_; }

    QString toString() const;
    static bool readConfigsFromRegistry(std::vector<HotkeyItem> *);

   private:
    QString title_ = "";
    QKeySequence hotkey_ = 0;  // empty
    QString op_ = "";          // open
    QString file_ = "";
    QString param_ = "";
    QString dir_ = "";
    int style_ = 0;  // SW_HIDE
};

#endif  // HOTKEY_ITEM_H
