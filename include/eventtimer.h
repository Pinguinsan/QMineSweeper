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

#ifndef TJLUTILS_EVENTTIMER_H
#define TJLUTILS_EVENTTIMER_H

#include <chrono>
#include <string>
#include <thread>
#include <future>
#include <list>
#include <functional>
#include "generalutilities.h"
#include "mathutilities.h"

template <typename ClockType = std::chrono::steady_clock>
class EventTimer
{
    using platform_clock_t = ClockType;
    using StandardFunctionCallback = std::function<void(long long int)>;
    using FunctionPointerCallback = void(*)(long long int);
    using CallbackHandlePair = std::pair<std::future<void>, StandardFunctionCallback>;

public:
    EventTimer<ClockType>() :
        m_startTime{platform_clock_t::now()},
        m_endTime{platform_clock_t::now()},
        m_cacheStartTime{platform_clock_t::now()},
        m_totalTime{0},
        m_hours{0},
        m_minutes{0},
        m_seconds{0},
        m_milliseconds{0},
        m_isPaused{false},
        m_monitoringAsyncHandle{nullptr},
        m_exitMonitorTicks{false}
    {

    }

    EventTimer<ClockType>(const EventTimer<ClockType> &other) :
        m_startTime{other.m_startTime},
        m_endTime{other.m_endTime},
        m_cacheStartTime{other.m_cacheStartTime},
        m_totalTime{other.m_totalTime},
        m_hours{other.m_hours},
        m_minutes{other.m_minutes},
        m_seconds{other.m_seconds},
        m_milliseconds{other.m_milliseconds},
        m_isPaused{other.m_isPaused},
        m_monitoringAsyncHandle{nullptr}
    {

    }

    void start()
    {
        if (!this->m_isPaused) {
            return;
        }
        this->m_totalTime = 0;
        this->m_hours = 0;
        this->m_minutes = 0;
        this->m_seconds = 0;
        this->m_milliseconds = 0;
        this->m_startTime = platform_clock_t::now();
        this->m_endTime = this->m_startTime;
        this->m_cacheStartTime = this->m_startTime;

        if (!this->m_monitoringAsyncHandle) {
            this->m_monitoringAsyncHandle = std::unique_ptr<std::future<void>>(new std::future<void>{std::async(std::launch::async, &EventTimer::monitorTicks, this)});
        }
        this->m_startTime = platform_clock_t::now();
        this->m_cacheStartTime = platform_clock_t::now();
        this->m_isPaused = false;
    }

    void restart() 
    { 
        this->m_isPaused = true;
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

    void monitorTicks()
    {
        long long int currentMilliseconds{this->milliseconds()};
        long long int currentSeconds{this->seconds()};
        long long int currentMinutes{this->minutes()};
        long long int currentHours{this->hours()};

        while (!this->m_exitMonitorTicks) {
            if (this->m_isPaused) {
                std::this_thread::sleep_for(this->getSleepDuration());
                std::this_thread::yield();
            }
            this->update();
            long long int updatedMilliseconds{this->milliseconds()};
            long long int updatedSeconds{this->seconds()};
            long long int updatedMinutes{this->minutes()};
            long long int updatedHours{this->hours()};
            if (updatedMilliseconds != currentMilliseconds) {
                currentMilliseconds = updatedMilliseconds;
                for (auto &it : this->m_registeredMillisecondsChangeCallback) {
                    if (it.first.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        it.first = std::async(std::launch::async, it.second, updatedMilliseconds);
                    }
                }
            }
            if (updatedSeconds != currentSeconds) {
                currentSeconds = updatedSeconds;
                for (auto &it : this->m_registeredSecondsChangeCallback) {
                    if (it.first.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        it.first = std::async(std::launch::async, it.second, updatedSeconds);
                    }
                }
            }
            if (updatedMinutes != currentMinutes) {
                currentMinutes = updatedMinutes;
                for (auto &it : this->m_registeredMinutesChangeCallback) {
                    if (it.first.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        it.first = std::async(std::launch::async, it.second, updatedMinutes);
                    }
                }
            }
            if (updatedHours != currentHours) {
                currentHours = updatedHours;
                for (auto &it : this->m_registeredHoursChangeCallback) {
                    if (it.first.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        it.first = std::async(std::launch::async, it.second, updatedHours);
                    }
                }
            } 
            std::this_thread::sleep_for(this->getSleepDuration());
            std::this_thread::yield();
        }
    }

    void update()
    {
        using namespace GeneralUtilities;
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

    std::chrono::microseconds getSleepDuration()
    {
        auto returnValue = std::chrono::microseconds(0);
        if (this->m_registeredMillisecondsChangeCallback.empty()) {
            returnValue = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(990));
        } else {
            returnValue = std::chrono::microseconds(990);
        }
        return returnValue;
    }

    long long int totalMicroseconds()
    {
        using namespace GeneralUtilities;
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
        using namespace GeneralUtilities;
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_SECOND);
    }

    long long int totalMinutes()
    {
        using namespace GeneralUtilities;
        if  (!this->cacheIsValid()) {
            this->validateCache();
            this->update();
        }
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_MINUTE);
    }

    long long int totalHours()
    {
        using namespace GeneralUtilities;
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
            returnString = GeneralUtilities::toString(this->hours()) + ':';
        }
        returnString += GeneralUtilities::toString(this->minutes())
                        + ':'
                        + GeneralUtilities::toString(this->seconds())
                        + '.'
                        + GeneralUtilities::toString(this->milliseconds()).substr(0, millisecondDigits);
        
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

    void registerMillisecondsChangedCallback(StandardFunctionCallback callback)
    {
        this->m_registeredMillisecondsChangeCallback.push_back(std::make_pair(std::future<void>(), callback));
    }

    void registerSecondsChangedCallback(StandardFunctionCallback callback)
    {
        this->m_registeredSecondsChangeCallback.push_back(std::make_pair(std::future<void>(), callback));
    }

    void registerMinutesChangedCallback(StandardFunctionCallback callback)
    {
        this->m_registeredMinutesChangeCallback.push_back(std::make_pair(std::future<void>(), callback));
    }

    void registerHoursChangedCallback(StandardFunctionCallback callback)
    {
        this->m_registeredHoursChangeCallback.push_back(std::make_pair(std::future<void>(), callback));
    }

    void registerMillisecondsChangedCallback(FunctionPointerCallback callback)
    {
        this->registerMillisecondsChangedCallback(static_cast<StandardFunctionCallback>(callback));
    }

    void registerSecondsChangedCallback(FunctionPointerCallback callback)
    {
        this->registerSecondsChangedCallback(static_cast<StandardFunctionCallback>(callback));
    }

    void registerMinutesChangedCallback(FunctionPointerCallback callback)
    {
        this->registerMinutesChangedCallback(static_cast<StandardFunctionCallback>(callback));
    }

    void registerHoursChangedCallback(FunctionPointerCallback callback)
    {
        this->registerHoursChangedCallback(static_cast<StandardFunctionCallback>(callback));
    }

    ~EventTimer<ClockType>()
    {
        if (this->m_monitoringAsyncHandle) {
            this->m_exitMonitorTicks = true;
            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            } while (this->m_monitoringAsyncHandle->wait_for(std::chrono::milliseconds(0)) != std::future_status::ready);
        }
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
    bool m_monitor;
    std::list<CallbackHandlePair> m_registeredMillisecondsChangeCallback;
    std::list<CallbackHandlePair> m_registeredSecondsChangeCallback;
    std::list<CallbackHandlePair> m_registeredMinutesChangeCallback;
    std::list<CallbackHandlePair> m_registeredHoursChangeCallback;
    std::unique_ptr<std::future<void>> m_monitoringAsyncHandle;
    bool m_exitMonitorTicks;

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

#endif //TJLUTILS_EVENTTIMER_H
