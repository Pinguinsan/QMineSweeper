/***********************************************************************
*    main.cpp:                                                         *
*    Only exists to create the main window and set it along            *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <cctype>
#include <cstdio>
#include <cstdlib>

#if defined(_WIN32)
    #include <Windows.h>
#else
    #include <signal.h>
    #include <unistd.h>

#endif

#include <QApplication>
#include <QDesktopWidget>
#include <QString>
#include <QRect>

#include "mainwindow.h"
#include "qminesweeperbutton.h"
#include "qminesweepericons.h"
#include "qminesweeperstrings.h"
#include "qminesweepersoundeffects.h"
#include "qminesweeperutilities.h"
#include "gamecontroller.h"

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
 */

static const char *PROGRAM_NAME{"QMineSweeper"};
static const char *LONG_PROGRAM_NAME{"QMineSweeper"};
static const char *AUTHOR_NAME{"Tyler Lewis"};
static const int SOFTWARE_MAJOR_VERSION{0};
static const int SOFTWARE_MINOR_VERSION{2};
static const int SOFTWARE_PATCH_VERSION{0};

#if defined(__GNUC__)
    static const char *COMPILER_NAME{"g++"};
    static const int COMPILER_MAJOR_VERSION{__GNUC__};
    static const int COMPILER_MINOR_VERSION{__GNUC_MINOR__};
    static const int COMPILER_PATCH_VERSION{__GNUC_PATCHLEVEL__};
#elif defined(_MSC_VER)
    static const char *COMPILER_NAME{"msvc"};
    static const int COMPILER_MAJOR_VERSION{_MSC_VER};
    static const int COMPILER_MINOR_VERSION{0};
    static const int COMPILER_PATCH_VERSION{0};
#else
    static const char *COMPILER_NAME{"unknown"};
    static const int COMPILER_MAJOR_VERSION{0};
    static const int COMPILER_MINOR_VERSION{0};
    static const int COMPILER_PATCH_VERSION{0};
#endif

const std::list<const char *> HELP_SWITCHES{"-h", "--h", "-help", "--help"};
const std::list<const char *> VERSION_SWITCHES{"v", "-v", "--v", "-version", "--version"};

void displayHelp();
void displayVersion();
void interruptHandler(int signal);
void installSignalHandlers(void (*signalHandler)(int));

int main(int argc, char *argv[])
{
    using namespace QMineSweeperStrings;
    using namespace QMineSweeperUtilities;
    (void)LONG_PROGRAM_NAME;

    installSignalHandlers(interruptHandler);

     std::cout << std::endl;
     for (int i = 1; i < argc; i++) {
         if (isSwitch(argv[i], HELP_SWITCHES)) {
             displayHelp();
             return 0;
         } else if (isSwitch(argv[i], VERSION_SWITCHES)) {
             displayVersion();
             return 0;
         } else {
            std::cout << "Invalid switch : " << argv[i] << ", skipping" << std::endl;
         }
     }
     displayVersion();

    QApplication qApplication(argc, argv);
    std::shared_ptr<QMineSweeperIcons> qmsi{std::make_shared<QMineSweeperIcons>()};
    std::shared_ptr<QMineSweeperSoundEffects> qmsse{std::make_shared<QMineSweeperSoundEffects>()};
    std::shared_ptr<GameController> gameController{std::make_shared<GameController>()};
    std::shared_ptr<QDesktopWidget> qDesktopWidget{std::make_shared<QDesktopWidget>()};
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(qmsi, qmsse, gameController, qDesktopWidget)};
    gameController->bindMainWindow(mainWindow);
    mainWindow->setupNewGame();

    QObject::connect(&qApplication, SIGNAL(aboutToQuit()), mainWindow.get(), SLOT(onApplicationExit()));
    mainWindow->setWindowIcon(qmsi->MINE_ICON_72);
    mainWindow->setWindowTitle(MAIN_WINDOW_TITLE);
#if defined(__ANDROID__)
    mainWindow->showMaximized();
    mainWindow->setFixedSize(mainWindow->minimumSize());
    mainWindow->resizeResetIcon();
    mainWindow->centerAndFitWindow();
#else
    mainWindow->show();
    mainWindow->setFixedSize(mainWindow->minimumSize());
    mainWindow->resizeResetIcon();
    mainWindow->centerAndFitWindow();
#endif
    return qApplication.exec();
}

void displayHelp()
{
    std::cout << "Usage: " << PROGRAM_NAME << " [options]" << std::endl << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "    -h, --h, -help, --help: Display this help text" << std::endl;
    std::cout << "    -v, --v, -version, --version: Display the version" << std::endl;
}

void displayVersion()
{
    std::cout << PROGRAM_NAME << ", v" << SOFTWARE_MAJOR_VERSION << "." << SOFTWARE_MINOR_VERSION << "." << SOFTWARE_PATCH_VERSION << std::endl;
    std::cout << "Written by " << AUTHOR_NAME << std::endl;
    std::cout << "Built with " << COMPILER_NAME << ", v" << COMPILER_MAJOR_VERSION << "." << COMPILER_MINOR_VERSION << "." << COMPILER_PATCH_VERSION << ", " << __DATE__ << std::endl << std::endl;
}


void interruptHandler(int signalNumber)
{
#if defined(_WIN32)

#else
    if ((signalNumber == SIGUSR1) || (signalNumber == SIGUSR2) || (signalNumber == SIGCHLD)) {
        return;
    }
    std::cout << std::endl << "Caught signal " << signalNumber << " (" << std::strerror(errno) << "), exiting " << PROGRAM_NAME << std::endl;
    exit (signalNumber);
#endif
}

void installSignalHandlers(void (*signalHandler)(int))
{
#if defined(_WIN32)

#else
    static struct sigaction signalInterruptHandler;
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
#endif
}


