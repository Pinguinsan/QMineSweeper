/***********************************************************************
*    qmsapplicationsettings.h                                          *
*    Class to hold default and user settings used in QMineSweeper      *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QmsApplicationSettings      *
*    class, as well as a QmsGlobalSettings namespace                   *
*    QmsGlobalSettings contains default settings, as well as           *
*    information about the program (author name, upstream url, etc)    *
*    QmsApplicationSettings holds settings such as board size,         *
*    icon sets, timers, difficulties, and other various items          *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMSAPPLICATIONSETTINGS_H
#define QMINESWEEPER_QMSAPPLICATIONSETTINGS_H

namespace QmsGlobalSettings
{
    extern const char *PROGRAM_NAME;
    extern const char *LONG_PROGRAM_NAME;
    extern const char *REMOTE_URL;
    extern const char *AUTHOR_NAME;
    extern const int SOFTWARE_MAJOR_VERSION;
    extern const int SOFTWARE_MINOR_VERSION;
    extern const int SOFTWARE_PATCH_VERSION;

    #if defined(__GNUC__)
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #elif defined(_MSC_VER)
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #else
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #endif
}


class QmsApplicationSettings
{
public:
    QmsApplicationSettings();
    int numberOfColumns() const;
    int numberOfRows() const;
    int audioVolume() const;

    void setNumberOfColumns(int columns);
    void setNumberOfRows(int rows);
    void setAudioVolume(int volume);

private:
    int m_numberOfColumns;
    int m_numberOfRows;
    int m_audioVolume;
};

#endif // QMINESWEEPER_QMSAPPLICATIONSETTINGS_H
