#ifndef QMINESWEEPER_GLOBALDEFINITIONS_HPP
#define QMINESWEEPER_GLOBALDEFINITIONS_HPP

#include <QDebug>
#include <QString>
#include <QtGlobal>

#if defined(USE_QT_LOG)
    #ifndef LOG_DEBUG
    #    define LOG_DEBUG(x) qDebug(x)
    #endif
    #ifndef LOG_INFO
    #    define LOG_INFO(x) qInfo(x)
    #endif
    #ifndef LOG_WARNING
    #    define LOG_WARNING(x) qWarning(x)
    #endif
    #ifndef LOG_CRITICAL
    #    define LOG_CRITICAL(x) qCritical(x)
    #endif
    #ifndef LOG_FATAL
    #    define LOG_FATAL(x) qFatal(x)
    #endif

    #ifndef QS_NUMBER
    #    define QS_NUMBER(x) QString::number(x)
    #endif
#else
    #ifndef LOG_DEBUG
    #    define LOG_DEBUG(x) Logger::createInstance(LogLevel::Debug, __FILE__, __LINE__, __func__)
    #endif //LOG_FATAL
    #ifndef LOG_WARN
    #    define LOG_WARN(x) Logger::createInstance(LogLevel::Warn, __FILE__, __LINE__, __func__)
    #endif //LOG_WARN
    #ifndef LOG_CRITICAL
    #    define LOG_CRITICAL(x) Logger::createInstance(LogLevel::Critical, __FILE__, __LINE__, __func__)
    #endif //LOG_CRITICAL
    #ifndef LOG_INFO
    #    define LOG_INFO(x) Logger::createInstance(LogLevel::Info, __FILE__, __LINE__, __func__)
    #endif //LOG_INFO
    #ifndef LOG_FATAL
    #    define LOG_FATAL(x) Logger::createInstance(LogLevel::Fatal, __FILE__, __LINE__, __func__)
    #endif //LOG_FATAL
#endif //defined(USE_QT_LOG)

#endif // QMINESWEEPER_GLOBALDEFINITIONS_HPP
