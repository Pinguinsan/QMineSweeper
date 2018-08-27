#ifndef QMINESWEEPER_EVENTTIMER_H
#define QMINESWEEPER_EVENTTIMER_H

#include <chrono>
#include <string>
#include <thread>
#include <future>
#include <sstream>
#include <list>
#include <functional>

class QmsGameState;

struct TimePoint {
    unsigned long hours;
    unsigned long minutes;
    unsigned long seconds;
    unsigned long milliseconds;
};

template<typename ClockType = std::chrono::steady_clock>
class EventTimer {
    friend class QmsGameState;

    using platform_clock_t = ClockType;

public:
    EventTimer<ClockType>() :
            m_startTime{platform_clock_t::now()},
            m_rawTime{},
            m_totalTime{0},
            m_hours{0},
            m_minutes{0},
            m_seconds{0},
            m_milliseconds{0},
            m_isPaused{false} {

    }

    EventTimer<ClockType>(const EventTimer<ClockType> &other) :
            m_startTime{other.m_startTime},
            m_rawTime{other.m_rawTime},
            m_totalTime{other.m_totalTime},
            m_hours{other.m_hours},
            m_minutes{other.m_minutes},
            m_seconds{other.m_seconds},
            m_milliseconds{other.m_milliseconds},
            m_isPaused{other.m_isPaused} {

    }

    template<typename T>
    static inline std::string toStdString(const T &item) {
        return dynamic_cast<std::stringstream &>(std::stringstream{} << item).str();
    }

    void start() {
        this->m_totalTime = 0;
        this->m_hours = 0;
        this->m_minutes = 0;
        this->m_seconds = 0;
        this->m_milliseconds = 0;
        this->m_startTime = platform_clock_t::now();
        this->m_isPaused = false;
    }

    void restart() {
        return this->start();
    }

    void pause() {
        this->stop();
    }

    void stop() {
        this->m_isPaused = true;
    }

    void resume() {
        this->m_isPaused = false;
    }

    void update() const {
        if (this->m_isPaused) {
            this->m_startTime = platform_clock_t::now() - this->m_rawTime;
        } else {
            this->calculateMembers();
        }
    }

    void calculateMembers() const {
        auto endTime = platform_clock_t::now();
        this->m_totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - this->m_startTime).count();
        this->m_rawTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - this->m_startTime);
        this->m_hours = (this->m_totalTime / MILLISECONDS_PER_HOUR);
        this->m_minutes = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR)) / MILLISECONDS_PER_MINUTE;
        this->m_seconds = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR) -
                           (this->m_minutes * MILLISECONDS_PER_MINUTE)) / MILLISECONDS_PER_SECOND;
        this->m_milliseconds = (this->m_totalTime - (this->m_hours * MILLISECONDS_PER_HOUR) -
                                (this->m_minutes * MILLISECONDS_PER_MINUTE) -
                                (this->m_seconds * MILLISECONDS_PER_SECOND));
    }

    long long int totalMicroseconds() {
        this->update();
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) * MICROSECONDS_PER_MILLISECOND);
    }

    TimePoint timePoint() {
        this->update();
        return TimePoint{
                static_cast<unsigned long>(this->m_hours),
                static_cast<unsigned long>(this->m_minutes),
                static_cast<unsigned long>(this->m_seconds),
                static_cast<unsigned long>(this->m_milliseconds)
        };
    }

    long long int totalMilliseconds() {
        this->update();
        return this->m_totalTime;
    }

    long long int totalSeconds() {
        this->update();
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_SECOND);
    }

    long long int totalMinutes() {
        this->update();
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_MINUTE);
    }

    long long int totalHours() {
        this->update();
        return static_cast<long long int>(static_cast<double>(this->m_totalTime) / MILLISECONDS_PER_HOUR);
    }

    long long int totalTime() {
        this->update();
        return this->m_totalTime;
    }

    std::string toString(uint8_t millisecondDigits = 3) const {
        if (!this->m_isPaused) {
            this->update();
        }
        std::string returnString{""};
        if (this->m_hours != 0) {
            returnString = toStdString(this->m_hours) + ':';
        }
        returnString += toStdString(this->m_minutes)
                        + ':'
                        + toStdString(this->m_seconds)
                        + '.';
        long long int millisecond{this->m_milliseconds};
        std::string millisecondsString{""};
        if (millisecond < 10) {
            millisecondsString = "00" + toStdString(millisecond);
        } else if (millisecond < 100) {
            millisecondsString = "0" + toStdString(millisecond);
        } else {
            millisecondsString = toStdString(millisecond);
        }
        returnString += millisecondsString.substr(0, millisecondDigits);

        return returnString;
    }

    long long int hours() {
        return this->m_hours;
    }

    long long int minutes() {
        return this->m_minutes;
    }

    long long int seconds() {
        return this->m_seconds;
    }

    long long int milliseconds() {
        return this->m_milliseconds;
    }

    inline bool isPaused() const {
        return this->m_isPaused;
    }

    inline bool isRunning() const {
        return (!this->isPaused());
    }

private:
    mutable std::chrono::time_point<platform_clock_t> m_startTime;
    mutable std::chrono::milliseconds m_rawTime;
    mutable long long int m_totalTime;
    mutable long long int m_hours;
    mutable long long int m_minutes;
    mutable long long int m_seconds;
    mutable long long int m_milliseconds;
    bool m_isPaused;

    static const int INVALIDATE_CACHE_TIMEOUT{100};

    static const long long int constexpr NANOSECONDS_PER_MICROSECOND{1000};
    static const long long int constexpr NANOSECONDS_PER_MILLISECOND{1000000};
    static const long long int constexpr NANOSECONDS_PER_SECOND{1000000000};
    static const long long int constexpr NANOSECONDS_PER_MINUTE{60000000000};
    static const long long int constexpr NANOSECONDS_PER_HOUR{3600000000000};
    static const long long int constexpr NANOSECONDS_PER_DAY{86400000000000};

    static const long long int constexpr MICROSECONDS_PER_SECOND{1000000};
    static const long long int constexpr MICROSECONDS_PER_MINUTE{60000000};
    static const long long int constexpr MICROSECONDS_PER_HOUR{3600000000};
    static const long long int constexpr MICROSECONDS_PER_DAY{86400000000};
    static const long long int constexpr MILLISECONDS_PER_SECOND{1000};
    static const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
    static const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
    static const long long int constexpr MILLISECONDS_PER_HOUR{3600000};
    static const long long int constexpr MILLISECONDS_PER_DAY{86400000};

    static const long long int constexpr SECONDS_PER_MINUTE{60};
    static const long long int constexpr SECONDS_PER_HOUR{3600};

    static const long long int constexpr MINUTES_PER_HOUR{60};
    static const long long int constexpr MINUTES_PER_DAY{1440};

    static const long long int constexpr HOURS_PER_DAY{24};

    void setTotalTime(long long int totalTime) {
        auto currentTime = platform_clock_t::now();
        this->m_startTime = currentTime - std::chrono::milliseconds(totalTime);
        this->calculateMembers();
    }

};

typedef EventTimer<std::chrono::steady_clock> SteadyEventTimer;
typedef EventTimer<std::chrono::high_resolution_clock> HighResolutionEventTimer;
typedef EventTimer<std::chrono::system_clock> SystemEventTimer;

#endif //QMINESWEEPER_EVENTTIMER_H
