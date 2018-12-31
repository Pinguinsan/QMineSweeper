#include "ApplicationSettings.hpp"
#include "GlobalDefinitions.hpp"
#include "ApplicationUtilities.hpp"
#include <QtCore/QVariant>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include "ApplicationStrings.hpp"

#if defined(_WIN32)
#    define PLATFORM_CONFIG_FORMAT QSettings::Format::IniFormat
#else
#    define PLATFORM_CONFIG_FORMAT QSettings::Format::NativeFormat
#endif //defined(_WIN32)

ApplicationSettings globalSettings{};

namespace GlobalSettings {
    const char *PROGRAM_NAME{"QMineSweeper"};
    const char *COMPANY_NAME{"Tyler Lewis"};
    const char *PROGRAM_LONG_NAME{"QMineSweeper"};
    const char *PROGRAM_DESCRIPTION{"An application for creating and viewing LIN bus traffic"};
    const char *REMOTE_URL{""};
    const char *AUTHOR_NAME{"Tyler Lewis"};
    const int SOFTWARE_MAJOR_VERSION{0};
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

    const char *getSystemErrorCodeString(GlobalSettings::SystemErrorCode errorCode) {
        if (errorCode == GlobalSettings::SystemErrorCode::NoValidLinControllerSerialPort) {
            return "No valid serial ports found for LinController";
        }
        if (errorCode == GlobalSettings::SystemErrorCode::ErrorInitializingLinController) {
            return "An error occurred while initializing LinController";
        }
        return "Unknown error";
    }

}

ApplicationSettings &ApplicationSettings::saveToDisk() {
    QSettings settingsSaver{PLATFORM_CONFIG_FORMAT, QSettings::UserScope, QCoreApplication::organizationName(),
                            QCoreApplication::applicationName(), nullptr};
    if (this->m_linControllerSerialPort.length() > 0) {
        settingsSaver.setValue(ApplicationStrings::LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY,
                               QVariant{this->m_linControllerSerialPort.c_str()});
        LOG_INFO() << QString{"Storing %1 value of %2"}.arg(ApplicationStrings::LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY,
                                                            this->m_linControllerSerialPort.c_str());
    } else {
        LOG_INFO() << QString{"Skipping %1 value, as it was never set by application"}.arg(
                ApplicationStrings::LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY);
    }
    if (this->m_applicationLanguage != SupportedLanguage::Unknown) {
        settingsSaver.setValue(ApplicationStrings::APPLICATION_LANGUAGE_SETTINGS_KEY,
                               QVariant{supportedLanguageToString(this->m_applicationLanguage).c_str()});
        LOG_INFO() << QString{"Storing %1 value of %2"}.arg(ApplicationStrings::APPLICATION_LANGUAGE_SETTINGS_KEY,
                                                            supportedLanguageToString(
                                                                    this->m_applicationLanguage).c_str());
    } else {
        LOG_INFO() << QString{"Skipping %1 value, as it was never set by application"}.arg(
                ApplicationStrings::APPLICATION_LANGUAGE_SETTINGS_KEY);
    }

    if (!this->m_linCommunicationMethod.empty()) {
        settingsSaver.setValue(ApplicationStrings::LIN_COMMUNICATION_METHOD_SETTINGS_KEY,
                               QVariant{this->m_linCommunicationMethod.c_str()});
        LOG_INFO() << QString{"Storing %1 value of %2"}.arg(ApplicationStrings::LIN_COMMUNICATION_METHOD_SETTINGS_KEY,
                                                            this->m_linCommunicationMethod.c_str());
    } else {
        LOG_INFO() << QString{"Skipping %1 value, as it was never set by application"}.arg(
                ApplicationStrings::LIN_COMMUNICATION_METHOD_SETTINGS_KEY);
    }

    settingsSaver.sync();
    LOG_INFO() << "Successfully saved (valid) application settings";

    return *this;
}

ApplicationSettings ApplicationSettings::loadNewFromDisk() {
    using namespace ApplicationStrings;
    /****Global Settings Loader Initialization****/
    QSettings globalSettingsLoader{PLATFORM_CONFIG_FORMAT, QSettings::Scope::SystemScope,
                                   QCoreApplication::organizationName(), QCoreApplication::applicationName(), nullptr};
    globalSettingsLoader.setFallbacksEnabled(false);
    LOG_INFO() << QString{"Using system-scope SettingsFile %1"}.arg(globalSettingsLoader.fileName());
    /****End Global Settings Loader Initialization****/

    /****Local Settings Loader Initialization****/
    QSettings settingsLoader{PLATFORM_CONFIG_FORMAT, QSettings::Scope::UserScope, QCoreApplication::organizationName(),
                             QCoreApplication::applicationName(), nullptr};
    settingsLoader.setFallbacksEnabled(false);
    LOG_INFO() << QString{"Using user-scope SettingsFile %1"}.arg(settingsLoader.fileName());
    /****End Local Settings Loader Initialization****/

    /****Load Temp Global Settings****/
    std::string globalLinControllerSerialPort{
            globalSettingsLoader.value(LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY).toString().toStdString()};
    std::string globalLanguage{globalSettingsLoader.value(APPLICATION_LANGUAGE_SETTINGS_KEY).toString().toStdString()};
    std::string globalLinCommunicationMethod{
            globalSettingsLoader.value(LIN_COMMUNICATION_METHOD_SETTINGS_KEY).toString().toStdString()};

    ApplicationSettings tempGlobalSettings{};
    tempGlobalSettings.setLinControllerSerialPort(globalLinControllerSerialPort);
    tempGlobalSettings.setLinCommunicationMethod(globalLinCommunicationMethod);
    /****End Load Temp Global Settings****/

    /****Load Local Settings****/
    ApplicationSettings settings{};
    std::string linControllerSerialPort{
            settingsLoader.value(LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY).toString().toStdString()};
    std::string language{settingsLoader.value(APPLICATION_LANGUAGE_SETTINGS_KEY).toString().toStdString()};
    std::string linCommunicationMethod{
            settingsLoader.value(LIN_COMMUNICATION_METHOD_SETTINGS_KEY).toString().toStdString()};
    /****End Load Local Settings****/

    if (tempGlobalSettings.linControllerSerialPort().length() > 0) {
        LOG_DEBUG() << QString{"Loaded %1 value of %2 from global settings file"}.arg(
                LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY, tempGlobalSettings.linControllerSerialPort().c_str());
    } else {
        LOG_DEBUG() << QString{"Skipped %1 setting, as it could not be loaded from the global settings file"}.arg(
                LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY);
    }

    if (globalLanguage.length() > 0) {
        LOG_DEBUG() << QString{"Loaded %1 value of %2 from global settings file"}.arg(APPLICATION_LANGUAGE_SETTINGS_KEY,
                                                                                      globalLanguage.c_str());
        SupportedLanguage supportedLanguage{ApplicationSettings::parseApplicationLanguage(globalLanguage)};
        if (supportedLanguage != SupportedLanguage::Unknown) {
            LOG_DEBUG() << QString{"%1 was parsed as a valid language from global settings file (%2)"}.arg(
                    globalLanguage.c_str(), ApplicationSettings::supportedLanguageToString(supportedLanguage).c_str());
            tempGlobalSettings.setApplicationLanguage(supportedLanguage);
        } else {
            LOG_DEBUG() << QString{"%1 was not parsed successfully from global settings file, and will be ignored"}.arg(
                    globalLanguage.c_str());
        }
    } else {
        LOG_DEBUG() << QString{"Skipped %1 setting, as it could not be loaded from the global settings file"}.arg(
                APPLICATION_LANGUAGE_SETTINGS_KEY);
    }
    if (tempGlobalSettings.linCommunicationMethod().length() > 0) {
        LOG_DEBUG()
                << QString{"Loaded %1 value of %2 from global settings file"}.arg(LIN_COMMUNICATION_METHOD_SETTINGS_KEY,
                                                                                  tempGlobalSettings.linCommunicationMethod().c_str());
    } else {
        LOG_DEBUG() << QString{"Skipped %1 setting, as it could not be loaded from the global settings file"}.arg(
                LIN_COMMUNICATION_METHOD_SETTINGS_KEY);
    }

    /****Apply Global Settings If User-Scope Setting Doesn't Exist****/
    if (!globalLinControllerSerialPort.empty()) {
        if (linControllerSerialPort.empty()) {
            LOG_DEBUG() << QString{"No user settings for %1 loaded, so global settings will be used"}.arg(
                    LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY);
            linControllerSerialPort = globalLinControllerSerialPort;
        } else {
            LOG_DEBUG()
                    << QString{"Global settings for %1 loaded, but user-scope will override (%2 will replace %3)"}.arg(
                            LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY, linControllerSerialPort.c_str(),
                            globalLinControllerSerialPort.c_str());
        }
    }
    if ((!globalLanguage.empty()) && (tempGlobalSettings.applicationLanguage() != SupportedLanguage::Unknown)) {
        SupportedLanguage checkParse{ApplicationSettings::parseApplicationLanguage(language)};
        if (language.empty()) {
            LOG_DEBUG() << QString{"No user settings for %1 loaded, so global settings will be used"}.arg(
                    APPLICATION_LANGUAGE_SETTINGS_KEY);
            language = globalLanguage;
        } else if (checkParse != SupportedLanguage::Unknown) {
            LOG_DEBUG()
                    << QString{"Global settings for %1 loaded, but user-scope will override (%2 will replace %3)"}.arg(
                            APPLICATION_LANGUAGE_SETTINGS_KEY, language.c_str(), globalLanguage.c_str());
        }
    }
    if (!globalLinCommunicationMethod.empty()) {
        if (linCommunicationMethod.empty()) {
            LOG_DEBUG() << QString{"No user settings for %1 loaded, so global settings will be used"}.arg(
                    LIN_COMMUNICATION_METHOD_SETTINGS_KEY);
            linCommunicationMethod = globalLinCommunicationMethod;
        } else {
            LOG_DEBUG()
                    << QString{"Global settings for %1 loaded, but user-scope will override (%2 will replace %3)"}.arg(
                            LIN_COMMUNICATION_METHOD_SETTINGS_KEY, linCommunicationMethod.c_str(),
                            globalLinCommunicationMethod.c_str());
        }
    }
    /****End Apply Global Settings If User-Scope Setting Doesn't Exist****/

    /****Local Settings Printing****/
    settings.setLinControllerSerialPort(linControllerSerialPort);
    settings.setLinCommunicationMethod(linCommunicationMethod);

    if (settings.linControllerSerialPort().length() > 0) {
        LOG_DEBUG() << QString{"Loaded %1 value of %2 from settings file"}.arg(LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY,
                                                                               settings.linControllerSerialPort().c_str());
    } else {
        LOG_DEBUG() << QString{"Skipped %1 setting, as it could not be loaded from the settings file"}.arg(
                LIN_CONTROLLER_SERIAL_PORT_SETTINGS_KEY);
    }
    if (language.length() > 0) {
        LOG_DEBUG() << QString{"Loaded %1 value of %2 from settings file"}.arg(APPLICATION_LANGUAGE_SETTINGS_KEY,
                                                                               language.c_str());
        SupportedLanguage supportedLanguage{ApplicationSettings::parseApplicationLanguage(language)};
        if (supportedLanguage != SupportedLanguage::Unknown) {
            LOG_DEBUG()
                    << QString{"%1 was parsed as a valid language from user settings file (%2)"}.arg(language.c_str(),
                                                                                                     ApplicationSettings::supportedLanguageToString(
                                                                                                             supportedLanguage).c_str());
            settings.setApplicationLanguage(supportedLanguage);
        } else {
            LOG_DEBUG() << QString{"%1 was not parsed successfully from user settings file, and will be ignored"}.arg(
                    language.c_str());
        }
    } else {
        LOG_DEBUG() << QString{"Skipped %1 setting, as it could not be loaded from the settings file"}.arg(
                APPLICATION_LANGUAGE_SETTINGS_KEY);
    }
    if (settings.linCommunicationMethod().length() > 0) {
        LOG_DEBUG() << QString{"Loaded %1 value of %2 from settings file"}.arg(LIN_COMMUNICATION_METHOD_SETTINGS_KEY,
                                                                               settings.linCommunicationMethod().c_str());
    } else {
        LOG_DEBUG() << QString{"Skipped %1 setting, as it could not be loaded from the settings file"}.arg(
                LIN_COMMUNICATION_METHOD_SETTINGS_KEY);
    }
    /****Local Settings Printing****/


    LOG_INFO() << "Successfully loaded (only valid) saved application settings";
    return settings;
}

std::string ApplicationSettings::supportedLanguageToString(SupportedLanguage language) {
    if (language == SupportedLanguage::English) {
        return "English";
    } else if (language == SupportedLanguage::Japanese) {
        return "Japanese";
    } else {
        return "Unknown";
    }
}

ApplicationSettings::SupportedLanguage
ApplicationSettings::parseApplicationLanguage(const std::string &applicationLanguage) {
    using namespace ApplicationStrings;
    using namespace ApplicationUtilities;
    std::string copyString{toLower(stripAllFromString(applicationLanguage, ' '))};
    std::string englishString{toLower(stripAllFromString(ENGLISH_STRING, ' '))};
    std::string japaneseString{toLower(stripAllFromString(JAPANESE_STRING, ' '))};
    if (copyString.find(englishString) == 0) {
        return SupportedLanguage::English;
    } else if (copyString.find(japaneseString) == 0) {
        return SupportedLanguage::Japanese;
    } else {
        return SupportedLanguage::Unknown;
    }
}

ApplicationSettings &ApplicationSettings::loadFromDisk() {
    auto settings = ApplicationSettings::loadNewFromDisk();
    *this = settings;
    return *this;
}

namespace {
    std::string toUpper(std::string input) {
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        return input;
    }
}

bool ApplicationSettings::isValidLinCommunicationMethod(const std::string &method) {
    auto compare = toUpper(method);
    return ((compare == "SERIAL") || (compare == "RS232") || (compare == "ETHERNET"));
}

ApplicationSettings::CommunicationMethod ApplicationSettings::getCommunicationMethod(std::string method) {
    auto compare = toUpper(method);
    if (compare == "ETHERNET") {
        return CommunicationMethod::NetworkServer;
    } else if ((compare == "SERIAL") || (compare == "RS232")) {
        return CommunicationMethod::SerialPortServer;
    } else {
        throw std::runtime_error("ApplicationSettings::getCommunicationMethod(): Invalid method \"" + method + "\"");
    }
}

