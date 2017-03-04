/***********************************************************************
*    eventtimer.cpp:                                                   *
*    Class EventTimer, for timing events and getting calculated times  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of an EventTimer class         *
*    EventTimer is used to time events that need to be able to pause   *
*    The class also allows access to calculated out hours/minutes/etc, *
*    suitable for a running game timer (eg 00:01.295)                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "eventtimer.h"

const int EventTimer::INVALIDATE_CACHE_TIMEOUT{100};

EventTimer::EventTimer() :
    m_startTime{std::chrono::high_resolution_clock::now()},
    m_endTime{std::chrono::high_resolution_clock::now()},
    m_cacheStartTime{std::chrono::high_resolution_clock::now()},
    m_totalTime{0},
    m_hours{0},
    m_minutes{0},
    m_seconds{0},
    m_milliseconds{0},
    m_isPaused{false}
{

}

void EventTimer::restart()
{
    return start();
}

void EventTimer::start()
{
    this->m_totalTime = 0;
    this->m_hours = 0;
    this->m_minutes = 0;
    this->m_seconds = 0;
    this->m_milliseconds = 0;
    this->m_startTime = std::chrono::high_resolution_clock::now();
    this->m_cacheStartTime = std::chrono::high_resolution_clock::now();
    this->m_isPaused = false;
}

bool EventTimer::cacheIsValid()
{
    return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->m_cacheStartTime).count() < EventTimer::INVALIDATE_CACHE_TIMEOUT);
}

void EventTimer::validateCache()
{
    this->m_cacheStartTime = std::chrono::high_resolution_clock::now();
}

void EventTimer::pause()
{
    this->m_isPaused = true;
}

void EventTimer::stop()
{
    this->m_isPaused = true;
}

void EventTimer::unpause()
{
    this->m_isPaused = false;
}

long long int EventTimer::hours()
{
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return this->m_hours;
}

long long int EventTimer::minutes()
{
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return this->m_minutes;
}

long long int EventTimer::seconds()
{
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return this->m_seconds;
}

long long int EventTimer::milliseconds()
{
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return this->m_milliseconds;
}

bool EventTimer::isPaused() const
{
    return this->m_isPaused;
}

void EventTimer::update()
{
    using namespace QMineSweeperUtilities;
    if (!this->m_isPaused) {
        this->m_endTime = std::chrono::high_resolution_clock::now();
        this->m_totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(this->m_endTime-this->m_startTime).count();
        this->m_rawTime = std::chrono::duration_cast<std::chrono::milliseconds>(this->m_endTime - this->m_startTime);
        this->m_hours = (this->m_totalTime/MILLISECONDS_PER_HOUR);
        this->m_minutes = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR)) / MILLISECONDS_PER_MINUTE;
        this->m_seconds = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR) - (this->m_minutes * MILLISECONDS_PER_MINUTE)) / MILLISECONDS_PER_SECOND;
        this->m_milliseconds = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR) - (this->m_minutes * MILLISECONDS_PER_MINUTE) - (this->m_seconds * MILLISECONDS_PER_SECOND));
    } else {
        this->m_startTime = std::chrono::high_resolution_clock::now() - this->m_rawTime;
    }
}

long long int EventTimer::totalMicroseconds()
{
    using namespace QMineSweeperUtilities;
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return static_cast<long long int>(static_cast<double>(this->m_totalTime) * MICROSECONDS_PER_MILLISECOND);
}

long long int EventTimer::totalMilliseconds()
{
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return this->m_totalTime;
}

long long int EventTimer::totalSeconds()
{
    using namespace QMineSweeperUtilities;
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_SECOND);
}

long long int EventTimer::totalMinutes()
{
    using namespace QMineSweeperUtilities;
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_MINUTE);
}

long long int EventTimer::totalHours()
{
    using namespace QMineSweeperUtilities;
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_HOUR);
    
}

long long int EventTimer::totalTime()
{
    if  (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    return this->m_totalTime;
}

std::string EventTimer::toString()
{
    if (!this->cacheIsValid()) {
        this->validateCache();
        this->update();
    }
    std::string returnString{""};
    if (this->hours() == 0) {
        returnString = QMineSweeperUtilities::toString(this->minutes())
                       + ':'
                       + QMineSweeperUtilities::toString(this->seconds())
                       + '.'
                       + QMineSweeperUtilities::toString(this->milliseconds());
    } else {
        returnString = QMineSweeperUtilities::toString(this->hours())
                       + ':'
                       + QMineSweeperUtilities::toString(this->minutes())
                       + ':'
                       + QMineSweeperUtilities::toString(this->seconds())
                       + '.'
                       + QMineSweeperUtilities::toString(this->milliseconds());
    }
    return returnString;
}
