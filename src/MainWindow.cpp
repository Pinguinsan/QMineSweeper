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
#include <QDesktopWidget>
#include <QWindow>
#include <QString>
#include <QRect>
#include <QTimer>
#include <QFileDialog>
#include <QDialog>
#include <QActionGroup>
#include <QTranslator>
#include <QSettings>
#include <QDateTime>

#include <cctype>

#include "QmsButton.hpp"
#include "QmsIcons.hpp"
#include "GameController.hpp"
#include "BoardResizeWidget.hpp"
#include "QmsSoundEffects.hpp"
#include "QmsUtilities.hpp"
#include "QmsStrings.hpp"
#include "QmsSettingsLoader.hpp"
#include "GlobalDefinitions.hpp"
#include "QmsApplicationSettings.hpp"
#include "AboutApplicationWidget.hpp"

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "AutoUpdateLCD.hpp"
#include <QMessageBox>


#if defined(_WIN32)
    const int MainWindow::s_TASKBAR_HEIGHT{25};
#else
    const int MainWindow::s_TASKBAR_HEIGHT{15};
#endif //defined(_WIN32)
const int MainWindow::s_HEIGHT_SCALE_FACTOR{18};
const int MainWindow::s_WIDTH_SCALE_FACTOR{28};
const int MainWindow::s_NUMBER_OF_HORIZONTAL_MARGINS{2};
const int MainWindow::s_NUMBER_OF_VERTIAL_MARGINS{4};
const int MainWindow::s_DEFAULT_MINE_SIZE_SCALE_FACTOR{19};
const int MainWindow::s_STATUS_BAR_FONT_POINT_SIZE{12};
const double MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR{0.75};

/* MainWindow() : Constructor. All UI stuff if initialized and
 * relevant events are hooked (QObject::connect()) to set up the game to play */
MainWindow::MainWindow(QmsSettingsLoader::SupportedLanguage initialDisplayLanguage,
                       QWidget *parent) :
    MouseMoveableQMainWindow{parent},
    m_eventTimer{new QTimer{}},
    m_userIdleTimer{new SteadyEventTimer{}},
    m_aboutQmsDialog{new AboutApplicationWidget{}},
    m_boardResizeDialog{new BoardResizeWidget{}},
    m_languageActionGroup{new QActionGroup{nullptr}},
    m_translator{new QTranslator{}},
    m_statusBarLabel{new QLabel{}},
    m_language{initialDisplayLanguage},
    m_reductionSizeScaleFactor{0},
    m_currentDefaultMineSize{QSize{0,0}},
    m_currentMaxMineSize{QSize{0,0}},
    m_currentIconReductionSize{QSize{0, 0}},
    m_maxMineSizeCacheIsValid{false},
    m_iconReductionSizeCacheIsValid{false},
    m_boardSizeGeometrySet{false},
    m_saveFilePath{""},
    m_ui{new Ui::MainWindow{}}
{

    using namespace QmsStrings;
    this->m_ui->setupUi(this);
    this->m_ui->centralwidget->setMouseTracking(true);
    this->setStyleSheet("");


    this->m_ui->numberOfMoves->setDataSource(gameController->numbersOfMovesMadeDataSource());
    this->m_ui->minesRemaining->setDataSource(gameController->userDisplayNumbersOfMinesDataSource());

    QFont tempFont{this->m_statusBarLabel->font()};
    tempFont.setPointSize(MainWindow::s_STATUS_BAR_FONT_POINT_SIZE);
    this->m_statusBarLabel->setFont(tempFont);
    this->m_ui->statusBar->addWidget(this->m_statusBarLabel.get());
    if (applicationSoundEffects->isMuted()) {
        this->m_ui->actionMuteSound->setChecked(true);
    }

    this->setLanguage(this->m_language);
    this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_SMILEY);
    this->m_eventTimer->setInterval(gameController->GAME_TIMER_INTERVAL());

    this->connect(this->m_ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveActionTriggered);
    this->connect(this->m_ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsActionTriggered);
    this->connect(this->m_ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionTriggered);

    this->m_ui->actionSave->setEnabled(false);
    this->m_ui->actionSaveAs->setEnabled(false);

    this->connect(this->m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    this->connect(this->m_ui->actionAboutQt, &QAction::triggered, this, &MainWindow::onAboutQtActionTriggered);
    this->connect(this->m_ui->actionBoardSize, &QAction::triggered, this, &MainWindow::onChangeBoardSizeActionTriggered);
    this->connect(this->m_ui->actionMuteSound, &QAction::triggered, this, &MainWindow::onActionMuteSoundChecked);

    this->connect(this->m_ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
    this->connect(gameController, &GameController::winEvent, this, &MainWindow::onGameWon);
    this->connect(gameController, &GameController::readyToBeginNewGame, this, &MainWindow::setupNewGame);
    this->connect(gameController, &GameController::userIsNoLongerIdle, this, &MainWindow::startUserIdleTimer);
    this->connect(gameController, &GameController::gameStarted, this, &MainWindow::onGameStarted);
    this->connect(gameController, &GameController::customMineRatioSet, this, &MainWindow::onCustomMineRatioSet);
    this->connect(gameController, &GameController::loadGameCompleted, this, &MainWindow::onLoadGameCompleted);

    this->connect(this->m_eventTimer.get(), &QTimer::timeout, this, &MainWindow::eventLoop);

    this->connect(gameController, &GameController::gameResumed, this, &MainWindow::onGameResumed);
    this->connect(gameController, &GameController::mineExplosionEvent, this, &MainWindow::onMineExplosionEventTriggered);

    this->connect(this->m_ui->menuFile, &QMenu::aboutToShow, gameController, &GameController::onContextMenuActive);
    this->connect(this->m_ui->menuPreferences, &QMenu::aboutToShow, gameController, &GameController::onContextMenuActive);
    this->connect(this->m_ui->menuHelp, &QMenu::aboutToShow, gameController, &GameController::onContextMenuActive);

    this->connect(this->m_ui->menuFile, &QMenu::aboutToHide, gameController, &GameController::onContextMenuInactive);
    this->connect(this->m_ui->menuPreferences, &QMenu::aboutToHide, gameController, &GameController::onContextMenuInactive);
    this->connect(this->m_ui->menuHelp, &QMenu::aboutToHide, gameController, &GameController::onContextMenuInactive);

    this->connect(this, &MainWindow::boardResize, gameController, &GameController::onBoardResizeTriggered);
    this->connect(this, &MainWindow::mineDisplayed, gameController, &GameController::onMineDisplayed);
    this->connect(this, &MainWindow::winEvent, gameController, &GameController::onGameWon);
    this->connect(this, &MainWindow::gamePaused, gameController, &GameController::onGamePaused);
    this->connect(this, &MainWindow::mineSweeperButtonCreated, gameController, &GameController::onMineSweeperButtonCreated);
    this->connect(this, &MainWindow::resetGame, gameController, &GameController::onGameReset);
    this->connect(this, &MainWindow::gameResumed, gameController, &GameController::onGameResumed);
    this->connect(this, &MainWindow::mineExplosionEvent, gameController, &GameController::onMineExplosionEventTriggered);

    this->m_languageActionGroup->addAction(this->m_ui->actionEnglish);
    this->m_languageActionGroup->addAction(this->m_ui->actionSpanish);
    this->m_languageActionGroup->addAction(this->m_ui->actionFrench);
    this->m_languageActionGroup->addAction(this->m_ui->actionJapanese);
    this->m_languageActionGroup->setExclusive(true);

    this->connect(this->m_ui->actionEnglish, &QAction::triggered, this, &MainWindow::onLanguageSelected);
    this->connect(this->m_ui->actionSpanish, &QAction::triggered, this, &MainWindow::onLanguageSelected);
    this->connect(this->m_ui->actionFrench, &QAction::triggered, this, &MainWindow::onLanguageSelected);
    this->connect(this->m_ui->actionJapanese, &QAction::triggered, this, &MainWindow::onLanguageSelected);
	
	this->m_aboutQmsDialog->addLicenseTab(QmsGlobalSettings::PROGRAM_NAME, QmsStrings::QMINESWEEPER_LICENSE_PATH);

    std::unique_ptr<QRect> avail{new QRect{QDesktopWidget{}.availableGeometry()}};

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
    this->m_boardResizeDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->m_boardResizeDialog->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    this->m_boardResizeDialog->setWindowIcon(applicationIcons->MINE_ICON_72);
    this->connect(this->m_boardResizeDialog.get(), &BoardResizeWidget::aboutToClose, this, &MainWindow::onBoardResizeDialogClosed);

    this->connect(this->m_ui->actionAboutQMineSweeper, &QAction::triggered, this, &MainWindow::onAboutQMineSweeperActionTriggered);
    this->connect(this->m_aboutQmsDialog.get(), &AboutApplicationWidget::aboutToClose, this, &MainWindow::onAboutQmsWindowClosed);

    this->m_eventTimer->start();
}

void MainWindow::onCustomMineRatioSet(float mineRatio)
{
    LOG_DEBUG() << QString{R"(Custom mine ratio %1 has been set)"}.arg(QS_NUMBER(mineRatio));
}

void MainWindow::onSaveActionTriggered()
{
    if (this->m_saveFilePath == "") {
        this->onSaveAsActionTriggered();
    } else {
        emit(gamePaused());
        this->doSaveGame(this->m_saveFilePath);
        emit(gameResumed());
    }
}

void MainWindow::onSaveAsActionTriggered()
{
    emit(gamePaused());
    QFileDialog fileDialog;
    fileDialog.setDirectory(QmsUtilities::getProgramSettingsDirectory());
    fileDialog.setDefaultSuffix(QmsStrings::SAVED_GAME_FILE_EXTENSION);
    fileDialog.setConfirmOverwrite(true);
    fileDialog.setFileMode(QFileDialog::FileMode::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
	fileDialog.setNameFilter(QString{ MainWindow::tr("QMineSweeper files : (*.%1)")}.arg(QmsStrings::SAVED_GAME_FILE_EXTENSION));
    this->m_saveFilePath = fileDialog.getSaveFileName(this, QFileDialog::tr(QmsStrings::SAVE_FILE_CAPTION));

    if (this->m_saveFilePath == "") {
        emit(gameResumed());
        return;
    } else {
        if (!this->m_saveFilePath.endsWith(QmsStrings::SAVED_GAME_FILE_EXTENSION)) {
            this->m_saveFilePath = this->m_saveFilePath + QmsStrings::SAVED_GAME_FILE_EXTENSION;
        }
    }
    this->doSaveGame(this->m_saveFilePath);
    emit(gameResumed());
}

void MainWindow::doSaveGame(const QString &filePath)
{
    QString errorString{""};
    auto saveGameResult = gameController->saveGame(filePath);
    if (saveGameResult == SaveGameStateResult::Success) {
        LOG_INFO() << QString{"Successfully saved game to %1"}.arg(filePath);
        return;
    } else if (saveGameResult == SaveGameStateResult::UnableToDeleteExistingFile) {
        errorString = "Unable to remove existing file (check permissions on target file)";
    } else if (saveGameResult == SaveGameStateResult::UnableToOpenFile) {
        errorString = "Unable to open file (check permissions on target file)";
    } else if (saveGameResult == SaveGameStateResult::UnableToDeleteExistingHashFile) {
        errorString = "Unable to remove existing hash file (check permissions on hash file)";
    } else if (saveGameResult == SaveGameStateResult::UnableToOpenFileToWriteHash) {
        errorString = "Unable to open hash file (check permissions on hash file)";
    }
    std::unique_ptr<QMessageBox> errorBox{new QMessageBox{}};
    errorBox->setWindowTitle(MainWindow::tr(QmsStrings::ERROR_SAVING_FILE_TITLE));
    QString errorText{QString{QmsStrings::ERROR_SAVING_FILE_MESSAGE}.arg(filePath, errorString)};
    LOG_WARNING() << errorText;
    errorBox->setText(errorText);

    errorBox->setWindowIcon(applicationIcons->MINE_ICON_48);
    errorBox->exec();
}

void MainWindow::onOpenActionTriggered()
{
    emit(gamePaused());
    QFileDialog fileDialog;
    fileDialog.setDirectory(QmsUtilities::getProgramSettingsDirectory());
    fileDialog.setDefaultSuffix(QmsStrings::SAVED_GAME_FILE_EXTENSION);
    fileDialog.setConfirmOverwrite(false);
    fileDialog.setFileMode(QFileDialog::FileMode::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fileDialog.setNameFilter(QString{ MainWindow::tr("QMineSweeper files : (*.%1)") }.arg(QmsStrings::SAVED_GAME_FILE_EXTENSION));
    QString maybeNewGamePath{fileDialog.getOpenFileName(this, MainWindow::tr(QmsStrings::OPEN_FILE_CAPTION))};

    if (maybeNewGamePath == "") {
        return;
    }
    if (!maybeNewGamePath.endsWith(QmsStrings::SAVED_GAME_FILE_EXTENSION)) {
        maybeNewGamePath = maybeNewGamePath + QmsStrings::SAVED_GAME_FILE_EXTENSION;
    }
    gameController->loadGame(maybeNewGamePath);
}

void MainWindow::onLoadGameCompleted(LoadGameStateResult loadResult, const QmsGameState &gameState)
{
    QString errorString{""};
    if (loadResult == LoadGameStateResult::Success) {
        this->resetGame();
        this->boardResize(gameController->numberOfColumns(), gameController->numberOfRows());
        this->setupNewGame();
        gameController->applyGameState(gameState);
        for (auto &it : gameController->mineSweeperButtons()) {
            (void)it;
            this->m_ui->mineFrameGridLayout->addWidget(it.second.get(), it.first.Y(), it.first.X(), 1, 1);
            it.second->setFixedSize(getMaxMineSize());
            emit(mineSweeperButtonCreated(it.second));
            it.second->setIconSize(it.second->size() * MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR);
            this->m_saveStyleSheet = it.second->styleSheet();
            if (it.second->isRevealed()) {
                it.second->reveal();
            }
        }
        //emit(gameResumed());
        return;
    } else if (loadResult == LoadGameStateResult::FileDoesNotExist) {
        errorString = "File does not exist";
    } else if (loadResult == LoadGameStateResult::HashFileDoesNotExist) {
        errorString = "Associated hash file does not exist (was it deleted?)";
    } else if (loadResult == LoadGameStateResult::HashVerificationFailed) {
        errorString = "Hash verification failed (was the save file altered?)";
    } else if (loadResult == LoadGameStateResult::UnableToOpenFile) {
        errorString = "Could not open file";
    }

    std::unique_ptr<QMessageBox> errorBox{new QMessageBox{}};
    errorBox->setWindowTitle(MainWindow::tr(QmsStrings::ERROR_LOADING_FILE_TITLE));
    QString errorText{QString{QmsStrings::ERROR_LOADING_FILE_MESSAGE}.arg(gameState.filePath(), errorString)};
    LOG_WARNING() << errorText;
    errorBox->setText(errorText);

    errorBox->setWindowIcon(applicationIcons->MINE_ICON_48);
    errorBox->exec();
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
    QmsApplicationSettings returnSettings{};
    returnSettings.setNumberOfColumns(gameController->numberOfColumns());
    returnSettings.setNumberOfRows(gameController->numberOfRows());
    returnSettings.setAudioVolume(applicationSoundEffects->audioVolume());
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
        LOG_DEBUG() << QString{"Attempted to set language to %1, but that language is already loaded"}.arg(QmsSettingsLoader::languageToString(language));
    } else {
        const char *targetTranslationFile{};
        if (language == QmsSettingsLoader::SupportedLanguage::English) {
            targetTranslationFile = QmsStrings::ENGLISH_TRANSLATION_PATH;
        } else if (language == QmsSettingsLoader::SupportedLanguage::Japanese) {
            targetTranslationFile = QmsStrings::JAPANESE_TRANSLATION_PATH;
        } else {
            LOG_DEBUG() << "Language action checked, but it was not a known QAction (this should never happen)";
            return;
        }
        LOG_DEBUG() << QString{"Changing program language to %1"}.arg(targetTranslationFile);
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
    checkedAction->dumpObjectInfo();
    if (checkedAction == this->m_ui->actionEnglish) {
        this->setLanguage(QmsSettingsLoader::SupportedLanguage::English);
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
    return this->m_boardResizeDialog->isVisible();
}

/* showEvent() : Called when the main window is shown, and emits a
 * gameResumed signal if the window is actually visible (ie not hidden) */
void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    //if ((!this->isHidden()) && (!this->isMinimized())) {
        if (!gameController->initialClickFlag() && (!gameController->gameOver())) {
            emit (gameResumed());
        }
    //}
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
    QmsApplicationSettings settings{this->collectApplicationSettings()};
    QmsSettingsLoader::saveApplicationSettings(settings);
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
void MainWindow::onBoardResizeDialogClosed(BoardResizeWidget::ResizeWidgetResult result)
{
    int columns{result.boardSize.columns};
    int rows{result.boardSize.rows};
    BoardResizeWidget::ResizeWidgetExitCode userAction{result.userAction};
    this->setEnabled(true);
    if (userAction == BoardResizeWidget::ResizeWidgetExitCode::Accepted) {
        this->invalidateSizeCaches();
        emit(boardResize(columns, rows));
    } else {
        if (!gameController->initialClickFlag() && !gameController->gameOver()) {
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
    this->m_ui->actionSave->setEnabled(false);
    this->m_ui->actionSaveAs->setEnabled(false);
    this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_BIG_SMILEY);
    gameController->setGameOver(true);

    for (auto &it : gameController->mineSweeperButtons()) {
        if(it.second->hasMine()) {
            if (it.second->hasFlag()) {
                it.second->setIcon(applicationIcons->STATUS_ICON_FLAG_CHECK);
                it.second->setChecked(true);
            } else {
                it.second->setIcon(applicationIcons->MINE_ICON_72);
                it.second->setChecked(true);
                it.second->setStyleSheet(UNCOVERED_MINE_STYLESHEET);
            }
        } else if (it.second->hasFlag()) {
            it.second->setIcon(applicationIcons->STATUS_ICON_FLAG_X);
            it.second->setChecked(true);
        }
        it.second->setIsRevealed(true);
    }
    std::unique_ptr<QMessageBox> winBox{new QMessageBox{}};
    winBox->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    QString winText{QString{QmsStrings::WIN_DIALOG}.arg(QS_NUMBER(gameController->numberOfMovesMade()),
                                            gameController->playTimer().toString(static_cast<uint8_t>(GameController::MILLISECOND_DELAY_DIGITS())).c_str())};
    LOG_INFO() << winText;
    winBox->setText(winText);
    winBox->setWindowIcon(applicationIcons->MINE_ICON_48);
    winBox->exec();
}


/* onGameStarted() : Called when a gameStarted() signal is emitted
 * Start the game timer, as well as starting the user idle timer, which
 * is used to change the smiley face to a sleepy face */
void MainWindow::onGameStarted()
{
    LOG_DEBUG() << QString{"Beginning game with dimensions (%1x%2)"}.arg(QS_NUMBER(gameController->numberOfColumns()), QS_NUMBER(gameController->numberOfRows()));
    this->startGameTimer();
    this->startUserIdleTimer();
    this->m_ui->actionSave->setEnabled(true);
    this->m_ui->actionSaveAs->setEnabled(true);
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
    this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_SMILEY);
    this->populateMineField();
    this->centerAndFitWindow(true, true);
}

/* onGamePaused() : Called when a gamePaused() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and
 * disable them, so the user cannot play the game until it is resumed */
void MainWindow::onGamePaused()
{
    if (gameController->gameState() == GameState::GameActive) {
        for (auto &it : gameController->mineSweeperButtons()) {
            it.second->setEnabled(false);
        }
    }
}

/* onGameResumed() : Called when a gameResumed() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and enable them,
 * so the user can continue with their game */
void MainWindow::onGameResumed()
{
    if (gameController->gameState() == GameState::GamePaused) {
        for (auto &it : gameController->mineSweeperButtons()) {
            it.second->setEnabled(true);
        }
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
        gameController->checkForOtherEmptyMines(msb);
    }
}

/* populateMineField() : The initialization for any new game, adding all QMineSweeperButtons
 * Iterate through the number of columns and rows and call GameController::addMineSweeperButton
 * to populate all of the QMineSweeperButtons. Also, save the stylesheet so it can be quickly
 * recalled on a new game. The size of the icons and the button itself is also set */
void MainWindow::populateMineField()
{
    for (int rowIndex = 0; rowIndex < gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <gameController->numberOfColumns(); columnIndex++) {
            gameController->addMineSweeperButton(columnIndex, rowIndex);
            std::shared_ptr<QmsButton> tempPtr{gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex)};
            this->m_ui->mineFrameGridLayout->addWidget(tempPtr.get(), rowIndex, columnIndex, 1, 1);
            tempPtr->setFixedSize(getMaxMineSize());
            emit(mineSweeperButtonCreated(tempPtr));
            tempPtr->setIconSize(tempPtr->size() * MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR);
            this->m_saveStyleSheet = tempPtr->styleSheet();
            tempPtr->setCheckable(true);
        }
    }
    this->m_saveStyleSheet = gameController->mineSweeperButtonAtIndex(0, 0)->styleSheet();
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
        int reductionSize{roundIntuitively(this->m_reductionSizeScaleFactor - (gameController->totalButtonCount() + gameController->DEFAULT_NUMBER_OF_MINES()))};
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
    QDesktopWidget qDesktopWidget{};
    if (!this->m_maxMineSizeCacheIsValid) {
        int defaultMineSize{qDesktopWidget.availableGeometry().height()/this->s_DEFAULT_MINE_SIZE_SCALE_FACTOR};
        this->m_currentDefaultMineSize = QSize{defaultMineSize, defaultMineSize};
        int statusBarHeight{this->m_ui->statusBar->height()};
        int menuBarHeight{this->m_ui->menuBar->height()};
        int titleFrameHeight{this->m_ui->titleFrame->height()};
        int gridSpacingHeight{this->centralWidget()->layout()->margin()*this->s_NUMBER_OF_VERTIAL_MARGINS};
        int gridSpacingWidth{this->centralWidget()->layout()->margin()*this->s_NUMBER_OF_HORIZONTAL_MARGINS};
        int heightScale{qDesktopWidget.availableGeometry().height()/this->s_HEIGHT_SCALE_FACTOR};
        int widthScale{qDesktopWidget.availableGeometry().width()/this->s_WIDTH_SCALE_FACTOR};
        int extraHeight{statusBarHeight + menuBarHeight + titleFrameHeight + gridSpacingHeight};
        int x{(qDesktopWidget.availableGeometry().height()-extraHeight-s_TASKBAR_HEIGHT-heightScale)/gameController->numberOfRows()};
        int y{(qDesktopWidget.availableGeometry().width()-gridSpacingWidth-widthScale)/gameController->numberOfColumns()};
        if ((x < 5) || (y < 5)) {
            //TODO: Deal with too small to play mines
            std::unique_ptr<QMessageBox> errorBox{new QMessageBox{}};
            errorBox->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
            errorBox->setText(tr(GENERIC_ERROR_MESSAGE));
            errorBox->setWindowIcon(applicationIcons->MINE_ICON_48);
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
    this->m_ui->resetButton->setIconSize(this->m_ui->resetButton->size() - this->getIconReductionSize());
}

/* resetResetButtonIcon() : The reset button icon is changed in response to game events
 * (mine being displayed, a multiple-empty-mine reveal, etc). This provides a single way
 * to reset the icon back to the normal smiley face icon */
void MainWindow::resetResetButtonIcon()
{
    this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_SMILEY);
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
    this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_FROWNY);
    gameController->setGameOver(true);
    for (auto &it : gameController->mineSweeperButtons()) {
        if(it.second->hasMine()) {
            if (it.second->hasFlag()) {
                it.second->setIcon(applicationIcons->STATUS_ICON_FLAG_CHECK);
                it.second->setChecked(true);
            } else {
                it.second->setIcon(applicationIcons->MINE_ICON_72);
                it.second->setChecked(true);
                it.second->setStyleSheet(UNCOVERED_MINE_STYLESHEET);
            }
        } else if (it.second->hasFlag()) {
            it.second->setIcon(applicationIcons->STATUS_ICON_FLAG_X);
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

/* onResetButtonClicked() : When the reset button is clicked, the user is requesting
 * to reset the current game. First, a gamePaused() signal is emitted, then
 * if there is a current game in progress, the user is asked via a QMessageBox if they
 * are sure they want to reset the current game. If so, the game is reset via
 * the doGameReset() method. If not, a gameResumed() signal is emitted */
void MainWindow::onResetButtonClicked()
{
    using namespace QmsStrings;
    emit(gamePaused());
    if (!gameController->gameOver() && !gameController->initialClickFlag()) {
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
    this->m_boardResizeDialog->hide();
    for (auto &it : gameController->mineSweeperButtons()) {
        it.second->setChecked(false);
        it.second->setFlat(false);
        it.second->setStyleSheet(this->m_saveStyleSheet);
        it.second->setIcon(applicationIcons->COUNT_MINES_0);
        it.second->setEnabled(true);
        it.second->setIsRevealed(false);
    }

    this->m_saveFilePath = "";
    emit(resetGame());
    this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_SMILEY);
    this->displayStatusMessage(QStatusBar::tr(START_NEW_GAME_INSTRUCTION));
}

/* eventLoop() : Called whenever the game timer times out, at the specified interval.
 * Any periodic things that must be done during the game are called here, such as
 * updating the visible game timer, and any resizing of the geometry */
void MainWindow::eventLoop()
{
    this->updateVisibleGameTimer();
    this->updateUserIdleTimer();
    this->updateMyGeometry();
}

/* updateMyGeometry() : Convenience function to center and fit the window,
 * if it is not already set by checking the size against the calculated minimum size */
void MainWindow::updateMyGeometry()
{
    if (this->size() != this->minimumSize()) {
        this->centerAndFitWindow(true, true);
    }
}

/* startGameTimer() : Called when a new game is setup up, via the gameStarted() signal.
 * This timer is to show the player how long they've been playing the current game */
void MainWindow::startGameTimer()
{
    gameController->startPlayTimer();
}

/* startUserIdleTimer() : Called after each move made by a player.
 * The user idle timer keeps track of when the user is idle, and is checked
 * against a threshold value. If/when the user idle timer crosses the
 * threshold, the reset icon is changed to a sleepy face */
void MainWindow::startUserIdleTimer()
{
    if (gameController->gameState() == GameState::GameActive) {
        this->m_userIdleTimer->restart();
    }
}

/* updateVisibleGameTimer() : Called in the main eventLoop() to update the visible timer to inform the player 
 * how long they've been playing the current game. Before updating, the current game state is checked from
 * the GameController::gameState() method. If the game is paused or in progress, the timer is updated.
 * If the game is stopped, the timer is not updated, and the "start new game" dialog is shown instead */
void MainWindow::updateVisibleGameTimer()
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    QString gameTime{""};
    if (gameController->gameState() == GameState::GameActive) {
        if (gameController->playTimer().isPaused()) {
            gameController->resumePlayTimer();
        }
        gameTime = toQString(gameController->playTimer().toString(static_cast<uint8_t>(GameController::MILLISECOND_DELAY_DIGITS())));
        this->displayStatusMessage(gameTime);
    } else if (!gameController->initialClickFlag()){
        gameController->pausePlayTimer();
        gameTime = toQString(gameController->playTimer().toString(static_cast<uint8_t>(GameController::MILLISECOND_DELAY_DIGITS())));
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
    if (gameController->gameState() == GameState::GameActive) {
        if (this->m_userIdleTimer->isPaused()) {
            this->m_userIdleTimer->resume();
        }
    } else if (!gameController->initialClickFlag()) {
        this->m_userIdleTimer->pause();
        this->m_userIdleTimer->update();
    }
    if (gameController->gameState() == GameState::GameActive) {
        if (this->m_userIdleTimer->totalTime() >= gameController->DEFAULT_SLEEPY_FACE_TIMEOUT()) {
            this->m_ui->resetButton->setIcon(applicationIcons->FACE_ICON_SLEEPY);
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

/* onMineExplosionEventTriggered() : When a player reveals a mine, a mineExplosionEvent() signal is
 * emitted by the GameController, and this method is called. All mines are displayed, and the mineExplosionEvent
 * is re-emitted by MainWindow, indicating that all UI elements of a game over are taken care of */
void MainWindow::onMineExplosionEventTriggered()
{
    using namespace QmsUtilities;
    displayAllMines();
    applicationSoundEffects->explosionEffect().play();
    emit(mineExplosionEvent());
}

/* onMuteSoundClicked() : Called when the "Mute Sound" menu option is
 * clicked. If the option is checked, all sounds from QMineSweeper
 * are disabled. If not, they are enabled */
void MainWindow::onActionMuteSoundChecked(bool checked) {
    applicationSoundEffects->setAudioMuted(checked);
    LOG_INFO() << QString{"Game audio volume set to %1"}.arg((checked ? QS_NUMBER(QmsSettingsLoader::DEFAULT_AUDIO_VOLUME) : "0"));
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
    this->m_boardResizeDialog->show(gameController->numberOfColumns(), gameController->numberOfRows());
    this->m_boardResizeDialog->centerAndFitWindow();
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
    if (!gameController->gameOver()) {
        emit(gameResumed());
    }
}

/* onAboutQmsActionTriggered() : Called when the About->About Qt menu
 * option is clicked, causing the About Qt window (supplied by the Qt toolchain)
 * to show. This separate event is also hooked, allowing the game to be paused if necessary */
void MainWindow::onAboutQtActionTriggered()
{
    emit(gamePaused());
    QMessageBox::aboutQt(this, QmsStrings::ABOUT_QT_WINDOW_TITLE);
    emit(gameResumed());
}

/* onAboutQMineSweeperActionTriggered() : Called when the About->About QMineSweeper menu
 * option is clicked, causing the About QMineSweeper window (defined in forms/aboutqmsdialog.ui)
 * to show. This method also pauses the game while the window is active */
void MainWindow::onAboutQMineSweeperActionTriggered()
{
    using namespace QmsGlobalSettings;
    this->setEnabled(false);
    emit(gamePaused());
    this->m_aboutQmsDialog->show();
}


/* onApplicationExit() : Called when the QApplication is about to close,
 * via hooking the QApplication::exit() event in main.cpp, empty by default */
void MainWindow::onApplicationExit() {

}


/* drawNumberOfSurroundingMines() : Called when a mine is about to be displayed
 * Queries the shared_ptr passed in for it's number of surrounding mines, then
 * uses the shared_ptr to the QMineSweeperIcons instance to set the graphic on the
 * QMineSweeperButton to the correct number via chained else-ifs */
void MainWindow::drawNumberOfSurroundingMines(QmsButton *msb)
{
    if (msb->numberOfSurroundingMines() == 0) {
        msb->setIcon(applicationIcons->COUNT_MINES_0);
    } else if (msb->numberOfSurroundingMines() == 1) {
        msb->setIcon(applicationIcons->COUNT_MINES_1);
    } else if (msb->numberOfSurroundingMines() == 2) {
        msb->setIcon(applicationIcons->COUNT_MINES_2);
    } else if (msb->numberOfSurroundingMines() == 3) {
        msb->setIcon(applicationIcons->COUNT_MINES_3);
    } else if (msb->numberOfSurroundingMines() == 4) {
        msb->setIcon(applicationIcons->COUNT_MINES_4);
    } else if (msb->numberOfSurroundingMines() == 5) {
        msb->setIcon(applicationIcons->COUNT_MINES_5);
    } else if (msb->numberOfSurroundingMines() == 6) {
        msb->setIcon(applicationIcons->COUNT_MINES_6);
    } else if (msb->numberOfSurroundingMines() == 7) {
        msb->setIcon(applicationIcons->COUNT_MINES_7);
    } else if (msb->numberOfSurroundingMines() == 8) {
        msb->setIcon(applicationIcons->COUNT_MINES_8);
    } else {
        msb->setIcon(applicationIcons->COUNT_MINES_0);
    }
}

/* ~MainWindow() : Destructor, empty by default, as all ownership is taken care
 * of by c++11's smart pointers (unique_ptr and shared_ptr) */
MainWindow::~MainWindow()
{
    delete this->m_ui;
}
