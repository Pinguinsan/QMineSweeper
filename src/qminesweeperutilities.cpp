/***********************************************************************
*    qminesweeperutilities.cpp                                         *
*    Namespace for general utilities used by classes in QMineSweeper   *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation for a QMineSweeperUtilities    *
*    namespace. QMineSweeperUtilities holds utilities shared by all    *
*    classes, such a delay functions, random functions, etc            *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qminesweeperutilities.h"
#include "globaldefinitions.h"

#include <QDateTime>
#include <QDir>

namespace QMineSweeperUtilities
{

    static QString installDirectory{""};
    static QString programSettingsDirectory{""};
    static QString logFileName{""};
    static QString configurationFilePath{""};
    static QString userConfigurationFilePath{""};

    #if defined(_WIN32)
        static QString bundledSettingsFilePath{":/qminesweeper-configurations/qminesweeper-defaults-win32.json"};
    #else
        static QString bundledSettingsFilePath{":/qminesweeper-configurations/qminesweeper-defaults-nix.json"};
    #endif

    QString getUserConfigurationFilePath()
    {
        if (!userConfigurationFilePath.isEmpty()) {
            return userConfigurationFilePath;
        }
        QString settingsDirectory{getProgramSettingsDirectory()};
    #if defined(_WIN32)
        return QString{settingsDirectory + "config\\settings.json"};
    #else
        return QString{settingsDirectory + "config/settings.json"};
    #endif

    }

    QString getConfigurationFilePath()
    {
        if (!configurationFilePath.isEmpty()) {
            return configurationFilePath;
        }
    #if defined(_WIN32)
        QString testString{getInstallDirectory() + "config\\settings.json"};
    #else
        QString testString{getInstallDirectory() + "config/settings.json"};
    #endif
        if (QFile(testString).exists()) {
            //Systemwide settings
            configurationFilePath = testString;
        } else {
            regenerateSystemwideSettingsFile(testString);
            configurationFilePath = testString;
        }
        return configurationFilePath;
    }

    void regenerateSystemwideSettingsFile(const QString &systemSettingsFilePath)
    {
        LOG_WARNING() << QString{"Systemwide settings file at %1 was not found, regenerating system defaults"}.arg(systemSettingsFilePath);
        QFile bundledSettingsFile{bundledSettingsFilePath};
        bool settingsFileOpen{bundledSettingsFile.open(QIODevice::OpenModeFlag::ReadOnly)};
        if (!settingsFileOpen) {
            throw std::runtime_error(QString{"Unable to open bundled settings file %1, the binary for QMineSweeper may be corrupt, a reinstall is recommended (errorString = %2)"}.arg(bundledSettingsFilePath, bundledSettingsFile.errorString()).toStdString());
        }
        QFile fileToCreate{systemSettingsFilePath};
        if (fileToCreate.exists()) {
            bool deleteFileSuccess{fileToCreate.remove()};
            if (!deleteFileSuccess) {
                throw std::runtime_error(QString{"Unable to delete corrupt file %1, the binary for QMineSweeper may be corrupt, a reinstall is recommended (errorString = %2)"}.arg(systemSettingsFilePath, fileToCreate.errorString()).toStdString());
            }
        }
        QStringList fileToCreateSplit{fileToCreate.fileName().split("/")};
        QString fileToCreateSplitPath{""};
        for (const auto &it : fileToCreateSplit) {
            if (it != fileToCreateSplit.at(fileToCreateSplit.size()-1)) {
                fileToCreateSplitPath += ("/" + it);
            }
        }
        QDir fileToCreateDirectory{fileToCreateSplitPath};
        if (!fileToCreateDirectory.exists()) {
            if (!fileToCreateDirectory.mkpath(".")) {
                throw std::runtime_error(QString{"Unable to create new application path %1, the permissions for the directory may be wrong, a reinstall is recommended"}.arg(fileToCreateSplitPath).toStdString());
            }
        }
        bool newSettingsFileOpen{fileToCreate.open(QIODevice::OpenModeFlag::WriteOnly)};
        if (!newSettingsFileOpen) {
            throw std::runtime_error(QString{"Unable to open new settings file %1, the binary for QMineSweeper may be corrupt, a reinstall is recommended (errorString = %2)"}.arg(systemSettingsFilePath, fileToCreate.errorString()).toStdString());
        }
        fileToCreate.write(bundledSettingsFile.readAll());
        LOG_INFO() << QString{"Systemwide settings file at %1 successfuly recreated from bundled default settings"}.arg(systemSettingsFilePath);

        /*
        QString userSettingsPath{QMineSweeperUtilities::getUserConfigurationFilePath()};
        QFile userFileToCreate{userSettingsPath};
        bool userSettingsFileOpen{userFileToCreate.open(QIODevice::WriteOnly)};
        if (!userSettingsFileOpen) {
            LOG_WARNING() << QString{"User settings directory at %1 could not be opened for writing"}.arg(userSettingsPath);
        } else {
            fileToCreate.write(bundledSettingsFile.readAll());
            LOG_INFO() << QString{"User settings file at %1 successfully recreated from bundled default settings"}.arg(userSettingsPath);
        }
        */

    }

    QString getProgramSettingsDirectory()
    {
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
                additionalSettingsPath += "\\AppData\\Local\\QMineSweeper\\";
            } else {
                additionalSettingsPath += "\\QMineSweeper\\";
            }
        #else
            additionalSettingsPath += "/.local/share/QMineSweeper/";
        #endif
        programSettingsDirectory = homeDirectory + additionalSettingsPath;
        return programSettingsDirectory;

    }

    QString getInstallDirectory()
    {
    #if defined(_WIN32)
        if (QSysInfo::windowsVersion() > QSysInfo::WinVersion::WV_VISTA) {
            return "C:\\Program Files (x86)\\QMineSweeper\\";
        } else {
            return "C:\\QMineSweeper\\";
        }
    #else
        return "/opt/QMineSweeper/";
    #endif
    }

    QString getOSVersion()
    {
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

    QString getBuildArchitecture()
    {
        return QSysInfo::buildCpuArchitecture();
    }

    QString getCurrentArchitecture()
    {
        return QSysInfo::currentCpuArchitecture();
    }

    void checkOrCreateProgramSettingsDirectory()
    {
        QString settings{getProgramSettingsDirectory()};
        QDir settingsDirectory{settings};
        if (settingsDirectory.exists()) {
            LOG_INFO() << QString{"Detected settings directory at %1"}.arg(settings);
        } else {
            if (settingsDirectory.mkpath(".")) {
                LOG_INFO() << QString{"Settings directory not found, created new directory at %1"}.arg(settings);
            } else {
                throw std::runtime_error(QString{"Settings directory not found, and one could not be created at %1"}.arg(settings).toStdString());
            }
        }
    }

    QString getLogFilePath()
    {
        if ((!programSettingsDirectory.isEmpty()) && (!logFileName.isEmpty())) {
            return programSettingsDirectory + logFileName;
        } else {
            QString log{getLogFileName()};
            QString settings{getProgramSettingsDirectory()};
            return log + settings;
        }
    }

    QString getLogFileName()
    {
        if (logFileName.isEmpty()) {
            QString currentDateTime{QDateTime::currentDateTime().toString()};
            QString  newDateTime{""};
            for (const auto &it : currentDateTime) {
                if ((it == ' ') || (it == ':')) {
                    newDateTime += '-';
                } else {
                    newDateTime += it.toLower();
                }
            }
            logFileName = QString{"%1-%2.log"}.arg(newDateTime, QS_NUMBER(randomBetween(0, 60000)));
        }
        return logFileName;
    }


    static std::unique_ptr<Random> randomDevice = std::unique_ptr<Random>{new Random()};

    Random::Random(std::mt19937::result_type seed) :
        m_randomEngine{seed}
    {

    }

    int Random::drawNumber(int min, int max)
    {
        return std::uniform_int_distribution<int>{min, max}(this->m_randomEngine);
    }

    int randomBetween(int lowLimit, int highLimit, bool lowInclusive, bool highInclusive)
    {
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

    int roundIntuitively(double numberToRound)
    {
        double tempContainer{numberToRound - static_cast<int>(numberToRound)};
        if (tempContainer >= 0.5) {
            return (static_cast<int>(numberToRound) + 1);
        } else {
            return static_cast<int>(numberToRound);
        }
    }

    std::string getPadding(size_t howMuch, char padChar)
    {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padChar;
        }
        return returnString;
    }

    std::string getPadding(size_t howMuch, const char *padString)
    {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padString;
        }
        return returnString;
    }

    std::string getPadding(size_t howMuch, const std::string &padString)
    {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padString;
        }
        return returnString;
    }

    int stringToInt(const std::string &str)
    {
        std::string copyString{""};
        std::copy_if(str.begin(), str.end(), std::back_inserter(copyString), [](char c) -> bool { return std::isdigit(c); });
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

    int stringToInt(const char *str)
    {
        return stringToInt(std::string{str});
    }

    bool endsWith(const std::string &stringToCheck, const std::string &matchString)
    {
        if (matchString.size() > stringToCheck.size()) {
            return false;
        }
        return std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
    }

    bool endsWith(const std::string &stringToCheck, char matchChar)
    {
        return endsWith(stringToCheck, std::string{1, matchChar});
    }

    std::string TStringFormat(const char *formatting)
    {
        return std::string{formatting};
    }

    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip)
    {
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
            returnString = returnString.substr(0, foundPosition) + returnString.substr(foundPosition+whatToStrip.length());
        }
        return returnString;
    }

    std::string stripFromString(const std::string &stringToStrip, char whatToStrip)
    {
        return stripFromString(stringToStrip, std::string(1, whatToStrip));
    }


    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip)
       {
           std::string returnString = stringToStrip;
           if (returnString.find(whatToStrip) == std::string::npos) {
               return returnString;
           }
           while (returnString.find(whatToStrip) != std::string::npos) {
               returnString = stripFromString(returnString, whatToStrip);
           }
           return returnString;
       }

       std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip)
       {
           return stripAllFromString(stringToStrip, std::string(1, whatToStrip));
       }

}
