/***********************************************************************
*    qminesweepersettingsloader.h                                      *
*    Class to load settings used in QMineSweeper                       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QMineSweeperSettingsLoader  *
*    namespace. QMineSweeperSettingsLoader loads configuration options *
*    for QMineSweeper, such as language, default board size, etc       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPERSETTINGSLOADER_H
#define QMINESWEEPERSETTINGSLOADER_H

#include <QObject>

class QMineSweeperSettingsLoader : public QObject
{
    Q_OBJECT
public:
    explicit QMineSweeperSettingsLoader(QObject *parent = nullptr);

    enum class SupportedLanguage {
        English,
        French,
        Spanish,
        Japanese
    };

    static int DEFAULT_COLUMN_COUNT();
    static int DEFAULT_ROW_COUNT();
    static QMineSweeperSettingsLoader::SupportedLanguage DEFAULT_LANGUAGE();
    static const char *languageToString(QMineSweeperSettingsLoader::SupportedLanguage language);
private:
    static const int s_DEFAULT_NUMBER_OF_COLUMNS;
    static const int s_DEFAULT_NUMBER_OF_ROWS;
    static const QMineSweeperSettingsLoader::SupportedLanguage s_DEFAULT_LANGUAGE;

public slots:
};

#endif // QMINESWEEPERSETTINGSLOADER_H
