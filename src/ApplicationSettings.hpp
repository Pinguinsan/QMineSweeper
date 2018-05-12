#ifndef QMINESWEEPER_APPLICATIONSETTINGS_HPP
#define QMINESWEEPER_APPLICATIONSETTINGS_HPP

#include <string>
#include <QtCore/QStringList>
#include "GlobalDefinitions.hpp"

class ApplicationSettings
{
public:
    enum class SupportedLanguage {
        Unknown,
        English,
        Japanese
    };

    enum class CommunicationMethod {
        SerialPortServer,
        NetworkServer
    };

    DEFAULT_CLASS_METHODS(ApplicationSettings);

    ApplicationSettings &saveToDisk();
    ApplicationSettings &loadFromDisk();
    static ApplicationSettings loadNewFromDisk();

    inline std::string linControllerSerialPort() const { return this->m_linControllerSerialPort; }
    inline void setLinControllerSerialPort(const std::string &serialPort) { this->m_linControllerSerialPort = serialPort; }

    inline SupportedLanguage applicationLanguage() const { return this->m_applicationLanguage; }
    inline void setApplicationLanguage(SupportedLanguage applicationLanguage) { this->m_applicationLanguage = applicationLanguage; }

    inline bool verboseLogging() const { return this->m_verboseLogging; }
    inline void setVerboseLogging(bool verboseLogging) { this->m_verboseLogging = verboseLogging; }

    inline std::string linCommunicationMethod() const { return this->m_linCommunicationMethod; }
    inline void setLinCommunicationMethod(const std::string &method) { this->m_linCommunicationMethod = method; }

    static SupportedLanguage parseApplicationLanguage(const std::string &applicationLanguage);
    static bool isValidLinCommunicationMethod(const std::string &method);

    static CommunicationMethod getCommunicationMethod(std::string method);
    static std::string supportedLanguageToString(SupportedLanguage language);

private:
    std::string m_linControllerSerialPort;
    std::string m_linCommunicationMethod;
    SupportedLanguage m_applicationLanguage;
    bool m_verboseLogging;
};

extern ApplicationSettings globalSettings;

namespace GlobalSettings
{
    extern const char *PROGRAM_NAME;
    extern const char *COMPANY_NAME;

    extern const char *PROGRAM_LONG_NAME;
    extern const char *PROGRAM_DESCRIPTION;
    extern const char *REMOTE_URL;
    extern const char *AUTHOR_NAME;
    extern const int SOFTWARE_MAJOR_VERSION;
    extern const int SOFTWARE_MINOR_VERSION;
    extern const int SOFTWARE_PATCH_VERSION;

    #if defined(__GNUC__)
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #elif defined(_MSC_VER)
    extern const char *COMPILER_NAME;
        extern const int COMPILER_MAJOR_VERSION;
        extern const int COMPILER_MINOR_VERSION;
        extern const int COMPILER_PATCH_VERSION;
        #else
        extern const char *COMPILER_NAME;
        extern const int COMPILER_MAJOR_VERSION;
        extern const int COMPILER_MINOR_VERSION;
        extern const int COMPILER_PATCH_VERSION;
        #endif

    enum SystemErrorCode : uint32_t {
        NoValidLinControllerSerialPort       = 0b00000001,
        ErrorInitializingLinController       = 0b00000010,
    };

    const char *getSystemErrorCodeString(GlobalSettings::SystemErrorCode errorCode);
}


#endif //QMINESWEEPER_APPLICATIONSETTINGS_HPP
