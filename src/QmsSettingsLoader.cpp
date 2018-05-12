#include "QmsSettingsLoader.hpp"
#include "QmsStrings.hpp"
#include "QmsApplicationSettings.hpp"
#include "GlobalDefinitions.hpp"

#include <QSettings>

const int QmsSettingsLoader::DEFAULT_COLUMN_COUNT{9};
const int QmsSettingsLoader::DEFAULT_ROW_COUNT{9};
const int QmsSettingsLoader::DEFAULT_AUDIO_VOLUME{20};
const char *QmsSettingsLoader::NUMBER_OF_COLUMNS_KEY{"columns"};
const char *QmsSettingsLoader::NUMBER_OF_ROWS_KEY{"rows"};
const char *QmsSettingsLoader::AUDIO_VOLUME_KEY{"volume"};

const QmsSettingsLoader::SupportedLanguage QmsSettingsLoader::DEFAULT_LANGUAGE{QmsSettingsLoader::SupportedLanguage::English};

QmsSettingsLoader *settingsLoader{nullptr};

QmsSettingsLoader::QmsSettingsLoader(QObject *parent) :
    QObject{parent}
{

}

const char *QmsSettingsLoader::languageToString(QmsSettingsLoader::SupportedLanguage language)
{
    if (language == QmsSettingsLoader::SupportedLanguage::English) {
        return QmsStrings::ENGLISH_STRING;
    } else if (language == QmsSettingsLoader::SupportedLanguage::Japanese) {
        return QmsStrings::JAPANESE_STRING;
    } else {
        Q_UNREACHABLE();
    }
}

void QmsSettingsLoader::saveApplicationSettings(const QmsApplicationSettings &settings) {
    QSettings settingsSaver;
    settingsSaver.setValue(QmsSettingsLoader::NUMBER_OF_COLUMNS_KEY, settings.numberOfColumns());
    settingsSaver.setValue(QmsSettingsLoader::NUMBER_OF_ROWS_KEY, settings.numberOfRows());
    settingsSaver.setValue(QmsSettingsLoader::AUDIO_VOLUME_KEY, settings.audioVolume());
    settingsSaver.sync();
    LOG_INFO() << "Successfully saved application settings";
}

QmsApplicationSettings QmsSettingsLoader::loadApplicationSettings() {
    QSettings settingsLoader;
    int columns{settingsLoader.value(QmsSettingsLoader::NUMBER_OF_COLUMNS_KEY).toInt()};
    int rows{settingsLoader.value(QmsSettingsLoader::NUMBER_OF_ROWS_KEY).toInt()};
    int volume{settingsLoader.value(QmsSettingsLoader::AUDIO_VOLUME_KEY).toInt()};
    QmsApplicationSettings settings{};
    if (columns <= 0) {
        columns = QmsSettingsLoader::DEFAULT_COLUMN_COUNT;
    }
    if (rows <= 0) {
        rows = QmsSettingsLoader::DEFAULT_ROW_COUNT;
    }
    if (volume < 0) {
        volume = QmsSettingsLoader::DEFAULT_AUDIO_VOLUME;
    }
    settings.setNumberOfColumns(columns);
    settings.setNumberOfRows(rows);
    settings.setAudioVolume(volume);
    return settings;
}

void QmsSettingsLoader::initializeInstance(QObject *parent) {
    if (settingsLoader == nullptr) {
        settingsLoader = new QmsSettingsLoader{parent};
    }
}
