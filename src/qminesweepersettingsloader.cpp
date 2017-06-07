/***********************************************************************
*    qminesweepersettingsloader.cpp                                    *
*    Class to load settings used in QMineSweeper                       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QMineSweeperSettingsLoader*
*    namespace. QMineSweeperSettingsLoader loads configuration options *
*    for QMineSweeper, such as language, default board size, etc       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/


#include "qminesweepersettingsloader.h"

#include "qminesweeperstrings.h"
#include "qminesweeperapplicationsettings.h"

#include <QSettings>

const int QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS{9};
const int QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS{9};
const char *QMineSweeperSettingsLoader::s_NUMBER_OF_COLUMNS_KEY{"columns"};
const char *QMineSweeperSettingsLoader::s_NUMBER_OF_ROWS_KEY{"rows"};

const QMineSweeperSettingsLoader::SupportedLanguage QMineSweeperSettingsLoader::s_DEFAULT_LANGUAGE{QMineSweeperSettingsLoader::SupportedLanguage::English};

QMineSweeperSettingsLoader::QMineSweeperSettingsLoader(QObject *parent) :
    QObject{parent}
{

}

const char *QMineSweeperSettingsLoader::languageToString(QMineSweeperSettingsLoader::SupportedLanguage language)
{
    if (language == QMineSweeperSettingsLoader::SupportedLanguage::English) {
        return QMineSweeperStrings::ENGLISH_STRING;
    } else if (language == QMineSweeperSettingsLoader::SupportedLanguage::French) {
        return QMineSweeperStrings::FRENCH_STRING;
    } else if (language == QMineSweeperSettingsLoader::SupportedLanguage::Spanish) {
        return QMineSweeperStrings::SPANISH_STRING;
    } else if (language == QMineSweeperSettingsLoader::SupportedLanguage::Japanese) {
        return QMineSweeperStrings::JAPANESE_STRING;
    } else {
        Q_UNREACHABLE();
    }
}

int QMineSweeperSettingsLoader::DEFAULT_COLUMN_COUNT()
{
    return QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS;
}

int QMineSweeperSettingsLoader::DEFAULT_ROW_COUNT()
{
    return QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS;
}

void QMineSweeperSettingsLoader::saveApplicationSettings(const QMineSweeperApplicationSettings &settings)
{
    QSettings settingsSaver;
    settingsSaver.setValue(QMineSweeperSettingsLoader::s_NUMBER_OF_COLUMNS_KEY, settings.numberOfColumns());
    settingsSaver.setValue(QMineSweeperSettingsLoader::s_NUMBER_OF_ROWS_KEY, settings.numberOfRows());
    settingsSaver.sync();
}

QMineSweeperApplicationSettings QMineSweeperSettingsLoader::loadApplicationSettings()
{
    QSettings settingsLoader;
    int columns{settingsLoader.value(QMineSweeperSettingsLoader::s_NUMBER_OF_COLUMNS_KEY).toInt()};
    int rows{settingsLoader.value(QMineSweeperSettingsLoader::s_NUMBER_OF_ROWS_KEY).toInt()};
    QMineSweeperApplicationSettings settings;
    if (columns <= 0) {
        columns = QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS;
    }
    if (rows <= 0) {
        rows = QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS;
    }
    settings.setNumberOfColumns(columns);
    settings.setNumberOfRows(rows);
    return settings;
}

QMineSweeperSettingsLoader::SupportedLanguage QMineSweeperSettingsLoader::DEFAULT_LANGUAGE()
{
    return QMineSweeperSettingsLoader::s_DEFAULT_LANGUAGE;
}
