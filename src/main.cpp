/***********************************************************************
*    main.cpp:                                                         *
*    Only exists to create the main window and set it along            *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
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
#include <signal.h>

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

static const char *PROGRAM_NAME = "QMineSweeper";
static const char *LONG_PROGRAM_NAME = "QMineSweeper";
static const char *AUTHOR_NAME = "Tyler Lewis";
static const int SOFTWARE_MAJOR_VERSION = 0;
static const int SOFTWARE_MINOR_VERSION = 2;
static const int SOFTWARE_PATCH_VERSION = 0;

#if defined(__GNUC__)
    static const int GCC_MAJOR_VERSION = __GNUC__;
    static const int GCC_MINOR_VERSION = __GNUC_MINOR__;
    static const int GCC_PATCH_VERSION = __GNUC_PATCHLEVEL__;
#else
    #error "The compiler must define __GNUC__ to use this program, but the compiler does not have it defined"
#endif

const std::list<const char *> HELP_SWITCHES{"-h", "--h", "-help", "--help"};
const std::list<const char *> VERSION_SWITCHES{"v", "-v", "--v", "-version", "--version"};

void displayHelp();
void displayVersion();
void interruptHandler(int signal);

int main(int argc, char *argv[])
{
    using namespace QMineSweeperStrings;
    using namespace QMineSweeperUtilities;
    (void)LONG_PROGRAM_NAME;

    //Signal handling stuff
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = interruptHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    //End signal handling

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
    std::cout << "Written by " << AUTHOR_NAME << ", " << __DATE__ << std::endl;
    std::cout << "Built with g++ v" << GCC_MAJOR_VERSION << "." << GCC_MINOR_VERSION << "." << GCC_PATCH_VERSION << ", " << __DATE__ << std::endl << std::endl;
}

void interruptHandler(int signalNumber)
{
    std::cout << std::endl << "Received signal " << signalNumber << " (" << strsignal(signalNumber) << "), exiting " << PROGRAM_NAME << std::endl;
    _Exit (signalNumber);
}

