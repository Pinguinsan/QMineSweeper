#ifndef QMINESWEEPER_QMSSETTINGSLOADER_HPP
#define QMINESWEEPER_QMSSETTINGSLOADER_HPP

#include <QObject>
#include <utility>

class QmsApplicationSettings;

class QmsSettingsLoader : public QObject {
Q_OBJECT
public:

    enum class SupportedLanguage {
        English,
        Japanese
    };

    static void initializeInstance(QObject *parent = nullptr);
    static const char *languageToString(QmsSettingsLoader::SupportedLanguage language);
    static QmsApplicationSettings loadApplicationSettings();
    static void saveApplicationSettings(const QmsApplicationSettings &settings);

    static const QmsSettingsLoader::SupportedLanguage DEFAULT_LANGUAGE;
    static const int DEFAULT_COLUMN_COUNT;
    static const int DEFAULT_ROW_COUNT;
    static const int DEFAULT_AUDIO_VOLUME;

private:
    static const char *NUMBER_OF_COLUMNS_KEY;
    static const char *NUMBER_OF_ROWS_KEY;
    static const char *AUDIO_VOLUME_KEY;

    explicit QmsSettingsLoader(QObject *parent = nullptr);
    QmsSettingsLoader(const QmsSettingsLoader &) = delete;
    QmsSettingsLoader &operator=(const QmsSettingsLoader &) = delete;
    ~QmsSettingsLoader() override = default;
};

extern QmsSettingsLoader *settingsLoader;

#endif // QMINESWEEPER_QMSSETTINGSLOADER_HPP
