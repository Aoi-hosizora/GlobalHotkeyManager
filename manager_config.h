#ifndef MANAGER_CONFIG_H
#define MANAGER_CONFIG_H

#include <QtCore/QString>
#include <QtGui/QKeySequence>
#include <vector>

class ManagerConfig {
   public:
    enum class Language {
        ENGLISH,
        CHINESE,
        JAPANESE,
    };

    ManagerConfig() = default;
    ManagerConfig(const ManagerConfig &) = default;
    ~ManagerConfig() = default;

    ManagerConfig(QKeySequence hotkey, Language lang)
        : hotkey_(hotkey), lang_(lang) { }

    QKeySequence hotkey() const { return hotkey_; }
    Language lang() const { return lang_; }

    static QString registryPath(bool = true);
    static bool readConfigFromRegistry(ManagerConfig *);

   private:
    QKeySequence hotkey_;
    Language lang_;
};  // class ManagerConfig

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
    friend bool operator==(const HotkeyItem &, const HotkeyItem &);
    friend bool operator!=(const HotkeyItem &, const HotkeyItem &);
    static bool readItemsFromRegistry(std::vector<HotkeyItem> *);

   private:
    QString title_ = "";
    QKeySequence hotkey_ = 0;  // empty
    QString op_ = "";  // open
    QString file_ = "";
    QString param_ = "";
    QString dir_ = "";
    int style_ = 0;  // SW_HIDE
};  // class HotkeyItem

#endif  // MANAGER_CONFIG_H
