/***********************************************************************
*    qmsapplicationsettings.cpp                                        *
*    Class to hold default and user settings used in QMineSweeper      *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QmsApplicationSettings    *
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


#include "qmsapplicationsettings.h"

namespace QmsGlobalSettings
{

    const char *PROGRAM_NAME{"QMineSweeper"};
    const char *LONG_PROGRAM_NAME{"QMineSweeper"};
    const char *REMOTE_URL{"https://github.com/tlewiscpp/QMineSweeper"};
    const char *AUTHOR_NAME{"Tyler Lewis"};
    const int SOFTWARE_MAJOR_VERSION{1};
    const int SOFTWARE_MINOR_VERSION{1};
    const int SOFTWARE_PATCH_VERSION{0};

    #if defined(__GNUC__)
        const char *COMPILER_NAME{"g++"};
        const int COMPILER_MAJOR_VERSION{__GNUC__};
        const int COMPILER_MINOR_VERSION{__GNUC_MINOR__};
        const int COMPILER_PATCH_VERSION{__GNUC_PATCHLEVEL__};
    #elif defined(_MSC_VER)
        const char *COMPILER_NAME{"msvc"};
        const int COMPILER_MAJOR_VERSION{_MSC_VER};
        const int COMPILER_MINOR_VERSION{0};
        const int COMPILER_PATCH_VERSION{0};
    #else
        const char *COMPILER_NAME{"unknown"};
        const int COMPILER_MAJOR_VERSION{0};
        const int COMPILER_MINOR_VERSION{0};
        const int COMPILER_PATCH_VERSION{0};
    #endif

}

QmsApplicationSettings::QmsApplicationSettings()
{

}

void QmsApplicationSettings::setNumberOfColumns(int columns)
{
    this->m_numberOfColumns = columns;
}

void QmsApplicationSettings::setNumberOfRows(int rows)
{
    this->m_numberOfRows = rows;
}

int QmsApplicationSettings::numberOfColumns() const
{
    return this->m_numberOfColumns;
}

int QmsApplicationSettings::numberOfRows() const
{
    return this->m_numberOfRows;
}
