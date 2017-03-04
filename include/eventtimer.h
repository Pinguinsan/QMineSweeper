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
*    This file holds the declarations of an EventTimer template class  *
*    EventTimer is used to time events that need to be able to pause   *
*    The class also allows access to calculated out hours/minutes/etc, *
*    suitable for a running game timer (eg 00:01.295)                  *
*    The default clock type is std::chrono::steady_clock, but this can *
*    be changed during instantiation via the template parameter to any *
*    clock specified in the std::chrono namespace (ie system_clock)    *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_EVENTTIMER_H
#define QMINESWEEPER_EVENTTIMER_H

#include <chrono>
#include <string>
#include "qminesweeperutilities.h"

template <typename ClockType = std::chrono::steady_clock>
class EventTimer
{
    using platform_clock_t = ClockType;

public:
    EventTimer() :
        m_startTime{platform_clock_t::now()},
        m_endTime{platform_clock_t::now()},
        m_cacheStartTime{platform_clock_t::now()},
        m_totalTime{0},
        m_hours{0},
        m_minutes{0},
        m_seconds{0},
        m_milliseconds{0},
        m_isPaused{false}
    {

    }

    void start()
    {
        this->m_totalTime = 0;
        this->m_hours = 0;
        this->m_minutes = 0;
        this->m_seconds = 0;
        this->m_milliseconds = 0;
        this->m_startTime = platform_clock_t::now();
        this->m_cacheStartTime = platform_clock_t::now();
        this->m_isPaused = false;
    }

    void restart()
    {
        return this->start();
    }

    void pause()
    {
        this->m_isPaused = true;
    }

    void stop()
    {
        return this->pause();
    }

    void unpause()
    {
        this->m_isPaused = false;
    }

    void update()
    {
        using namespace QMineSweeperUtilities;
        if (!this->m_isPaused) {
            this->m_endTime = platform_clock_t::now();
            this->m_totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(this->m_endTime-this->m_startTime).count();
            this->m_rawTime = std::chrono::duration_cast<std::chrono::milliseconds>(this->m_endTime - this->m_startTime);
            this->m_hours = (this->m_totalTime/MILLISECONDS_PER_HOUR);
            this->m_minutes = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR)) / MILLISECONDS_PER_MINUTE;
            this->m_seconds = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR) - (this->m_minutes * MILLISECONDS_PER_MINUTE)) / MILLISECONDS_PER_SECOND;
            this->m_milliseconds = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR) - (this->m_minutes * MILLISECONDS_PER_MINUTE) - (this->m_seconds * MILLISECONDS_PER_SECOND));
        } else {
            this->m_startTime = platform_clock_t::now() - this->m_rawTime;
        }
    }

    long long int totalMicroseconds()
    {
        using namespace QMineSweeperUtilities;
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) * MICROSECONDS_PER_MILLISECOND);
    }

    long long int totalMilliseconds()
    {
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return this->m_totalTime;
    }

    long long int totalSeconds()
    {
        using namespace QMineSweeperUtilities;
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_SECOND);
    }

    long long int totalMinutes()
    {
        using namespace QMineSweeperUtilities;
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_MINUTE);
    }

    long long int totalHours()
    {
        using namespace QMineSweeperUtilities;
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_HOUR);
    }

    long long int totalTime()
    {
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return this->m_totalTime;
    }

    std::string toString(uint8_t millisecondDigits = 3)
    {
        if (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        std::string returnString{""};
        if (this->hours() != 0) {
            returnString = QMineSweeperUtilities::toString(this->hours()) + ':';
        }
        returnString += QMineSweeperUtilities::toString(this->minutes())
                        + ':'
                        + QMineSweeperUtilities::toString(this->seconds())
                        + '.'
                        + QMineSweeperUtilities::toString(this->milliseconds()).substr(0, millisecondDigits);

        return returnString;
    }

    long long int hours()
    {
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return this->m_hours;
    }

    long long int minutes()
    {
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return this->m_minutes;
    }

    long long int seconds()
    {
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return this->m_seconds;
    }

    long long int milliseconds()
    {
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return this->m_milliseconds;
    }

    inline bool isPaused() const
    {
        return this->m_isPaused;
    }

    inline bool isRunning() const
    {
        return !this->isPaused();
    }

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

    inline bool cacheIsValid()
    {
        return (std::chrono::duration_cast<std::chrono::milliseconds>(platform_clock_t::now() - this->m_cacheStartTime).count() < EventTimer::INVALIDATE_CACHE_TIMEOUT);
    }

    inline void validateCache()
    {
        this->m_cacheStartTime = platform_clock_t::now();
    }

    static const int INVALIDATE_CACHE_TIMEOUT{100};
};

#endif //QMINESWEEPER_EVENTTIMER_H
