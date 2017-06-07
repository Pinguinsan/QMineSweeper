/***********************************************************************
*    qmssettingsloader.h                                               *
*    Class to load settings used in QMineSweeper                       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QmsSettingsLoader           *
*    namespace. QmsSettingsLoader loads configuration options          *
*    for QMineSweeper, such as language, default board size, etc       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMSSETTINGSLOADER_H
#define QMINESWEEPER_QMSSETTINGSLOADER_H

#include <QObject>
#include <utility>

class QmsApplicationSettings;

class QmsSettingsLoader : public QObject
{
    Q_OBJECT
public:
    explicit QmsSettingsLoader(QObject *parent = nullptr);

    enum class SupportedLanguage {
        English,
        French,
        Spanish,
        Japanese
    };

    static int DEFAULT_COLUMN_COUNT();
    static int DEFAULT_ROW_COUNT();
    static QmsSettingsLoader::SupportedLanguage DEFAULT_LANGUAGE();
    static const char *languageToString(QmsSettingsLoader::SupportedLanguage language);
    static QmsApplicationSettings loadApplicationSettings();
    static void saveApplicationSettings(const QmsApplicationSettings &settings);
private:
    static const int s_DEFAULT_NUMBER_OF_COLUMNS;
    static const int s_DEFAULT_NUMBER_OF_ROWS;
    static const char *s_NUMBER_OF_COLUMNS_KEY;
    static const char *s_NUMBER_OF_ROWS_KEY;
    static const QmsSettingsLoader::SupportedLanguage s_DEFAULT_LANGUAGE;

public slots:
};

#endif // QMINESWEEPER_QMSSETTINGSLOADER_H
