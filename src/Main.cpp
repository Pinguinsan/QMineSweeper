#include <iostream>
#include <sstream>
#include <memory>
#include <array>
#include <algorithm>

#if defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <QApplication>
#include <QRect>
#include <QDateTime>
#include <forward_list>
#include <signal.h>

#include "MainWindow.hpp"
#include "QmsButton.hpp"
#include "QmsIcons.hpp"
#include "QmsStrings.hpp"
#include "QmsSoundEffects.hpp"
#include "GameController.hpp"
#include "GlobalDefinitions.hpp"
#include "QmsApplicationSettings.hpp"
#include "StaticLogger.hpp"
#include "ProgramOption.hpp"

#include <getopt.h>

/*
 * The program is organized like this:
 *     MainWindow - Contains all UI elements, and handling of UI signals
 *     GameController - Single instance class that controls all of the game logic
 *     MineCoordinates - Representing the X,Y coordinates of a button
 *     MineCoordinatesHash - A functor hash function to be able to store the QMineSweeperButton in a std::unordered_map
 *     QMineSweeperButton - The actual button as it appears on the screen, inheriting from QPushButton
 *     QMineSweeperIcons - Single instance class that holds all of the common icons used
 *     QMineSweeperSounds - Single instance class that holds all of the common sounds used
 *     QMineSweeperStrings - Single instance class that holds all of the common strings used
 *     QMineSweeperUtilities - Collection of general utilities (random numbers, etc) used
 *     QMineSweeperSettingsLoader - Single instance class that holds all settings from a configuration file
 */

static const ProgramOption verboseOption       {'e', "verbose", no_argument, "Enable verbose logging"};
static const ProgramOption helpOption          {'h', "help", no_argument, "Display help text and exit"};
static const ProgramOption versionOption       {'v', "version", no_argument, "Display version text and exit"};
static const ProgramOption dimensionsOption    {'d', "dimensions", required_argument, "Specify startup game board size"};
static const ProgramOption mineRatioOption     {'r', "ratio", required_argument, "Specify decimal ratio to use for mines (between 0 and 1)"};

static struct option longOptions[]{
        verboseOption.toPosixOption(),
        helpOption.toPosixOption(),
        versionOption.toPosixOption(),
        dimensionsOption.toPosixOption(),
        mineRatioOption.toPosixOption(),
        {nullptr, 0, nullptr, 0}
};

template <typename T, size_t N> inline size_t constexpr arraySize(T (&)[N] ) { return N; }
static const size_t constexpr PROGRAM_OPTION_COUNT{arraySize(longOptions)-1};

static const std::array<const ProgramOption *, PROGRAM_OPTION_COUNT> programOptions {
        &verboseOption,
        &helpOption,
        &versionOption,
        &dimensionsOption,
        &mineRatioOption
};

void displayHelp();
void displayVersion();
void interruptHandler(int signalNumber);
void installSignalHandlers(void (*signalHandler)(int));
void globalLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
std::pair<int, int> tryParseDimensions(std::string str);
float tryParseMineRatio(std::string str);

static bool verboseLogging{false};
static std::string initialGameStateFile{""};

using namespace QmsStrings;
using namespace QmsGlobalSettings;

int main(int argc, char *argv[]) {
    installSignalHandlers(interruptHandler);
#if defined(USE_QT_LOG)
    qInstallMessageHandler(globalLogHandler);
#else
    StaticLogger::initializeInstance(globalLogHandler);
#endif //USE_QT_LOG
    QCoreApplication::setOrganizationName(AUTHOR_NAME);
    QCoreApplication::setOrganizationDomain(REMOTE_URL);
    QCoreApplication::setApplicationName(LONG_PROGRAM_NAME);

    QmsUtilities::checkOrCreateProgramLogDirectory();
    QmsUtilities::checkOrCreateProgramSettingsDirectory();



    int columnCount{-1};
    int rowCount{-1};
    bool columnsSetByCommandLine{false};
    float mineRatio{};
    bool mineRatioSetByCommandLine{false};
    std::pair<int, int> dimensions{-1, -1};

    int optionIndex{0};
    int currentOption{0};
    opterr = 0; //Force getopt_long to not print out error messages
    std::string shortOptions{ProgramOption::buildShortOptions(programOptions)};
    while ( (currentOption = getopt_long(argc, argv, shortOptions.c_str(), longOptions, &optionIndex)) != -1) {
        switch (currentOption) {
            case 'h':
                displayHelp();
                exit(EXIT_SUCCESS);
            case 'v':
                displayVersion();
                exit(EXIT_SUCCESS);
            case 'e':
                verboseLogging = true;
                LOG_DEBUG() << "Enabling verbose logging due to command line switch";
                break;
            case 'd':
                dimensions = tryParseDimensions(optarg);
                columnCount = dimensions.first;
                rowCount = dimensions.second;
                break;
            case 'r':
                mineRatio = tryParseMineRatio(optarg);
                break;
            default:
                LOG_WARNING() << QString{R"(Invalid switch "%1" detected, ignoring option)"}.arg(static_cast<char>(currentOption));
                break;
        };
    }

    displayVersion();
    LOG_INFO() << QString{"Using log file %1"}.arg(QmsUtilities::getLogFilePath());
    LOG_INFO() << QString{"Current PID: %1"}.arg(QmsUtilities::getPID());
    LOG_INFO() << QString{"Process UUID: %1"}.arg(QmsUtilities::getProcessUUID());

    if (!columnsSetByCommandLine) {
        for (int i = 1; i < argc; i++) {
            if (QmsUtilities::containsSeparator(argv[i])) {
                dimensions = tryParseDimensions(argv[i]);
                columnCount = dimensions.first;
                rowCount = dimensions.second;
            }
        }
    }

    if (rowCount == -1) {
        columnCount = QmsSettingsLoader::DEFAULT_COLUMN_COUNT;
        rowCount = QmsSettingsLoader::DEFAULT_ROW_COUNT;
    } else {
        columnsSetByCommandLine = true;
    }

    if (mineRatio != float{}) {
        mineRatioSetByCommandLine = true;
    }

    for (int i = 1; i < argc; i++) {
        if (QmsUtilities::endsWith(argv[i], QmsStrings::SAVED_GAME_FILE_EXTENSION)) {
            initialGameStateFile = argv[i];
        }
    }


    displayVersion();
    QmsApplicationSettings settings{QmsSettingsLoader::loadApplicationSettings()};


    if (!columnsSetByCommandLine) {
        if ((columnCount <= 0) || (rowCount <= 0)) {
            columnCount = QmsSettingsLoader::DEFAULT_COLUMN_COUNT;
            rowCount = QmsSettingsLoader::DEFAULT_ROW_COUNT;
        } else {
            columnCount = settings.numberOfColumns();
            rowCount = settings.numberOfRows();
        }
    }
    LOG_INFO() << QString{"Using log file %1"}.arg(QmsUtilities::getLogFilePath());
    //LOG_DEBUG() << QString{"Beginning game with dimensions (%1x%2)"}.arg(QS_NUMBER(columnCount), QS_NUMBER(rowCount));
    //TODO: Load language from config file

    QApplication qApplication{argc, argv};
    QmsIcons::initializeInstance();
    QmsSettingsLoader::initializeInstance(nullptr);
    GameController::initializeInstance(columnCount, rowCount);
#if defined(__ANDROID__)
    QmsSoundEffects::initializeInstance();
#else
    QmsSoundEffects::initializeInstance(settings.audioVolume());
#endif
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(QmsSettingsLoader::DEFAULT_LANGUAGE)};
    gameController->bindMainWindow(mainWindow);
    if (mineRatioSetByCommandLine) {
        LOG_INFO() << QString{R"(Using custom mine ratio %1)"}.arg(QS_NUMBER(mineRatio));
        gameController->setCustomMineRatio(mineRatio);
    }
    mainWindow->setupNewGame();

    QMainWindow::connect(&qApplication, SIGNAL(aboutToQuit()), mainWindow.get(), SLOT(onApplicationExit()));
    mainWindow->setWindowIcon(applicationIcons->MINE_ICON_72);
    mainWindow->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    mainWindow->show();
    mainWindow->centerAndFitWindow(true);
    mainWindow->resizeResetIcon();
    if (!initialGameStateFile.empty()) {
        auto result = gameController->loadGame(initialGameStateFile.c_str());
        if (result.first != LoadGameStateResult::Success) {
            QString errorString{};
            QMessageBox warningBox{};
            warningBox.setWindowTitle(QmsStrings::FAILED_TO_LOAD_GAME_STATE_TITLE);
            warningBox.setWindowIcon(applicationIcons->FACE_ICON_FROWNY);
            warningBox.setText(QMessageBox::tr(QString{QmsStrings::FAILED_TO_LOAD_GAME_STATE}.arg(result.second.c_str()).toStdString().c_str()));
            warningBox.exec();
        }
    }
    return qApplication.exec();
}

std::pair<int, int> tryParseDimensions(std::string str) {
    if (QmsUtilities::startsWith(str, '=')) {
        str.erase(0, 1);
    }

    auto returnValue = QmsUtilities::tryParseDimensions(str);
    if ( (returnValue.first == -1) && (returnValue.second == -1) ) {
        LOG_WARNING() << QString{R"(Invalid game dimensions argument "%1")"}.arg(str.c_str());
    }
    return returnValue;
}

float tryParseMineRatio(std::string str) {
    if (QmsUtilities::startsWith(str, '=')) {
        str.erase(0, 1);
    }
    auto returnValue = QmsUtilities::tryParseMineRatio(str);
    if (returnValue == float{}) {
        LOG_WARNING() << QString{R"(Invalid mine ratio argument "%1")"}.arg(str.c_str());
    }
    return returnValue;
}

void interruptHandler(int signalNumber) {
#if defined(_WIN32)
    std::cout << std::endl << "Caught signal " << signalNumber << " (" << QmsUtilities::getSignalName(signalNumber) << "), exiting " << PROGRAM_NAME << std::endl;
    exit (signalNumber);
#else
    if ((signalNumber == SIGUSR1) || (signalNumber == SIGUSR2) || (signalNumber == SIGCHLD)) {
        return;
    }
    std::cout << std::endl << "Caught signal " << signalNumber << " (" << QmsUtilities::getSignalName(signalNumber) << "), exiting " << PROGRAM_NAME << std::endl;
    exit (signalNumber);
#endif //defined(_WIN32)
}

void installSignalHandlers(void (*signalHandler)(int)) {
#if defined(_WIN32)
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
#else
    static struct sigaction signalInterruptHandler{};
    signalInterruptHandler.sa_handler = signalHandler;
    sigemptyset(&signalInterruptHandler.sa_mask);
    signalInterruptHandler.sa_flags = 0;
    sigaction(SIGHUP, &signalInterruptHandler, NULL);
    sigaction(SIGINT, &signalInterruptHandler, NULL);
    sigaction(SIGQUIT, &signalInterruptHandler, NULL);
    sigaction(SIGILL, &signalInterruptHandler, NULL);
    sigaction(SIGABRT, &signalInterruptHandler, NULL);
    sigaction(SIGFPE, &signalInterruptHandler, NULL);
    sigaction(SIGPIPE, &signalInterruptHandler, NULL);
    sigaction(SIGALRM, &signalInterruptHandler, NULL);
    sigaction(SIGTERM, &signalInterruptHandler, NULL);
    sigaction(SIGUSR1, &signalInterruptHandler, NULL);
    sigaction(SIGUSR2, &signalInterruptHandler, NULL);
    sigaction(SIGCHLD, &signalInterruptHandler, NULL);
    sigaction(SIGCONT, &signalInterruptHandler, NULL);
    sigaction(SIGTSTP, &signalInterruptHandler, NULL);
    sigaction(SIGTTIN, &signalInterruptHandler, NULL);
    sigaction(SIGTTOU, &signalInterruptHandler, NULL);
#endif //defined(_WIN32)
}

void displayVersion() {
    using namespace QmsUtilities;
    LOG_INFO() << QString{"%1, v%2.%3.%4"}.arg(PROGRAM_NAME, QS_NUMBER(SOFTWARE_MAJOR_VERSION),
                                               QS_NUMBER(SOFTWARE_MINOR_VERSION), QS_NUMBER(SOFTWARE_PATCH_VERSION));
    LOG_INFO() << QString{"Written by %1"}.arg(AUTHOR_NAME);
    LOG_INFO() << QString{"Built with %1 v%2.%3.%4, %5"}.arg(COMPILER_NAME, QS_NUMBER(COMPILER_MAJOR_VERSION),
                                                             QS_NUMBER(COMPILER_MINOR_VERSION),
                                                             QS_NUMBER(COMPILER_PATCH_VERSION), __DATE__);
    LOG_INFO() << QString{"Build architecture: %1"}.arg(getBuildArchitecture());
    LOG_INFO() << QString{"Current detected architecture: %1"}.arg(getCurrentArchitecture());
    LOG_INFO() << QString{"Detected OS version: %1"}.arg(getOSVersion());
}


void displayHelp()
{
    std::cout << QString{"Usage: %1 Option [=value]"}.arg(PROGRAM_NAME).toStdString() << std::endl;
    std::cout << "Options: " << std::endl;
    for (const auto &it : programOptions) {
        std::cout << "    -" << static_cast<char>(it->shortOption()) << ", --" << it->longOption() << ": " << it->description() << std::endl;
    }
}

#if defined(USE_QT_LOG)

template<typename StringType, typename FileStringType>
void logToFile(const StringType &str, const FileStringType &filePath) {
    using QmsUtilities::toStdString;
    using QmsUtilities::toQString;
    QFile qFile{filePath};
    if (qFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        if (qFile.write(toStdString(str).c_str(), toStdString(str).length()) == -1) {
            throw std::runtime_error(
                    QString{R"(Failed to log data "%1" to file "%2" (file was opened, but not writable, permission problem?))"}.arg(
                            toQString(str), toQString(filePath)).toStdString());
        }
    } else {
        throw std::runtime_error(
                QString{R"(Failed to log data "%1" to file "%2" (could not open file))"}.arg(toQString(str), toQString(
                        filePath)).toStdString());
    }

}

void globalLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg{msg.toLocal8Bit()};
    QString logContext{""};
    auto *outputStream = &std::cout;
    switch (type) {
        case QtDebugMsg:
            if (!verboseLogging) {
                return;
            }
            logContext = "D";
            outputStream = &std::cout;
            break;
        case QtInfoMsg:
            logContext = "I";
            outputStream = &std::clog;
            break;
        case QtWarningMsg:
            logContext = "W";
            outputStream = &std::cout;
            break;
        case QtCriticalMsg:
            logContext = "C";
            outputStream = &std::cerr;
            break;
        case QtFatalMsg:
            logContext = "F";
            outputStream = &std::cerr;
            abort();
    }
    QString logMessage{""};
    std::string coreLogMessage{QString{localMsg}.toStdString()};
    if (coreLogMessage.find('\"') == 0) {
        coreLogMessage = coreLogMessage.substr(1);
    }
    if (coreLogMessage.find_last_of('\"') == coreLogMessage.length() - 1) {
        coreLogMessage = coreLogMessage.substr(0, coreLogMessage.length() - 1);
    }
    //coreLogMessage.erase(std::remove_if(coreLogMessage.begin(), coreLogMessage.end(),[](char c) { return c == '\"'; }), coreLogMessage.end());
    if ((type == QtCriticalMsg) || (type == QtFatalMsg)) {
        logMessage = QString{"[%1][%2] %3 (%4:%5, %6)"}.arg(QDateTime::currentDateTime().time().toString(), logContext,
                                                             coreLogMessage.c_str(), context.file,
                                                             QS_NUMBER(context.line), context.function);
    } else {
        logMessage = QString{"[%1][%2] %3"}.arg(QDateTime::currentDateTime().time().toString(), logContext,
                                                 coreLogMessage.c_str());
    }
    bool addLineEnding{true};
    static const QList<const char *> LINE_ENDINGS{"\r\n", "\r", "\n", "\n\r"};
    for (const auto &it : LINE_ENDINGS) {
        if (logMessage.endsWith(it)) {
            addLineEnding = false;
        }
    }
    if (addLineEnding) {
        logMessage.append("\n");
    }
    if (outputStream) {
        *outputStream << logMessage.toStdString();
    }
    logToFile(logMessage.toStdString(), QmsUtilities::getLogFilePath());
    outputStream->flush();
}

#else
void prettyLogFunction(LogLevel logLevel, LogContext logContext, const std::string &str) {
#if defined(_WIN32)
    std::ostream *outputStream{nullptr};
    switch (logLevel) {
        case LogLevel::Debug:
            outputStream = &std::clog;
            break;
        case LogLevel::Info:
            outputStream = &std::cout;
            break;
        case LogLevel::Warn:
            outputStream = &std::cout;
            break;
        case LogLevel::Critical:
            outputStream = &std::cout;
            break;
        case LogLevel::Fatal:
            outputStream = &std::cerr;
            break;
    }
    *outputStream << str << std::endl;
#else
    ForegroundColor foregroundColor{ForegroundColor::FG_DEFAULT};
    BackgroundColor backgroundColor{BackgroundColor::BG_DEFAULT};
    FontAttribute fontAttribute{FontAttribute::FA_DEFAULT};
    std::ostream *outputStream{nullptr};
    switch (logLevel) {
        case LogLevel::Debug:
            outputStream = &std::clog;
            fontAttribute = FontAttribute::FA_DIM;
            break;
        case LogLevel::Info:
            outputStream = &std::cout;
            break;
        case LogLevel::Warn:
            outputStream = &std::cout;
            fontAttribute = FontAttribute::FA_BOLD;
            foregroundColor = ForegroundColor::FG_BLUE;
            break;
        case LogLevel::Critical:
            outputStream = &std::cout;
            fontAttribute = FontAttribute::FA_BOLD;
            foregroundColor = ForegroundColor::FG_LIGHT_RED;
            break;
        case LogLevel ::Fatal:
            outputStream = &std::cerr;
            fontAttribute = FontAttribute::FA_BOLD;
            foregroundColor = ForegroundColor::FG_RED;
            break;
    }
    PrettyPrinter prettyPrinter{foregroundColor, backgroundColor, outputStream};
    prettyPrinter.setFontAttributes(fontAttribute);
    prettyPrinter.println(str);
}
#endif //defined(_WIN32)

template<typename StringType, typename FileStringType>
void logToFile(const StringType &str, const FileStringType &filePath) {
    using QmsUtilities::toStdString;
    using QmsUtilities::toQString;
    std::fstream writeToFile{};
    writeToFile.open(std::ios::app)
    if (writeToFile.good()) {
        writeToFile.write(toStdString(str).c_str(), toStdString(str).length());
        if (!writeToFile.good()) {
            throw std::runtime_error(
                    QString{R"(Failed to log data "%1" to file "%2" (file was opened, but not writable, permission problem?))"}.arg(
                            toQString(str), toQString(filePath)).toStdString());
        }
    } else {
        throw std::runtime_error(
                QString{R"(Failed to log data "%1" to file "%2" (could not open file))"}.arg(toQString(str), toQString(
                        filePath)).toStdString());
    }
}

#endif //USE_QT_LOG



