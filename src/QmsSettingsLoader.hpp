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

#endif // QMINESWEEPER_QMSSETTINGSLOADER_H
