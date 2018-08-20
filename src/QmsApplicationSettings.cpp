#include "QmsApplicationSettings.hpp"

#if defined(_WIN32)
#    define PLATFORM_CONFIG_FORMAT QSettings::Format::IniFormat
#else
#    define PLATFORM_CONFIG_FORMAT QSettings::Format::NativeFormat
#endif //defined(_WIN32)

namespace QmsGlobalSettings {
    const char *PROGRAM_NAME{"QMineSweeper"};
    const char *LONG_PROGRAM_NAME{"QMineSweeper"};
    const char *PROGRAM_DESCRIPTION{"A recreation of the popular WIN32 game MineSweeper"};
    const char *REMOTE_URL{"https://github.com/tlewiscpp/QMineSweeper"};
    const char *AUTHOR_NAME{"Tyler Lewis"};
    const int SOFTWARE_MAJOR_VERSION{1};
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

}

void QmsApplicationSettings::setNumberOfColumns(int columns) {
    this->m_numberOfColumns = columns;
}

void QmsApplicationSettings::setNumberOfRows(int rows) {
    this->m_numberOfRows = rows;
}

void QmsApplicationSettings::setAudioVolume(int audioVolume) {
    this->m_audioVolume = audioVolume;
}

int QmsApplicationSettings::numberOfColumns() const {
    return this->m_numberOfColumns;
}

int QmsApplicationSettings::numberOfRows() const {
    return this->m_numberOfRows;
}

int QmsApplicationSettings::audioVolume() const {
    return this->m_audioVolume;
}