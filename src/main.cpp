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

#include <QApplication>
#include <QDesktopWidget>
#include <QString>
#include <QRect>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "mainwindow.h"
#include "qminesweeperbutton.h"
#include "qminesweepericons.h"
#include "qminesweeperstrings.h"
#include "qminesweepersoundeffects.h"
#include "gamecontroller.h"

int main(int argc, char *argv[])
{
    using namespace QMineSweeperStrings;
    QApplication qApplication(argc, argv);
    std::shared_ptr<QMineSweeperIcons> qmsi{std::make_shared<QMineSweeperIcons>()};
    std::shared_ptr<QMineSweeperSoundEffects> qmsse{std::make_shared<QMineSweeperSoundEffects>()};
    std::shared_ptr<GameController> gameController{std::make_shared<GameController>()};
    std::shared_ptr<QDesktopWidget> qDesktopWidget{std::make_shared<QDesktopWidget>()};
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(qmsi, qmsse, gameController, qDesktopWidget)};
    gameController->bindMainWindow(mainWindow);
    //mainWindow->bindGameController(gameController);
    //mainWindow->bindQDesktopWidget(qDesktopWidget);
    mainWindow->setupNewGame();

    QObject::connect(&qApplication, SIGNAL(aboutToQuit()), mainWindow.get(), SLOT(onApplicationExit()));
    mainWindow->setWindowIcon(mainWindow->qmsiPtr()->MINE_ICON_72);
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

