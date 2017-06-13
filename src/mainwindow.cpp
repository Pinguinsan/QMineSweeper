/***********************************************************************
*    mainwindow.cpp:                                                   *
*    Main window for displaying the game                               *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source  file for QMineSweeper:                          *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a custom QMainWindow class  *
*    MainWindow is the default container for a Qt Desktop Application  *
*    The source code is released under the GNU LGPL                    *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <QMediaPlayer>
#include <QGridLayout>
#include <QLCDNumber>
#include <QDesktopWidget>
#include <QApplication>
#include <QWindow>
#include <QString>
#include <QRect>
#include <QTimer>
#include <QDialog>
#include <QActionGroup>
#include <QTranslator>
#include <QSettings>
#include <QDateTime>
#include <QSpacerItem>

#include <cctype>
#include <algorithm>

#include "qmsbutton.h"
#include "qmsicons.h"
#include "gamecontroller.h"
#include "aboutqmsdialog.h"
#include "boardresizedialog.h"
#include "qmssoundeffects.h"
#include "qmsutilities.h"
#include "qmsstrings.h"
#include "qmssettingsloader.h"
#include "globaldefinitions.h"
#include "qmsapplicationsettings.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_boardresizedialog.h"
#include "ui_aboutqmsdialog.h"

/* static const initializations */

#if defined(__ANDROID__)
    const int MainWindow::s_TASKBAR_HEIGHT{10};
    const int MainWindow::s_HEIGHT_SCALE_FACTOR{18};
    const int MainWindow::s_WIDTH_SCALE_FACTOR{28};
    const int MainWindow::s_NUMBER_OF_HORIZONTAL_MARGINS{2};
    const int MainWindow::s_NUMBER_OF_VERTIAL_MARGINS{4};
    const int MainWindow::s_DEFAULT_MINE_SIZE_SCALE_FACTOR{1};
    const double MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR{0.8};
#else
    const int MainWindow::s_TASKBAR_HEIGHT{10};
    const int MainWindow::s_HEIGHT_SCALE_FACTOR{18};
    const int MainWindow::s_WIDTH_SCALE_FACTOR{28};
    const int MainWindow::s_NUMBER_OF_HORIZONTAL_MARGINS{2};
    const int MainWindow::s_NUMBER_OF_VERTIAL_MARGINS{4};
    const int MainWindow::s_DEFAULT_MINE_SIZE_SCALE_FACTOR{19};
    const double MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR{0.75};
#endif

/* MainWindow() : Constructor, passing in shared_ptrs to all of
 * the relevant media and logic controllers. All UI stuff if initialized and
 * relevant events are hooked (QObject::this->connect()) to set up the game to play */
MainWindow::MainWindow(std::shared_ptr<QmsIcons> gameIcons,
                       std::shared_ptr<QmsSoundEffects> gameSoundEffects,
                       std::shared_ptr<QmsSettingsLoader> settingsLoader,
                       std::shared_ptr<GameController> gameController,
                       std::shared_ptr<QDesktopWidget> desktopWidget,
                       QmsSettingsLoader::SupportedLanguage initialDisplayLanguage,
                       QWidget *parent) :
    QMainWindow{parent},
    m_eventTimer{new QTimer{}},
    m_playTimer{new SteadyEventTimer{}},
    m_userIdleTimer{new SteadyEventTimer{}},
    m_ui{new Ui::MainWindow{}},
    m_aboutQmsDialog{new AboutQmsDialog{}},
    m_boardSizeDialog{new BoardResizeDialog{}},
    m_languageActionGroup{new QActionGroup{nullptr}},
    m_translator{new QTranslator{}},
    m_statusBarLabel{new QLabel{}},
    m_gameIcons{gameIcons},
    m_gameSoundEffects{gameSoundEffects},
    m_settingsLoader{settingsLoader},
    m_gameController{gameController},
    m_qDesktopWidget{desktopWidget},
    m_language{initialDisplayLanguage},
    m_xPlacement{0},
    m_yPlacement{0},
    m_currentDefaultMineSize{QSize{0,0}},
    m_currentMaxMineSize{QSize{0,0}},
    m_maxMineSizeCacheIsValid{false},
    m_tempPauseFlag{false},
    m_boardSizeGeometrySet{false}
{
    using namespace QmsStrings;
    this->m_ui->setupUi(this);

    this->m_ui->statusBar->addWidget(this->m_statusBarLabel.get());
    if (this->m_gameSoundEffects->isMuted()) {
        this->m_ui->actionMuteSound->setChecked(true);
    }

    this->setLanguage(this->m_language);
    this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_SMILEY);
    this->m_eventTimer->setInterval(this->m_gameController->GAME_TIMER_INTERVAL());

    this->connect(this->m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    this->connect(this->m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    this->connect(this->m_ui->actionAboutQt, &QAction::triggered, this, &MainWindow::onAboutQtActionTriggered);
    this->connect(this->m_ui->actionBoardSize, &QAction::triggered, this, &MainWindow::onChangeBoardSizeActionTriggered);
    this->connect(this->m_ui->actionMuteSound, &QAction::triggered, this, &MainWindow::onActionMuteSoundChecked);

    this->connect(this->m_ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
    this->connect(this->m_gameController.get(), &GameController::winEvent, this, &MainWindow::onGameWon);
    this->connect(this->m_gameController.get(), &GameController::readyToBeginNewGame, this, &MainWindow::setupNewGame);
    this->connect(this->m_gameController.get(), &GameController::userIsNoLongerIdle, this, &MainWindow::startUserIdleTimer);
    this->connect(this->m_gameController.get(), &GameController::gameStarted, this, &MainWindow::onGameStarted);

    this->connect(this->m_eventTimer.get(), &QTimer::timeout, this, &MainWindow::eventLoop);

    this->connect(this->m_gameController.get(), &GameController::numberOfMinesRemainingChanged, this, &MainWindow::updateNumberOfMinesLCD);
    this->connect(this->m_gameController.get(), &GameController::numberOfMovesMadeChanged, this, &MainWindow::updateNumberOfMovesMadeLCD);
    this->connect(this->m_gameController.get(), &GameController::gameResumed, this, &MainWindow::onGameResumed);
    this->connect(this->m_gameController.get(), &GameController::mineExplosionEvent, this, &MainWindow::onMineExplosionEventTriggered);

    this->connect(this->m_ui->menuFile, &QMenu::aboutToShow, this->m_gameController.get(), &GameController::onContextMenuActive);
    this->connect(this->m_ui->menuPreferences, &QMenu::aboutToShow, this->m_gameController.get(), &GameController::onContextMenuActive);
    this->connect(this->m_ui->menuHelp, &QMenu::aboutToShow, this->m_gameController.get(), &GameController::onContextMenuActive);

    this->connect(this->m_ui->menuFile, &QMenu::aboutToHide, this->m_gameController.get(), &GameController::onContextMenuInactive);
    this->connect(this->m_ui->menuPreferences, &QMenu::aboutToHide, this->m_gameController.get(), &GameController::onContextMenuInactive);
    this->connect(this->m_ui->menuHelp, &QMenu::aboutToHide, this->m_gameController.get(), &GameController::onContextMenuInactive);

    this->connect(this, &MainWindow::boardResize, this->m_gameController.get(), &GameController::onBoardResizeTriggered);
    this->connect(this, &MainWindow::mineDisplayed, this->m_gameController.get(), &GameController::onMineDisplayed);
    this->connect(this, &MainWindow::winEvent, this->m_gameController.get(), &GameController::onGameWon);
    this->connect(this, &MainWindow::gamePaused, this->m_gameController.get(), &GameController::onGamePaused);
    this->connect(this, &MainWindow::mineSweeperButtonCreated, this->m_gameController.get(), &GameController::onMineSweeperButtonCreated);
    this->connect(this, &MainWindow::resetGame, this->m_gameController.get(), &GameController::onGameReset);
    this->connect(this, &MainWindow::gameResumed, this->m_gameController.get(), &GameController::onGameResumed);
    this->connect(this, &MainWindow::mineExplosionEvent, this->m_gameController.get(), &GameController::onMineExplosionEventTriggered);

    this->m_languageActionGroup->addAction(this->m_ui->actionEnglish);
    this->m_languageActionGroup->addAction(this->m_ui->actionSpanish);
    this->m_languageActionGroup->addAction(this->m_ui->actionFrench);
    this->m_languageActionGroup->addAction(this->m_ui->actionJapanese);
    this->m_languageActionGroup->setExclusive(true);

    this->connect(this->m_ui->actionEnglish, &QAction::triggered, this, &MainWindow::onLanguageSelected);
    this->connect(this->m_ui->actionSpanish, &QAction::triggered, this, &MainWindow::onLanguageSelected);
    this->connect(this->m_ui->actionFrench, &QAction::triggered, this, &MainWindow::onLanguageSelected);
    this->connect(this->m_ui->actionJapanese, &QAction::triggered, this, &MainWindow::onLanguageSelected);

    std::unique_ptr<QRect> avail{new QRect{this->m_qDesktopWidget->availableGeometry()}};

    #if defined(__ANDROID__)
        this->m_reductionSizeScaleFactor = 1;
    #else
        if (avail->height() > 800) {
            this->m_reductionSizeScaleFactor = 0.7;
        } else {
            this->m_reductionSizeScaleFactor = 0.5;
        }
    #endif

    using namespace QmsGlobalSettings;

    /* initialize all strings and stuff for the BoardResizeWindow */
    this->m_boardSizeDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->m_boardSizeDialog->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    this->m_boardSizeDialog->setWindowIcon(this->m_gameIcons->MINE_ICON_72);
    this->connect(this->m_boardSizeDialog.get(), &BoardResizeDialog::aboutToClose, this, &MainWindow::onBoardResizeDialogClosed);

    this->connect(this->m_ui->actionAboutQMineSweeper, &QAction::triggered, this, &MainWindow::onAboutQMineSweeperActionTriggered);
    this->connect(this->m_aboutQmsDialog.get(), &AboutQmsDialog::aboutToClose, this, &MainWindow::onAboutQmsWindowClosed);

    this->m_eventTimer->start();
    this->updateNumberOfMovesMadeLCD(this->m_gameController->numberOfMovesMade());
    this->updateNumberOfMinesLCD(this->m_gameController->userDisplayNumberOfMines());

}

/* displayStatusMessage() : Called to include a space in the beginning
 * of the statusBar message, because without it, it is too closely aligned
 * to the corner of the window and doesn't look quite right */
void MainWindow::displayStatusMessage(QString statusMessage)
{
    this->m_statusBarLabel->setText(" " + statusMessage);
}

/* collectApplicationSettings() : Called when a the main windows is closing.
 * This method collects all of the application settings that may or may not
 * have been changed throughout the course of the gameplay, for purpose of
 * passing them to QMineSweeperSettingsLoader to write them to peristant storage. */
QmsApplicationSettings MainWindow::collectApplicationSettings() const
{
    QmsApplicationSettings returnSettings;
    returnSettings.setNumberOfColumns(this->m_gameController->numberOfColumns());
    returnSettings.setNumberOfRows(this->m_gameController->numberOfRows());
    returnSettings.setAudioVolume(this->m_gameSoundEffects->audioVolume());
    return returnSettings;
}

/* setLanguage() : Called when the UI must have it's language set (from
 * a button click on the main UI or from a configuration file).
 * First checks if the language is already set to the passed in language
 * parameter, and if it is not, sets the language and retranslates the UI*/
void MainWindow::setLanguage(QmsSettingsLoader::SupportedLanguage language)
{
    //TODO: Retranslate all non-ui set strings
    if (this->m_language == language) {
        //LOG_DEBUG() << QString{"Attempted to set language to %1, but that language is already loaded"}.arg(QmsSettingsLoader::languageToString(language));
    } else {
        const char *targetTranslationFile{};
        if (language == QmsSettingsLoader::SupportedLanguage::English) {
            targetTranslationFile = QmsStrings::ENGLISH_TRANSLATION_PATH;
        } else if (language == QmsSettingsLoader::SupportedLanguage::French) {
            targetTranslationFile = QmsStrings::FRENCH_TRANSLATION_PATH;
        } else if (language == QmsSettingsLoader::SupportedLanguage::Spanish) {
            targetTranslationFile = QmsStrings::SPANISH_TRANSLATION_PATH;
        } else if (language == QmsSettingsLoader::SupportedLanguage::Japanese) {
            targetTranslationFile = QmsStrings::JAPANESE_TRANSLATION_PATH;
        } else {
            LOG_DEBUG() << "Language action checked, but it was not a known QAction (this should never happen)";
            return;
        }
        this->m_translator->load(targetTranslationFile);
        this->m_language = language;
        this->displayStatusMessage(QStatusBar::tr(this->m_ui->statusBar->currentMessage().toStdString().c_str()));
        this->setWindowTitle(MainWindow::tr(this->windowTitle().toStdString().c_str()));
        this->m_ui->retranslateUi(this);
    }
}

/* onLanguageSelected() : Called when a new language is selected from the
 * preferences menu, causing a potential retranslations of the UI */
void MainWindow::onLanguageSelected(bool checked)
{
    (void)checked;
    QAction *checkedAction{this->m_languageActionGroup->checkedAction()};
    checkedAction->dumpObjectTree();
    checkedAction->dumpObjectInfo();
    if (checkedAction == this->m_ui->actionEnglish) {
        this->setLanguage(QmsSettingsLoader::SupportedLanguage::English);
    } else if (checkedAction == this->m_ui->actionFrench) {
        this->setLanguage(QmsSettingsLoader::SupportedLanguage::French);
    } else if (checkedAction == this->m_ui->actionSpanish) {
        this->setLanguage(QmsSettingsLoader::SupportedLanguage::Spanish);
    } else if (checkedAction == this->m_ui->actionJapanese) {
        this->setLanguage(QmsSettingsLoader::SupportedLanguage::Japanese);
    } else {
        LOG_DEBUG() << "Language action checked, but it was not a known QAction (this should never happen)";
    }
}

/* boardResizeDialogVisible() : Pass through function, delegating to
 * the shared_ptr of the board size form, returning whether or not
 * the form is visible, for use in pausing or unpausing the game */
bool MainWindow::boardResizeDialogVisible()
{
    return this->m_boardSizeDialog->isVisible();
}

/* showEvent() : Called when the main window is shown, and emits a
 * gameResumed signal if the window is actually visible (ie not hidden) */
void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if ((!this->isHidden()) && (!this->isMinimized())) {
        if (!this->m_gameController->initialClickFlag() && (!this->m_gameController->gameOver())) {
            emit (gameResumed());
        }
    }
}

/* hideEvent() : Called when the main window is hidden, and emits
 * a gamePaused signal if the window actually is hidden or minimized */
void MainWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    if ((this->isHidden()) || (this->isMinimized())) {
        emit (gamePaused());
    }
}

/* closeEvent() : Called when the main window is about to be closed,
 * so the user can be prompted if they'd like to close the game */
void MainWindow::closeEvent(QCloseEvent *event)
{
#if !defined(__ANDROID__)
    QmsApplicationSettings settings{this->collectApplicationSettings()};
    QmsSettingsLoader::saveApplicationSettings(settings);
#endif
    return QMainWindow::closeEvent(event);
    /*
    emit(gamePaused());
    QMessageBox::StandardButton userReply;
    userReply = QMessageBox::question(this, CLOSE_APPLICATION_WINDOW_TITLE, CLOSE_APPLICATION_WINDOW_PROMPT, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        this->m_boardSizeDialog->close();
        event->accept();
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
        if (!this->m_gcPtr->initialClickFlag() && !this->m_gcPtr->gameOver()) {
            emit (gameResumed());
        }
    }
    */
}


/* onCustomDialogClosed() : Called when a custom dialog (board resize, etc)
 * is closed, signaling to the main game window to resume the game, if a game is in progress */
void MainWindow::onBoardResizeDialogClosed(int columns, int rows, QDialog::DialogCode userAction)
{
    this->setEnabled(true);
    if (userAction == QDialog::DialogCode::Accepted) {
        this->invalidateSizeCaches();
        emit(boardResize(columns, rows));
    } else {
        if (!this->m_gameController->initialClickFlag() && !this->m_gameController->gameOver()) {
            emit (gameResumed());
        }
    }
}

/* onGameWon() : Called when the GameController class detects (via a mine click event,
 * flagging enough mines, or whatever win condition is defined) that the game has been won
 * Upon call, it iterates through the game board and sets the icons appropriately (green check if
 * a mine was correctly marked, red x if a flag was incorrectly marked, etc */
void MainWindow::onGameWon()
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_BIG_SMILEY);
    this->m_gameController->setGameOver(true);

    for (auto &it : this->m_gameController->mineSweeperButtons()) {
        if(it.second->hasMine()) {
            if (it.second->hasFlag()) {
                it.second->setIcon(this->m_gameIcons->STATUS_ICON_FLAG_CHECK);
                it.second->setChecked(true);
            } else {
                it.second->setIcon(this->m_gameIcons->MINE_ICON_72);
                it.second->setChecked(true);
                it.second->setStyleSheet(UNCOVERED_MINE_STYLESHEET);
            }
        } else if (it.second->hasFlag()) {
            it.second->setIcon(this->m_gameIcons->STATUS_ICON_FLAG_X);
            it.second->setChecked(true);
        }
        it.second->setIsRevealed(true);
    }
    std::unique_ptr<QMessageBox> winBox{new QMessageBox{}};
    winBox->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    winBox->setText(QString{"%1%2%3%4"}.arg(QMessageBox::tr(WIN_DIALOG_BASE),
                                           QS_NUMBER(this->m_gameController->numberOfMovesMade()),
                                           QMessageBox::tr(WIN_DIALOG_MIDDLE),
                                           QMessageBox::tr(this->statusBar()->currentMessage().toStdString().c_str())));
    winBox->setWindowIcon(this->m_gameIcons->MINE_ICON_48);
    winBox->exec();
}

/* centerAndFitWindow() : Called whenever code requests the main window to be centered
 * Typically called after resizing the UI on the main window */
void MainWindow::centerAndFitWindow()
{
    /*
#if defined(ANDROID)
    this->setGeometry(this->x(), this->y(), this->minimumWidth(), this->minimumHeight());
#else
    this->setFixedSize(this->minimumSize());
#endif
    */
    this->setFixedSize(this->minimumSize());
    this->calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

/* onGameStarted() : Called when a gameStarted() signal is emitted
 * Start the game timer, as well as starting the user idle timer, which
 * is used to change the smiley face to a sleepy face */
void MainWindow::onGameStarted()
{
    this->startGameTimer();
    this->startUserIdleTimer();
}

/* setupNewGame() : Called when a gameStarted() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and delete them,
 * so new MineSweeperButtons can be created from a known fresh state, then
 * populate the mineFrame via populateMineFrame() */
void MainWindow::setupNewGame()
{
    QLayoutItem *wItem;
    while ((wItem = this->m_ui->mineFrame->layout()->takeAt(0)) != 0) {
        delete wItem;
    }
    this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_SMILEY);
    this->populateMineField();
    this->centerAndFitWindow();
}

/* onGamePaused() : Called when a gamePaused() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and
 * disable them, so the user cannot play the game until it is resumed */
void MainWindow::onGamePaused()
{
    for (auto &it : this->m_gameController->mineSweeperButtons()) {
        it.second->setEnabled(false);
    }
}

/* onGameResumed() : Called when a gameResumed() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and enable them,
 * so the user can continue with their game */
void MainWindow::onGameResumed()
{
    for (auto &it : this->m_gameController->mineSweeperButtons()) {
        it.second->setEnabled(true);
    }
}

/* displayMine() : Called via the click handlers or other code (maybe onGameWon())
 * to provide a consistent way to display a mine on the MainWindow. First the number
 * of surrounding mines is drawn (the push button icon is set to the correct number
 * of surrounding mines), then a mineDisplayed() signal is emitted, to inform anything
 * connected that a mine is being displayed, then recursively check for other empty mines */
void MainWindow::displayMine(QmsButton* msb)
{
    drawNumberOfSurroundingMines(msb);
    msb->setFlat(true);
    msb->setChecked(true);
    emit(mineDisplayed());
    if (msb->numberOfSurroundingMines() == 0) {
        this->m_gameController->checkForOtherEmptyMines(msb);
    }
}

/* populateMineField() : The initialization for any new game, adding all QMineSweeperButtons
 * Iterate through the number of columns and rows and call GameController::addMineSweeperButton
 * to populate all of the QMineSweeperButtons. Also, save the stylesheet so it can be quickly
 * recalled on a new game. The size of the icons and the button itself is also set */
void MainWindow::populateMineField()
{
    for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gameController->numberOfColumns(); columnIndex++) {
            this->m_gameController->addMineSweeperButton(columnIndex, rowIndex);
            std::shared_ptr<QmsButton> tempPtr{this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex)};
            this->m_ui->mineFrameGridLayout->addWidget(tempPtr.get(), rowIndex, columnIndex, 1, 1);
            tempPtr->setFixedSize(getMaxMineSize());
            emit(mineSweeperButtonCreated(tempPtr));
#if defined(__ANDROID__)
            tempPtr->setIconSize(tempPtr->size());
#else
            tempPtr->setIconSize(tempPtr->size() * MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR);
#endif
            this->m_saveStyleSheet = tempPtr->styleSheet();
            tempPtr->setCheckable(true);
        }
    }
    this->m_saveStyleSheet = this->m_gameController->mineSweeperButtonAtIndex(0, 0)->styleSheet();
}

/* invalidateSizeCaches() : The maximum size of a QMineSweeperButton and the
 * icon reduction size are both cached for quicker recall. After a new game is
 * started or the board is resized, this is called to invalidate the caches */
void MainWindow::invalidateSizeCaches()
{
    this->m_maxMineSizeCacheIsValid = false;
    this->m_iconReductionSizeCacheIsValid = false;
}

/* getIconReductionSize() : The size of the icons for the QMineSweeperButtons are
 * set in relation to the full size of the button, minus a calculated reduction size
 * This function first checks if the cache for this number is still valid, and if it's
 * not, calculates the new value. It is dependant on the number of mines, the reduction
 * size scale factor (platform dependant), and the overall size of the board */
QSize MainWindow::getIconReductionSize()
{
    using namespace QmsUtilities;
    if (!this->m_iconReductionSizeCacheIsValid) {
        int reductionSize{roundIntuitively(this->m_reductionSizeScaleFactor - (this->m_gameController->totalButtonCount() + this->m_gameController->DEFAULT_NUMBER_OF_MINES()))};
        if (reductionSize <= 0) {
            reductionSize = 0;
        }
        this->m_currentIconReductionSize = QSize{reductionSize, reductionSize};
        this->m_iconReductionSizeCacheIsValid = true;
    }
    return this->m_currentIconReductionSize;
}

/* getMaximumMineSize() : The maximum size of a QMineSweeperButton is important, to
 * make sure the entire game can fit on the player's screen. To do this, the lesser
 * of maximum height or maximum width will be returned from this function, to keep
 * the QMineSweeperButton geometry as a square, as opposed to a rectangle. This function
 * first checks if the cache for this number is still valid, and if it's not, calculates the new value.
 * It is dependant on the available geometry of the screen, as well as the total number of mines */
QSize MainWindow::getMaxMineSize()
{
    using namespace QmsStrings;
    if (!this->m_maxMineSizeCacheIsValid) {
        int defaultMineSize{this->m_qDesktopWidget->availableGeometry().height()/this->s_DEFAULT_MINE_SIZE_SCALE_FACTOR};
        this->m_currentDefaultMineSize = QSize{defaultMineSize, defaultMineSize};
        int statusBarHeight{this->m_ui->statusBar->height()};
        int menuBarHeight{this->m_ui->menuBar->height()};
        int titleFrameHeight{this->m_ui->titleFrame->height()};
        int gridSpacingHeight{this->centralWidget()->layout()->margin()*this->s_NUMBER_OF_VERTIAL_MARGINS};
        int gridSpacingWidth{this->centralWidget()->layout()->margin()*this->s_NUMBER_OF_HORIZONTAL_MARGINS};
        int heightScale{this->m_qDesktopWidget->availableGeometry().height()/this->s_HEIGHT_SCALE_FACTOR};
        int widthScale{this->m_qDesktopWidget->availableGeometry().width()/this->s_WIDTH_SCALE_FACTOR};
        int extraHeight{statusBarHeight + menuBarHeight + titleFrameHeight + gridSpacingHeight};
        int x{(this->m_qDesktopWidget->availableGeometry().height()-extraHeight-s_TASKBAR_HEIGHT-heightScale)/this->m_gameController->numberOfRows()};
        int y{(this->m_qDesktopWidget->availableGeometry().width()-gridSpacingWidth-widthScale)/this->m_gameController->numberOfColumns()};
        if ((x < 5) || (y < 5)) {
            //TODO: Deal with too small to play mines
            std::unique_ptr<QMessageBox> errorBox{new QMessageBox{}};
            errorBox->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
            errorBox->setText(tr(GENERIC_ERROR_MESSAGE));
            errorBox->setWindowIcon(this->m_gameIcons->MINE_ICON_48);
            errorBox->exec();
            exit(EXIT_FAILURE);
        }
        if ((x >= m_currentDefaultMineSize.width()) && (y >= m_currentDefaultMineSize.height())) {
            this->m_currentMaxMineSize = this->m_currentDefaultMineSize;
        } else if (x < m_currentDefaultMineSize.width()) {
            if (y < x) {
                this->m_currentMaxMineSize = QSize{y, y};
            } else {
                this->m_currentMaxMineSize = QSize{x, x};
            }
        } else {
            this->m_currentMaxMineSize = QSize{y, y};
        }
        this->m_maxMineSizeCacheIsValid = true;
    }
    return this->m_currentMaxMineSize;
}

/* resizeResetIcon(): The reset button's icon is set separate from the rest of the
 * QMineSweeperButton icons, because it can be any size, independant of the rest of
 * the icons. The value is calculated based upon the size of the adjacent LCDs */
void MainWindow::resizeResetIcon()
{
#if defined(__ANDROID__)
    this->m_ui->resetButton->setFixedSize(this->m_gameController->mineSweeperButtonAtIndex(0, 0)->size());
    this->m_ui->resetButton->setIconSize(this->m_ui->resetButton->size());
    this->m_ui->numberOfMoves->setFixedSize(this->m_ui->numberOfMoves->size() * 3);
    this->m_ui->minesRemaining->setFixedSize(this->m_ui->numberOfMoves->size());
#else
    this->m_ui->resetButton->setIconSize(this->m_ui->resetButton->size() - this->getIconReductionSize());
#endif
}

/* resetResetButtonIcon() : The reset button icon is changed in response to game events
 * (mine being displayed, a multiple-empty-mine reveal, etc). This provides a single way
 * to reset the icon back to the normal smiley face icon */
void MainWindow::resetResetButtonIcon()
{
    this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_SMILEY);
}

/* setResetButtonIcon() : The reset button icon is changed in response to game events
 * (mine being displayed, a multiple-empty-mine reveal, etc). This provides a single way
 * to set the icon to whatever icon is passed into the function */
void MainWindow::setResetButtonIcon(const QIcon &icon)
{
    this->m_ui->resetButton->setIcon(icon);
}

/* displayAllMines() : Called when a game is won or lost, to reveal all of the
 * QMineSweeperButtons. This also displays any correctly or incorrectly marked
 * flags, to show the user where they made mistakes or were correct */
void MainWindow::displayAllMines()
{
    using namespace QmsStrings;
    this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_FROWNY);
    this->m_gameController->setGameOver(true);
    for (auto &it : this->m_gameController->mineSweeperButtons()) {
        if(it.second->hasMine()) {
            if (it.second->hasFlag()) {
                it.second->setIcon(this->m_gameIcons->STATUS_ICON_FLAG_CHECK);
                it.second->setChecked(true);
            } else {
                it.second->setIcon(this->m_gameIcons->MINE_ICON_72);
                it.second->setChecked(true);
                it.second->setStyleSheet(UNCOVERED_MINE_STYLESHEET);
            }
        } else if (it.second->hasFlag()) {
            it.second->setIcon(this->m_gameIcons->STATUS_ICON_FLAG_X);
            it.second->setChecked(true);
        }
        it.second->setIsRevealed(true);
    }
}

/* saveStyleSheet() : The default Qt-supplied stylesheet for the
 * QMineSweeperButtons is saved to quickly recall it, this is a
 * simple member access function for this value */
QString MainWindow::saveStyleSheet() const
{
    return this->m_saveStyleSheet;
}

/* qmsiPtr() : All of the icons used in QMineSweeper are stored in an
 * instance of QMineSweeperIcons, and all relevant classes are supplied
 * with a shared_ptr to this instance either via their constructors or
 * via a late binding dependancy-injection function. This is a simple
 * member access function for this shared_ptr, in case it needs to be accessed
 * by an object that does not obtain this shared_ptr */
std::shared_ptr<QmsIcons> MainWindow::gameIcons() const
{
    return this->m_gameIcons;
}

/* qmssePtr() : All of the sounds used in QMineSweeper are stored in an
 * instance of QMineSweeperIcons, and all relevant classes are supplied
 * with a shared_ptr to this instance either via their constructors or
 * via a late binding dependancy-injection function. This is a simple
 * member access function for this shared_ptr, in case it needs to be accessed
 * by an object that does not obtain this shared_ptr */
std::shared_ptr<QmsSoundEffects> MainWindow::gameSounds() const
{
    return this->m_gameSoundEffects;
}

/* onResetButtonClicked() : When the reset button is clicked, the user is requesting
 * to reset the current game. First, a gamePaused() signal is emitted, then
 * if there is a current game in progress, the user is asked via a QMessageBox if they
 * are sure they want to reset the current game. If so, the game is reset via
 * the doGameReset() method. If not, a gameResumed() signal is emitted */
void MainWindow::onResetButtonClicked()
{
    using namespace QmsStrings;
    emit(gamePaused());
    if (!this->m_gameController->gameOver() && !this->m_gameController->initialClickFlag()) {
        QMessageBox::StandardButton userReply;
        userReply = static_cast<QMessageBox::StandardButton>(QMessageBox::question(this, START_NEW_GAME_WINDOW_TITLE, START_NEW_GAME_PROMPT, QMessageBox::Yes|QMessageBox::No));
        if (userReply == QMessageBox::Yes) {
            this->doGameReset();
        } else {
            emit(gameResumed());
        }
    } else {
        this->doGameReset();
    }
}

/* doGameReset() : To reset the current game, all of the columns and rows
 * are iterated through and each QMineSweeperButton is set to a default state,
 * (not checked, flat, default stylesheet, etc). After all are set, a
 * resetGame() signal is emitted and the reset button is set to default */
void MainWindow::doGameReset()
{
    using namespace QmsStrings;
    this->m_boardSizeDialog->hide();
    for (auto &it : this->m_gameController->mineSweeperButtons()) {
        it.second->setChecked(false);
        it.second->setFlat(false);
        it.second->setStyleSheet(this->m_saveStyleSheet);
        it.second->setIcon(this->m_gameIcons->COUNT_MINES_0);
        it.second->setEnabled(true);
        it.second->setIsRevealed(false);
    }

    emit(resetGame());
    this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_SMILEY);
    this->displayStatusMessage(QStatusBar::tr(START_NEW_GAME_INSTRUCTION));
    this->updateNumberOfMinesLCD(this->m_gameController->numberOfMines());
    this->updateNumberOfMovesMadeLCD(0);
}

/* eventLoop() : Called whenever the game timer times out, at the specified interval.
 * Any periodic things that must be done during the game are called here, such as
 * updating the visible game timer, and any resizing of the geometry */
void MainWindow::eventLoop()
{
    updateVisibleGameTimer();
    updateUserIdleTimer();
    updateGeometry();
}

/* updateGeometry() : Convenience function to center and fit the window,
 * if it is not already set by checking the size against the calculated minimum size */
void MainWindow::updateGeometry()
{
    if (this->size() != this->minimumSize()) {
        this->centerAndFitWindow();
    }
}

/* startGameTimer() : Called when a new game is setup up, via the gameStarted() signal.
 * This timer is to show the player how long they've been playing the current game */
void MainWindow::startGameTimer()
{
    this->m_playTimer->restart();
}

/* startUserIdleTimer() : Called after each move made by a player.
 * The user idle timer keeps track of when the user is idle, and is checked
 * against a threshold value. If/when the user idle timer crosses the
 * threshold, the reset icon is changed to a sleepy face */
void MainWindow::startUserIdleTimer()
{
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        this->m_userIdleTimer->restart();
    }
}

/* updateVisibleGameTimer() : Called whenever the play timer emits it's millisecondChanged
 * (or equivalent) signal, to update the visible timer to inform the player how long they've
 * been playing the current game. Before updating, the current game state is checked from
 * the GameController::gameState() method. If the game is paused or in progress, the timer is updated.
 * If the game is stopped, the timer is not updated, and the "start new game" dialog is shown instead */
void MainWindow::updateVisibleGameTimer()
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    QString gameTime{""};
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_playTimer->isPaused()) {
            this->m_playTimer->resume();
        }
        this->m_playTimer->update();
        gameTime = toQString(this->m_playTimer->toString(static_cast<uint8_t>(GameController::MILLISECOND_DELAY_DIGITS())));
        this->displayStatusMessage(gameTime);
    } else if (!this->m_gameController->initialClickFlag()){
        this->m_playTimer->pause();
        this->m_playTimer->update();
        gameTime = toQString(this->m_playTimer->toString(static_cast<uint8_t>(GameController::MILLISECOND_DELAY_DIGITS())));
        this->displayStatusMessage(gameTime);
    } else {
        this->displayStatusMessage(QStatusBar::tr(START_NEW_GAME_INSTRUCTION));
    }
}

/* updateUserIdleTimer() : If the current game state is not paused or stopped,
 * the user idle timer is checked, the see if it has crossed the GameControllers
 * DEFAULT_SLEEPY_FACE_TIMEOUT threshold. If it has, the reset icon is changed to
 * a sleepy face. If it has not, the method returns */
void MainWindow::updateUserIdleTimer()
{
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_userIdleTimer->isPaused()) {
            this->m_userIdleTimer->resume();
        }
        this->m_playTimer->update();
    } else if (!this->m_gameController->initialClickFlag()) {
        this->m_userIdleTimer->pause();
        this->m_userIdleTimer->update();
    }
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_userIdleTimer->totalTime() >= this->m_gameController->DEFAULT_SLEEPY_FACE_TIMEOUT()) {
            this->m_ui->resetButton->setIcon(this->m_gameIcons->FACE_ICON_SLEEPY);
        }
    }
}

/* getLCDPadding() : Both LCDs in the MainWindow must always display 3 digits.
 * This is a common function to call to get a string object with how many padded
 * zeroes are requested (ie getLCDPadding(2) returns "00") */
std::string MainWindow::getLCDPadding(uint8_t howMuch)
{
    return QmsUtilities::getPadding(howMuch, '0');
}

/* updateNumberOfMinesLCD() : The right LCD on the MainWindow shows how
 * many mines may be remaining in the game, in accordance with how many mines have been flagged
 * by the user, indiciating that they thing there is a mine there. Whenever this value is updated
 * via the GameController, GameController emits a numberOfMinesRemainingChanged() signal, which is
 * hooked by this function. The LCD should always display 3 characters, so the parameter to the
 * function is checked against powers of 10, and the correct number of 0's are
 * added before the number is displayed */
void MainWindow::updateNumberOfMinesLCD(int numberOfMines)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    if (numberOfMines > 999) {
        this->m_ui->minesRemaining->display(LCD_OVERFLOW_STRING);
    } else if (numberOfMines <= 0) {
        this->m_ui->minesRemaining->display(toQString(this->getLCDPadding(3)));
    } else if (numberOfMines < 10) {
        this->m_ui->minesRemaining->display(QStringFormat("%s%i", this->getLCDPadding(2).c_str(), numberOfMines));
    } else if (numberOfMines < 100) {
        this->m_ui->minesRemaining->display(QStringFormat("%s%i", this->getLCDPadding(1).c_str(), numberOfMines));
    } else {
        this->m_ui->minesRemaining->display(numberOfMines);
    }
}

/* updateNumberOfMovesMadeLCD() : The left LCD on the MainWindow shows how
 * many moves have been made by the user. Whenever this is updated via the GameController,
 * GameController emits a numberOfMovesMadeChanged() signal, which is hooked by this function.
 * The LCD should always display 3 characters, so the parameter to the function is checked
 * against powers of 10, and the correct number of 0's are added before the number is displayed */
void MainWindow::updateNumberOfMovesMadeLCD(int numberOfMovesMade)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    if (numberOfMovesMade > 999) {
        this->m_ui->numberOfMoves->display(LCD_OVERFLOW_STRING);
    } else if (numberOfMovesMade <= 0) {
        this->m_ui->numberOfMoves->display(toQString(this->getLCDPadding(3)));
    } else if (numberOfMovesMade < 10) {
        this->m_ui->numberOfMoves->display(QStringFormat("%s%i", this->getLCDPadding(2).c_str(), numberOfMovesMade));
    } else if (numberOfMovesMade < 100) {
        this->m_ui->numberOfMoves->display(QStringFormat("%s%i", this->getLCDPadding(2).c_str(), numberOfMovesMade));
    } else {
        this->m_ui->numberOfMoves->display(numberOfMovesMade);
    }

}

/* onMineExplosionEventTriggered() : When a player reveals a mine, a mineExplosionEvent() signal is
 * emitted by the GameController, and this method is called. All mines are displayed, and the mineExplosionEvent
 * is re-emitted by MainWindow, indicating that all UI elements of a game over are taken care of */
void MainWindow::onMineExplosionEventTriggered()
{
    using namespace QmsUtilities;
    displayAllMines();
    this->m_gameSoundEffects->explosionEffect().play();
    emit(mineExplosionEvent());
}

/* onMuteSoundClicked() : Called when the "Mute Sound" menu option is
 * clicked. If the option is checked, all sounds from QMineSweeper
 * are disabled. If not, they are enabled */
void MainWindow::onActionMuteSoundChecked(bool checked)
{
    this->m_gameSoundEffects->setAudioMuted(checked);
}

/* onChangeBoardSizeActionTriggered() : Called when the "change board size" menu option
 * is triggered. The current game is first paused, then the user is shown the BoardSize form.
 * The MainWindow flag m_boardSizeGeometrySet is checked, and if it is false, the geometry for the
 * BoardSize form is set. If it is true, the geometry is not set. */
void MainWindow::onChangeBoardSizeActionTriggered()
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    this->setEnabled(false);
    this->m_boardSizeDialog->show(this->m_gameController->numberOfColumns(), this->m_gameController->numberOfRows());
    this->m_boardSizeDialog->centerAndFitWindow(this->m_qDesktopWidget.get());
    emit(gamePaused());
}

/* onAboutQmsWindowClosed() : Called when the AboutQmsWindow is closed. If a game is currently
 * in progress, the game is resumed by emitting a gameResumed() signal, and all widgets on the
 * MainWindow are re-enabled, as well as hiding the AboutQmsWindow */
void MainWindow::onAboutQmsWindowClosed()
{
    this->setEnabled(true);
    this->show();
    this->m_aboutQmsDialog->hide();
    if (!this->m_gameController->gameOver()) {
        emit(gameResumed());
    }
}

/* onAboutQMineSweeperActionTriggered() : Called when the About->About QMineSweeper menu
 * option is clicked, causing the About QMineSweeper window (defined in forms/aboutqmsdialog.ui)
 * to show. This method also pauses the game while the window is active */
void MainWindow::onAboutQtActionTriggered()
{

}

/* onAboutQmActionTriggered() : Called when the About->About Qt menu
 * option is clicked, causing the About Qt window (supplied by the Qt toolchain)
 * to show. This separate event is also hooked, allowing the game to be paused if necessary */
void MainWindow::onAboutQMineSweeperActionTriggered()
{
    using namespace QmsGlobalSettings;
    this->setEnabled(false);
    emit(gamePaused());
    this->m_aboutQmsDialog->show();
    this->m_aboutQmsDialog->centerAndFitWindow(this->m_qDesktopWidget.get());
}


/* onApplicationExit() : Called when the QApplication is about to close,
 * via hooking the QApplication::exit() event in main.cpp, empty by default */
void MainWindow::onApplicationExit()
{

}

/* bindGameController() : Convenience function for late binding of a shared_ptr
 * to the GameController instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindGameController(std::shared_ptr<GameController> gameController)
{
    this->m_gameController.reset();
    this->m_gameController = gameController;
}

/* bindQDesktopWidget() : Convenience function for late binding of a shared_ptr
 * to the QDesktopWidget instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

/* bindQMineSweeperIcons() : Convenience function for late binding of a shared_ptr
 * to the QMineSweeperIcons instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQMineSweeperIcons(std::shared_ptr<QmsIcons> gameIcons)
{
    this->m_gameIcons.reset();
    this->m_gameIcons = gameIcons;
}

/* bindGameController() : Convenience function for late binding of a shared_ptr
 * to the GameController instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQMineSweeperSoundEffects(std::shared_ptr<QmsSoundEffects> gameSoundEffects)
{
    this->m_gameSoundEffects.reset();
    this->m_gameSoundEffects = gameSoundEffects;
}

/* bindQMineSweeperSettingsLoader() : Convenience function for late binding of a shared_ptr
 * to the QMineSweeperSettingsLoader instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQMineSweeperSettingsLoader(std::shared_ptr<QmsSettingsLoader> qmsslPtr)
{
    this->m_settingsLoader.reset();
    this->m_settingsLoader = qmsslPtr;
}


/* drawNumberOfSurroundingMines() : Called when a mine is about to be displayed
 * Queries the shared_ptr passed in for it's number of surrounding mines, then
 * uses the shared_ptr to the QMineSweeperIcons instance to set the graphic on the
 * QMineSweeperButton to the correct number via chained else-ifs */
void MainWindow::drawNumberOfSurroundingMines(QmsButton *msb)
{
    if (msb->numberOfSurroundingMines() == 0) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_0);
    } else if (msb->numberOfSurroundingMines() == 1) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_1);
    } else if (msb->numberOfSurroundingMines() == 2) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_2);
    } else if (msb->numberOfSurroundingMines() == 3) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_3);
    } else if (msb->numberOfSurroundingMines() == 4) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_4);
    } else if (msb->numberOfSurroundingMines() == 5) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_5);
    } else if (msb->numberOfSurroundingMines() == 6) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_6);
    } else if (msb->numberOfSurroundingMines() == 7) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_7);
    } else if (msb->numberOfSurroundingMines() == 8) {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_8);
    } else {
        msb->setIcon(this->m_gameIcons->COUNT_MINES_0);
    }
}

/* xPlacement() : Member access function for MainWindow::m_xPlacement */
int MainWindow::xPlacement() const
{
    return this->m_xPlacement;
}

/* yPlacement() : Member access function for MainWindow::m_yPlacement */
int MainWindow::yPlacement() const
{
    return this->m_yPlacement;
}

/* calculateXYPlacement() : Checks the currently available screen geometry, and calulates
 * where the MainWindow must be moved to appear on at the center of the users screen */
void MainWindow::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{new QRect{this->m_qDesktopWidget->availableGeometry()}};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
#if defined(__ANDROID__)
    this->m_yPlacement = avail->height() - this->height();
#else
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - this->s_TASKBAR_HEIGHT;
#endif
}


/* ~MainWindow() : Destructor, empty by default, as all ownership is taken care
 * of by c++11's smart pointers (unique_ptr and shared_ptr) */
MainWindow::~MainWindow()
{

}
