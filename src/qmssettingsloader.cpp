/***********************************************************************
*    qmssettingsloader.cpp                                             *
*    Class to load settings used in QMineSweeper                       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QmsSettingsLoader         *
*    namespace. QmsSettingsLoader loads configuration options          *
*    for QMineSweeper, such as language, default board size, etc       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qmssettingsloader.h"
#include "qmsstrings.h"
#include "qmsapplicationsettings.h"

#include <QSettings>

const int QmsSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS{9};
const int QmsSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS{9};
const char *QmsSettingsLoader::s_NUMBER_OF_COLUMNS_KEY{"columns"};
const char *QmsSettingsLoader::s_NUMBER_OF_ROWS_KEY{"rows"};

const QmsSettingsLoader::SupportedLanguage QmsSettingsLoader::s_DEFAULT_LANGUAGE{QmsSettingsLoader::SupportedLanguage::English};

QmsSettingsLoader::QmsSettingsLoader(QObject *parent) :
    QObject{parent}
{

}

const char *QmsSettingsLoader::languageToString(QmsSettingsLoader::SupportedLanguage language)
{
    if (language == QmsSettingsLoader::SupportedLanguage::English) {
        return QmsStrings::ENGLISH_STRING;
    } else if (language == QmsSettingsLoader::SupportedLanguage::French) {
        return QmsStrings::FRENCH_STRING;
    } else if (language == QmsSettingsLoader::SupportedLanguage::Spanish) {
        return QmsStrings::SPANISH_STRING;
    } else if (language == QmsSettingsLoader::SupportedLanguage::Japanese) {
        return QmsStrings::JAPANESE_STRING;
    } else {
        Q_UNREACHABLE();
    }
}

int QmsSettingsLoader::DEFAULT_COLUMN_COUNT()
{
    return QmsSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS;
}

int QmsSettingsLoader::DEFAULT_ROW_COUNT()
{
    return QmsSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS;
}

void QmsSettingsLoader::saveApplicationSettings(const QmsApplicationSettings &settings)
{
    QSettings settingsSaver;
    settingsSaver.setValue(QmsSettingsLoader::s_NUMBER_OF_COLUMNS_KEY, settings.numberOfColumns());
    settingsSaver.setValue(QmsSettingsLoader::s_NUMBER_OF_ROWS_KEY, settings.numberOfRows());
    settingsSaver.sync();
}

QmsApplicationSettings QmsSettingsLoader::loadApplicationSettings()
{
    QSettings settingsLoader;
    int columns{settingsLoader.value(QmsSettingsLoader::s_NUMBER_OF_COLUMNS_KEY).toInt()};
    int rows{settingsLoader.value(QmsSettingsLoader::s_NUMBER_OF_ROWS_KEY).toInt()};
    QmsApplicationSettings settings;
    if (columns <= 0) {
        columns = QmsSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS;
    }
    if (rows <= 0) {
        rows = QmsSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS;
    }
    settings.setNumberOfColumns(columns);
    settings.setNumberOfRows(rows);
    return settings;
}

QmsSettingsLoader::SupportedLanguage QmsSettingsLoader::DEFAULT_LANGUAGE()
{
    return QmsSettingsLoader::s_DEFAULT_LANGUAGE;
}
