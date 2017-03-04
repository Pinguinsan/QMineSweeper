/***********************************************************************
*    eventtimer.h:                                                     *
*    Class EventTimer, for timing events and getting calculated times  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an EventTimer class           *
*    EventTimer is used to time events that need to be able to pause   *
*    The class also allows access to calculated out hours/minutes/etc, *
*    suitable for a running game timer (eg 00:01.295)                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_EVENTTIMER_H
#define QMINESWEEPER_EVENTTIMER_H

#include <chrono>
#include <string>
#include "qminesweeperutilities.h"


using platform_clock_t = std::chrono::steady_clock;

class EventTimer
{
public:
    EventTimer();

    void start();
    void restart();
    void pause();
    void stop();
    void unpause();
    void update();

    long long int totalMicroseconds();
    long long int totalMilliseconds();
    long long int totalSeconds();
    long long int totalMinutes();
    long long int totalHours();
    long long int totalTime();

    long long int hours();
    long long int minutes();
    long long int seconds();
    long long int milliseconds();
    bool isPaused() const;

    std::string toString(uint8_t millisecondDigits = 3);

private:
    std::chrono::time_point<platform_clock_t> m_startTime;
    std::chrono::time_point<platform_clock_t> m_endTime;
    std::chrono::time_point<platform_clock_t> m_cacheStartTime;
    std::chrono::milliseconds m_rawTime;
    long long int m_totalTime;
    long long int m_hours;
    long long int m_minutes;
    long long int m_seconds;
    long long int m_milliseconds;
    bool m_isPaused;

    bool cacheIsValid();
    void validateCache();

    static const int INVALIDATE_CACHE_TIMEOUT;
};

#endif //TJLUTILS_EVENTTIMER_H
