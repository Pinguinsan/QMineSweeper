#ifndef QMINESWEEPER_QMSAPPLICATIONSETTINGS_HPP
#define QMINESWEEPER_QMSAPPLICATIONSETTINGS_HPP

namespace QmsGlobalSettings
{
    extern const char *PROGRAM_NAME;
	extern const char *PROGRAM_DESCRIPTION;
    extern const char *LONG_PROGRAM_NAME;
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
}


class QmsApplicationSettings
{
public:
    int numberOfColumns() const;
    int numberOfRows() const;
    int audioVolume() const;

    void setNumberOfColumns(int columns);
    void setNumberOfRows(int rows);
    void setAudioVolume(int volume);

private:
    int m_numberOfColumns;
    int m_numberOfRows;
    int m_audioVolume;
};

#endif // QMINESWEEPER_QMSAPPLICATIONSETTINGS_HPP
