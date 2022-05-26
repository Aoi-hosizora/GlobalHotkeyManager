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

class ManagerConfig {
   public:
    ManagerConfig() = default;
    ManagerConfig(const ManagerConfig &) = default;
    ~ManagerConfig() = default;

    ManagerConfig(QKeySequence hotkey, QString lang)
        : hotkey_(hotkey), lang_(lang), keyId_(0) { }

    QKeySequence hotkey() const { return hotkey_; }
    QString lang() const { return lang_; }
    int keyId() const { return keyId_; }
    void setKeyId(int keyId) { keyId_ = keyId; }

    static bool readConfigFromRegistry(ManagerConfig *);

   private:
    QKeySequence hotkey_;
    QString lang_;
    int keyId_;
};

#endif  // HOTKEY_ITEM_H
