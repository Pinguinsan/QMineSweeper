#include "QmsUtilities.hpp"
#include "GlobalDefinitions.hpp"
#include "QmsApplicationSettings.hpp"

#include <QDateTime>
#include <QByteArray>
#include <QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QUuid>

namespace QmsUtilities {

    static QString installDirectory{""};
    static QString programSettingsDirectory{""};
    static QString logFileName{""};
    const std::list<char> KNOWN_DIMENSIONS_SEPARATORS{'x', ','};

    bool toBool(const QString &str, bool *ok) {
        if ((str == "false") || (str == "0")) {
            if (ok) *ok = true;
            return false;
        } else if ((str == "true") || (str == "1")) {
            if (ok) *ok = true;
            return true;
        }
        if (ok) *ok = false;
        return false;
    }

    bool toBool(const std::string &str, bool *ok) {
        return toBool(QString::fromStdString(str), ok);
    }

    std::string stripLineEndings(const std::string &str) {
        std::string copyString{str};
        if (copyString.empty()) {
            return copyString;
        }
        if (copyString.length() >= 2) {
            if (endsWith(copyString, '\r') || endsWith(copyString, '\n')) {
                copyString.pop_back();
            }
            if (endsWith(copyString, '\r') || endsWith(copyString, '\n')) {
                copyString.pop_back();
            }
        } else if (copyString.length() >= 1) {
            if (endsWith(copyString, '\r') || endsWith(copyString, '\n')) {
                copyString.pop_back();
            }
        }
        return copyString;
    }

    bool clearDirectoryOfFiles(const QString &directoryPath) {
        QDir directory{directoryPath};
        if (!directory.exists()) {
            return false;
        }
        directory.setNameFilters({"*.*"});
        directory.setFilter(QDir::Files);
        for (auto &it : directory.entryList()) {
            directory.remove(it);
        }
        return true;
    }

    void checkOrCreateProgramLogDirectory() {
        std::vector<QString> toLogInfo{};
        QString logDirectoryString{QDir::tempPath() + '/' + QCoreApplication::applicationName()};
        QDir logDirectory{logDirectoryString};
        if (logDirectory.exists()) {
            clearDirectoryOfFiles(logDirectoryString);
            toLogInfo.push_back(QString{"Detected log directory at %1"}.arg(logDirectoryString));
            toLogInfo.emplace_back("Cleared stale log entries");
        } else {
            if (logDirectory.mkpath(".")) {
                toLogInfo.push_back(
                        QString{"Log directory not found, created new directory at %1"}.arg(logDirectoryString));
            } else {
                throw std::runtime_error(QString{"Log directory not found, and one could not be created at %1"}.arg(
                        logDirectoryString).toStdString());
            }
        }
        for (auto &it : toLogInfo) {
            LOG_INFO() << it;
        }
    }

    void checkOrCreateProgramSettingsDirectory() {
        QString settings{QmsUtilities::getProgramSettingsDirectory()};
        QDir settingsDirectory{settings};
        if (settingsDirectory.exists()) {
            LOG_INFO() << QString{"Detected settings directory at %1"}.arg(settings);
        } else {
            if (settingsDirectory.mkpath(".")) {
                LOG_INFO() << QString{"Settings directory not found, created new directory at %1"}.arg(settings);
            } else {
                throw std::runtime_error(
                        QString{"Settings directory not found, and one could not be created at %1"}.arg(
                                settings).toStdString());
            }
        }
    }

    QString getLogFilePath() {
        if (logFileName.isEmpty()) {
            QString log{getLogFileName()};
        }
        return QString{"%1/%2/%3"}.arg(QDir::tempPath(), QCoreApplication::applicationName(), logFileName);
    }

    QString getLogFileName() {
        if (logFileName.isEmpty()) {
            QString currentDateTime{QDateTime::currentDateTime().toString()};
            QString newDateTime{""};
            for (const auto &it : currentDateTime) {
                if ((it == ' ') || (it == ':')) {
                    newDateTime += '-';
                } else {
                    newDateTime += it.toLower();
                }
            }
            //logFileName = QString{"%1-%2.log"}.arg(newDateTime, QS_NUMBER(randomBetween(0, 60000)));
            logFileName = QString{"%1.log"}.arg(newDateTime);
        }
        return logFileName;
    }

    QString getProgramSettingsDirectory() {
        if (!programSettingsDirectory.isEmpty()) {
            return programSettingsDirectory;
        }
        QString homeDirectory{QDir::toNativeSeparators(QDir::homePath())};
        QDir baseDirectory{homeDirectory};
        if (!baseDirectory.exists()) {
            throw std::runtime_error("You don't exist, go away");
        }
        QString additionalSettingsPath{""};
#if defined(_WIN32)
        if (QSysInfo::windowsVersion() > QSysInfo::WinVersion::WV_VISTA) {
            additionalSettingsPath += (QString{R"(\AppData\Local\)"} + QmsGlobalSettings::LONG_PROGRAM_NAME + R"(\)");
        } else {
            additionalSettingsPath += (QString{R"(\)"} + QmsGlobalSettings::LONG_PROGRAM_NAME + R"(\)");
        }
#else
        additionalSettingsPath += (QString{"/.local/share/"} + QmsGlobalSettings::LONG_PROGRAM_NAME + "/");
#endif
        programSettingsDirectory = homeDirectory + additionalSettingsPath;
        return programSettingsDirectory;

    }

    QString getOSVersion() {
#if defined(_WIN32)
        if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_32s) {
            return "Windows 3.1";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_95) {
            return "Windows 95";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_98) {
            return "Windows 98";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_Me) {
            return "Windows Me";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_NT) {
            return "Windows NT";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_2000) {
            return "Windows 2000";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_XP) {
            return "Windows XP";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_2003) {
            return "Windows 2003";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_VISTA) {
            return "Windows Vista";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS7) {
            return "Windows 7";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS8) {
            return "Windows 8";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS8_1) {
            return "Windows 8.1";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS10) {
            return "Windows 10";
        } else {
            return "I dunno, maybe DOS or something";
        }
#elif defined(__linux__)
        return "Linux, or something";
#elif defined(__MAC_OSX__)
        if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_None) {
                return "Non Darwin-based MacOS";
            }else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_9) {
                return "MacOS 9";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_0) {
                return "MacOS 10.0 (Cheetah)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_1) {
                return "MacOS 10.1 (Puma)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_2) {
                return "MacOS 10.2 (Jaguar)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_3) {
                return "MacOS 10.3 (Panther)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_4) {
                return "MacOS 10.4 (Tiger)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_5) {
                return "MacOS 10.5 (Leopard)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_6) {
                return "MacOS 10.6 (Snow Leopard)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_7) {
                return "MacOS 10.7 (Lion)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_8) {
                return "MacOS 10.8 (Mountain Lion)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_9) {
                return "MacOS 10.9 (Mavericks)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_10) {
                return "MacOS 10.10 (Yosemite)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_11) {
                return "MacOS 10.11 (El Capitan)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_12) {
                return "MacOS 10.12 (Sierra)";
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_4_3) {
                return "iOS 4.3"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_5_0) {
                return "iOS 5.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_5_1) {
                return "iOS 5.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_6_0) {
                return "iOS 6.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_6_1) {
                return "iOS 6.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_7_0) {
                return "iOS 7.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_7_1) {
                return "iOS 7.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_0) {
                return "iOS 8.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_1) {
                return "iOS 8.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_2) {
                return "iOS 8.2"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_3) {
                return "iOS 8.3"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_4) {
                return "iOS 8.4"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
                return "iOS 9.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
                return "iOS 9.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
                return "iOS 9.2"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
                return "iOS 9.3"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_10_0) {
                return "iOS 10.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_TVOS_9_0) {
                return "tvOS 9.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_TVOS_9_1) {
                return "tvOS 9.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_TVOS_9_2) {
                return "tvOS 9.2"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_10_0) {
                return "tvOS 10.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_2_0) {
                return "watchOS 2.0"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_2_1) {
                return "watchOS 2.1"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_2_2) {
                return "watchOS 2.2"
            } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_3_0) {
                return "watchOS 3.0"
            } else {
                return "Unknown MacOS version";
            }
#else
            return "Unknown OS";
#endif
    }

    QString getBuildArchitecture() {
        return QSysInfo::buildCpuArchitecture();
    }

    QString getCurrentArchitecture() {
        return QSysInfo::currentCpuArchitecture();
    }

    static std::unique_ptr<Random> randomDevice = std::unique_ptr<Random>{new Random()};

    Random::Random(std::mt19937::result_type seed) :
            m_randomEngine{seed} {

    }

    int Random::drawNumber(int min, int max) {
        return std::uniform_int_distribution<int>{min, max}(this->m_randomEngine);
    }

    int randomBetween(int lowLimit, int highLimit, bool lowInclusive, bool highInclusive) {
        int low{lowInclusive ? lowLimit : lowLimit + 1};
        int high{highInclusive ? highLimit : highLimit - 1};
        return randomDevice->drawNumber(low, high);
    }

    void logString(const std::string &str) { std::cout << str << std::endl; }

    std::string toString(const std::string &str) { return str; }

    std::string toString(const char *str) { return static_cast<std::string>(str); }

    QString toQString(const std::string &convert) { return QString::fromStdString(convert); }

    QString toQString(const char *convert) { return QString::fromStdString(static_cast<std::string>(convert)); }

    QString toQString(const QString &convert) { return convert; }

    static QString PID{""};
    static QString processUUID{""};

    QString getPID() {
        if (PID.isEmpty()) {
            PID = QS_NUMBER(QCoreApplication::applicationPid());
        }
        return PID;
    }

    QString getProcessUUID() {
        if (processUUID.isEmpty()) {
            processUUID = QUuid::createUuid().toString();
        }
        return processUUID;
    }

    int roundIntuitively(double numberToRound) {
        double tempContainer{numberToRound - static_cast<int>(numberToRound)};
        if (tempContainer >= 0.5) {
            return (static_cast<int>(numberToRound) + 1);
        } else {
            return static_cast<int>(numberToRound);
        }
    }

    std::string getPadding(size_t howMuch, char padChar) {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padChar;
        }
        return returnString;
    }

    std::string getPadding(size_t howMuch, const char *padString) {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padString;
        }
        return returnString;
    }

    std::string getPadding(size_t howMuch, const std::string &padString) {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padString;
        }
        return returnString;
    }

    int stringToInt(const std::string &str) {
        std::string copyString{""};
        std::copy_if(str.begin(), str.end(), std::back_inserter(copyString),
                     [](char c) -> bool { return ::isdigit(c); });
        if (std::all_of(copyString.begin(), copyString.end(), [](char c) -> bool { return c == '0'; })) {
            return 0;
        }
        int returnValue{atoi(copyString.c_str())};
        if (!returnValue) {
            throw std::runtime_error("");
        } else {
            return returnValue;
        }
    }

    int stringToInt(const char *str) {
        return stringToInt(std::string{str});
    }

    bool endsWith(const std::string &stringToCheck, const std::string &matchString) {
        if (matchString.size() > stringToCheck.size()) {
            return false;
        }
        return std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
    }

    bool endsWith(const std::string &stringToCheck, char matchChar) {
        return endsWith(stringToCheck, std::string{1, matchChar});
    }

    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip) {
        std::string returnString{stringToStrip};
        if (returnString.find(whatToStrip) == std::string::npos) {
            return returnString;
        }
        size_t foundPosition{stringToStrip.find(whatToStrip)};
        if (foundPosition == 0) {
            returnString = returnString.substr(whatToStrip.length());
        } else if (foundPosition == (returnString.length() - whatToStrip.length())) {
            returnString = returnString.substr(0, foundPosition);
        } else {
            returnString =
                    returnString.substr(0, foundPosition) + returnString.substr(foundPosition + whatToStrip.length());
        }
        return returnString;
    }

    std::string stripFromString(const std::string &stringToStrip, char whatToStrip) {
        return stripFromString(stringToStrip, std::string(1, whatToStrip));
    }

    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip) {
        std::string returnString = stringToStrip;
        if (returnString.find(whatToStrip) == std::string::npos) {
            return returnString;
        }
        while (returnString.find(whatToStrip) != std::string::npos) {
            returnString = stripFromString(returnString, whatToStrip);
        }
        return returnString;
    }

    std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip) {
        return stripAllFromString(stringToStrip, std::string(1, whatToStrip));
    }

    std::string getSignalName(int signalNumber) {
#if defined(_WIN32)
#    if defined(_MSC_VER)
        return std::to_string(signalNumber);
#    else
        switch (signalNumber) {
            case SIGABRT_COMPAT :
            case SIGABRT        : return "Abnormal termination";
            case SIGFPE         : return "Floating-point error";
            case SIGILL         : return "Illegal instruction";
            case SIGINT         : return "CTRL+C signal";
            case SIGSEGV        : return "Illegal storage access";
            case SIGTERM        : return "Termination request";
            case SIGBREAK       : return "Break signal";
            default             : return "Unknown signal";
        };
#    endif //define(_MSC_VER)
#else
        return strsignal(signalNumber);
#endif //defined(_WIN32)
    }

    std::pair<int, int> tryParseDimensions(const std::string &maybeDimensions) {
        std::string stringCopy{maybeDimensions};
        std::transform(stringCopy.begin(), stringCopy.end(), stringCopy.begin(), ::tolower);
        char divider{'\0'};
        for (auto &it : KNOWN_DIMENSIONS_SEPARATORS) {
            if (stringCopy.find(it) != std::string::npos) {
                divider = it;
            }

        }
        if (divider == '\0') {
            return std::make_pair(-1, -1);
        }
        size_t foundDividerPosition{stringCopy.find(divider)};
        try {
            int maybeColumns{stringToInt(stringCopy.substr(0, foundDividerPosition).c_str())};
            int maybeRows{stringToInt(stringCopy.substr(foundDividerPosition + 1).c_str())};
            if ((maybeColumns == 0) || (maybeRows == 0)) {
                return std::make_pair(-1, -1);
            }
            return std::make_pair(maybeColumns, maybeRows);
        } catch (const std::exception &e) {
            (void) e;
            return std::make_pair(-1, -1);
        }
    }

    float tryParseMineRatio(const std::string &str) {
        float mineRatio{};
        try {
            mineRatio = std::stof(str);
        } catch (const std::exception &e) {
            (void) e;
            return mineRatio;
        }
        if (!QmsUtilities::isValidMineRatio(mineRatio)) {
            mineRatio = float{};
        }
        return mineRatio;
    }

    bool isValidMineRatio(float mineRatio) {
        return ((mineRatio >= 0.001) && (mineRatio < 1.0));
    }

    bool containsSeparator(const char *testString) {
        std::string copyString{testString};
        std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);

        for (auto &it : KNOWN_DIMENSIONS_SEPARATORS) {
            if (copyString.find(it) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    std::string boolToString(bool value) {
        return (value ? "true" : "false");
    }

    QString boolToQString(bool value) {
        return QString::fromStdString(boolToString(value));
    }

    QByteArray getFileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm) {
        QFile inputFile{fileName};
        if (!inputFile.exists()) {
            throw std::runtime_error(
                    QString{"In QmsUtilities::getFileChecksum(const QString &, QCryptographicHash::Algorithm): input file %1 does not exist"}.arg(
                            fileName).toStdString());
        }
        if (!inputFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
            throw std::runtime_error(
                    QString{"In QmsUtilities::getFileChecksum(const QString &, QCryptographicHash::Algorithm): could not open file %1"}.arg(
                            fileName).toStdString());
        }
        QCryptographicHash hash{hashAlgorithm};
        hash.addData(&inputFile);
        return hash.result();
    }

    QByteArray getFileChecksum(QIODevice *inputDevice, QCryptographicHash::Algorithm hashAlgorithm) {
        if (!inputDevice) {
            throw std::runtime_error(
                    "In QmsUtilities::getFileChecksum(QIODevice *, QCryptographicHash::Algorithm): input QIODevice is a nullptr");
        }
        if (!inputDevice->isOpen()) {
            if (!inputDevice->open(QIODevice::OpenModeFlag::ReadOnly)) {
                throw std::runtime_error(
                        "In QmsUtilities::getFileChecksum(QIODevice *, QCryptographicHash::Algorithm): could not open QIODevice");
            }
        }
        QCryptographicHash hash{hashAlgorithm};
        hash.addData(inputDevice);
        return hash.result();
    }

    QString getFileDirectoryPath(const QFile &file) {
        if (!file.exists()) {
            return QString{""};
        }
        QChar pathSeparator{'/'};
        if (looksLikeWindowsFilePath(file.fileName())) {
            pathSeparator = '\\';
        }
        QString tempString{file.fileName()};
        auto lastSeparator = tempString.lastIndexOf(pathSeparator);
        if (lastSeparator == -1) {
            return QString{""};
        }
        return tempString.mid(0, lastSeparator + 1);
    }

    QString getFileName(const QFile &file) {
        if (!file.exists()) {
            return QString{""};
        }
        QChar pathSeparator{'/'};
        if (looksLikeWindowsFilePath(file.fileName())) {
            pathSeparator = '\\';
        }
        QString tempString{file.fileName()};
        auto lastSeparator = tempString.lastIndexOf(pathSeparator);
        if (lastSeparator == -1) {
            return QString{""};
        }
        return tempString.mid(lastSeparator + 1);
    }

    bool looksLikeWindowsFilePath(const QString &path) {
        std::string copyString{path.toStdString()};
        auto windowsPathCount = std::count_if(copyString.begin(), copyString.end(), [](char c) { return c == '\\'; });
        auto nixPathCount = std::count_if(copyString.begin(), copyString.end(), [](char c) { return c == '/'; });
        return windowsPathCount > nixPathCount;
    }
}
