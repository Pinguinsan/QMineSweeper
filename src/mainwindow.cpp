/***********************************************************************
*    mainwindow.cpp:                                                   *
*    Main window for displaying the game                               *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source  file for QMineSweeper:                          *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the implementation of a custom QMainWindow class  *
*    MainWindow is the default container for a Qt Desktop Application  *
*    The source code is released under the GNU LGPL                    *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_boardresizewindow.h"

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
 * relevant events are hooked (QObject::connect()) to set up the game to play */
MainWindow::MainWindow(std::shared_ptr<QMineSweeperIcons> qmsiPtr,
                       std::shared_ptr<QMineSweeperSoundEffects> qmssePtr,
                       std::shared_ptr<GameController> gcPtr,
                       std::shared_ptr<QDesktopWidget> qdwPtr,
                       QWidget *parent) :
    QMainWindow{parent},
    m_eventTimer{std::make_unique<QTimer>()},
    m_playTimer{std::make_unique<EventTimer>()},
    m_userIdleTimer{std::make_unique<EventTimer>()},
    m_bsui{std::make_unique<Ui::BoardResizeWindow>()},
    m_ui{std::make_unique<Ui::MainWindow>()},
    m_boardSizeWindow{std::make_unique<BoardResizeWindow>()},
    m_qmsiPtr{qmsiPtr},
    m_qmssePtr{qmssePtr},
    m_gameController{gcPtr},
    m_qDesktopWidget{qdwPtr},
    m_xPlacement{0},
    m_yPlacement{0},
    m_currentDefaultMineSize{QSize{0,0}},
    m_currentMaxMineSize{QSize{0,0}},
    m_maxMineSizeCacheIsValid{false},
    m_tempPauseFlag{false},
    m_boardSizeGeometrySet{false}
{
    using namespace QMineSweeperStrings;
    this->m_ui->setupUi(this);
    this->m_bsui->setupUi(this->m_boardSizeWindow.get());
    this->m_boardSizeWindow->setWindowTitle(MAIN_WINDOW_TITLE);
    this->m_boardSizeWindow->setWindowIcon(this->m_qmsiPtr->MINE_ICON_72);
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
    this->m_eventTimer->setInterval(this->m_gameController->GAME_TIMER_INTERVAL());
    this->m_bsui->columnsBox->setTabOrder(this->m_bsui->columnsBox, this->m_bsui->rowsBox);

    connect(this->m_ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(this->m_ui->actionAboutQt, SIGNAL(triggered(bool)), this, SLOT(onAboutQtActionTriggered()));
    connect(this->m_ui->actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    connect(this->m_ui->resetButton, SIGNAL(clicked(bool)), this, SLOT(onResetButtonClicked()));
    connect(this->m_ui->actionBoardSize, SIGNAL(triggered(bool)), this, SLOT(onChangeBoardSizeActionTriggered()));
    connect(this->m_bsui->okayButton, SIGNAL(clicked(bool)), this, SLOT(onBsuiOkayButtonClicked()));
    connect(this->m_bsui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onBsuiCancelButtonClicked()));
    connect(this, SIGNAL(boardResize(int, int)), this->m_gameController.get(), SLOT(onBoardResizeTriggered(int, int)));
    connect(this->m_gameController.get(), SIGNAL(readyToBeginNewGame()), this, SLOT(setupNewGame()));
    connect(this, SIGNAL(mineDisplayed()), this->m_gameController.get(), SLOT(onMineDisplayed()));
    connect(this->m_gameController.get(), SIGNAL(winEvent()), this, SLOT(onGameWon()));
    connect(this, SIGNAL(winEvent()), this->m_gameController.get(), SLOT(onGameWon()));
    connect(this, SIGNAL(gamePaused()), this->m_gameController.get(), SLOT(onGamePaused()));
    connect(this->m_gameController.get(), SIGNAL(userIsNoLongerIdle()), this, SLOT(startUserIdleTimer()));
    connect(this, SIGNAL(mineSweeperButtonCreated(std::shared_ptr<QMineSweeperButton>)), this->m_gameController.get(), SLOT(onMineSweeperButtonCreated(std::shared_ptr<QMineSweeperButton>)));

    connect(this->m_eventTimer.get(), SIGNAL(timeout()), this, SLOT(eventLoop()));
    connect(this->m_gameController.get(), SIGNAL(gameStarted()), this, SLOT(onGameStarted()));
    connect(this, SIGNAL(resetGame()), this->m_gameController.get(), SLOT(onGameReset()));

    connect(this->m_gameController.get(), SIGNAL(numberOfMinesRemainingChanged(int)), this, SLOT(updateNumberOfMinesLCD(int)));
    connect(this->m_gameController.get(), SIGNAL(numberOfMovesMadeChanged(int)), this, SLOT(updateNumberOfMovesMadeLCD(int)));

    connect(this->m_ui->menuFile, SIGNAL(aboutToShow()), this->m_gameController.get(), SLOT(onContextMenuActive()));
    connect(this->m_ui->menuPreferences, SIGNAL(aboutToShow()), this->m_gameController.get(), SLOT(onContextMenuActive()));
    connect(this->m_ui->menuHelp, SIGNAL(aboutToShow()), this->m_gameController.get(), SLOT(onContextMenuActive()));

    connect(this->m_ui->menuFile, SIGNAL(aboutToHide()), this->m_gameController.get(), SLOT(onContextMenuInactive()));
    connect(this->m_ui->menuPreferences, SIGNAL(aboutToHide()), this->m_gameController.get(), SLOT(onContextMenuInactive()));
    connect(this->m_ui->menuHelp, SIGNAL(aboutToHide()), this->m_gameController.get(), SLOT(onContextMenuInactive()));

    connect(this->m_gameController.get(), SIGNAL(gameResumed()), this, SLOT(onGameResumed()));
    connect(this, SIGNAL(gameResumed()), this->m_gameController.get(), SLOT(onGameResumed()));
    connect(this->m_gameController.get(), SIGNAL(gamePaused()), this->m_gameController.get(), SLOT(onGamePaused()));

    connect(this, SIGNAL(mineExplosionEvent()), this->m_gameController.get(), SLOT(onMineExplosionEventTriggered()));
    connect(this->m_gameController.get(), SIGNAL(mineExplosionEvent()), this, SLOT(onMineExplosionEventTriggered()));
    connect(this->m_boardSizeWindow.get(), SIGNAL(aboutToClose()), this, SLOT(onCustomDialogClosed()));
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};

    #if defined(__ANDROID__)
        this->m_reductionSizeScaleFactor = 1;
    #else
        if (avail->height() > 800) {
            this->m_reductionSizeScaleFactor = 0.7;
        } else {
            this->m_reductionSizeScaleFactor = 0.5;
        }
    #endif
    this->m_eventTimer->start();
    this->updateNumberOfMovesMadeLCD(this->m_gameController->numberOfMovesMade());
    this->updateNumberOfMinesLCD(this->m_gameController->userDisplayNumberOfMines());
}

/* boardResizeDialogVisible() : Pass through function, delegating to
 * the shared_ptr of the board size form, returning whether or not
 * the form is visible, for use in pausing or unpausing the game */
bool MainWindow::boardResizeDialogVisible()
{
    return this->m_boardSizeWindow->isVisible();
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
    (void)event;
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
void MainWindow::onCustomDialogClosed()
{
    this->setEnabled(true);
    if (!this->m_gameController->initialClickFlag() && !this->m_gameController->gameOver()) {
        emit (gameResumed());
    }
}

/* onGameWon() : Called when the GameController class detects (via a mine click event,
 * flagging enough mines, or whatever win condition is defined) that the game has been won
 * Upon call, it iterates through the game board and sets the icons appropriately (green check if
 * a mine was correctly marked, red x if a flag was incorrectly marked, etc */
void MainWindow::onGameWon()
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_BIG_SMILEY);
    this->m_gameController->setGameOver(true);

    for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex < this->m_gameController->numberOfColumns(); columnIndex++) {
            std::shared_ptr<QMineSweeperButton> tempMsb = this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex);
            if(tempMsb->hasMine()) {
                if (tempMsb->hasFlag()) {
                    tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_CHECK);
                    tempMsb->setChecked(true);
                } else {
                    tempMsb->setIcon(this->m_qmsiPtr->MINE_ICON_72);
                    tempMsb->setChecked(true);
                    tempMsb->setStyleSheet(UNCOVERED_MINE_STYLESHEET);
                }
            } else if (tempMsb->hasFlag()) {
                tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_X);
                tempMsb->setChecked(true);
            }
            tempMsb->setIsRevealed(true);
        }
    }
    std::unique_ptr<QMessageBox> winBox{std::make_unique<QMessageBox>()};
    winBox->setWindowTitle(MAIN_WINDOW_TITLE);
    winBox->setText(WIN_DIALOG_BASE + toQString(this->m_gameController->numberOfMovesMade()) + WIN_DIALOG_MIDDLE + this->statusBar()->currentMessage());
    winBox->setWindowIcon(this->m_qmsiPtr->MINE_ICON_48);
    winBox->exec();
}

/* centerAndFitWindow() : Called whenever code requests the main window to be centered
 * Typically called after resizing the UI on the main window */
void MainWindow::centerAndFitWindow()
{
    this->setFixedSize(this->minimumSize());
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

/* onGameStarted() : Called when a gameStarted() signal is emitted
 * Start the game timer, as well as starting the user idle timer, which
 * is used to change the smiley face to a sleepy face */
void MainWindow::onGameStarted()
{
    startGameTimer();
    startUserIdleTimer();
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
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
    populateMineField();
    centerAndFitWindow();
}

/* onGamePaused() : Called when a gamePaused() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and
 * disable them, so the user cannot play the game until it is resumed */
void MainWindow::onGamePaused()
{
    for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gameController->numberOfColumns(); columnIndex++) {
            this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex)->setEnabled(false);
        }
    }
}

/* onGameResumed() : Called when a gameResumed() signal is emitted
 * Iterate through all of the QMineSweeperButtons on the mineFrame and enable them,
 * so the user can continue with their game */
void MainWindow::onGameResumed()
{
    for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gameController->numberOfColumns(); columnIndex++) {
            this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex)->setEnabled(true);
        }
    }
}

/* displayMine() : Called via the click handlers or other code (maybe onGameWon())
 * to provide a consistent way to display a mine on the MainWindow. First the number
 * of surrounding mines is drawn (the push button icon is set to the correct number
 * of surrounding mines), then a mineDisplayed() signal is emitted, to inform anything
 * connected that a mine is being displayed, then recursively check for other empty mines */
void MainWindow::displayMine(std::shared_ptr<QMineSweeperButton> msb)
{
    drawNumberOfSurroundingMines(msb);
    msb->setFlat(true);
    msb->setChecked(true);
    emit(mineDisplayed());
    if (msb->numberOfSurroundingMines() == 0) {
        this->m_gameController->checkForOtherEmptyMines(msb);
    }
}
void MainWindow::populateMineField()
{
    for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gameController->numberOfColumns(); columnIndex++) {
            this->m_gameController->addMineSweeperButton(columnIndex, rowIndex);
            std::shared_ptr<QMineSweeperButton> tempPtr = this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex);
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

void MainWindow::invalidateSizeCaches()
{
    this->m_maxMineSizeCacheIsValid = false;
    this->m_iconReductionSizeCacheIsValid = false;
}

QSize MainWindow::getIconReductionSize()
{
    using namespace QMineSweeperUtilities;
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

QSize MainWindow::getMaxMineSize()
{
    using namespace QMineSweeperStrings;
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
            std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
            errorBox->setWindowTitle(MAIN_WINDOW_TITLE);
            errorBox->setText(GENERIC_ERROR_MESSAGE);
            errorBox->setWindowIcon(this->m_qmsiPtr->MINE_ICON_48);
            errorBox->exec();
            exit(EXIT_FAILURE);
            QCoreApplication::exit(EXIT_FAILURE);
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

void MainWindow::resetResetButtonIcon()
{
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
}

void MainWindow::setResetButtonIcon(const QIcon &icon)
{
    this->m_ui->resetButton->setIcon(icon);
}

void MainWindow::displayAllMines()
{
    using namespace QMineSweeperStrings;
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_FROWNY);
    this->m_gameController->setGameOver(true);
    for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex < this->m_gameController->numberOfColumns(); columnIndex++) {
            std::shared_ptr<QMineSweeperButton> tempMsb = this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex);
            if(tempMsb->hasMine()) {
                if (tempMsb->hasFlag()) {
                    tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_CHECK);
                    tempMsb->setChecked(true);
                } else {
                    tempMsb->setIcon(this->m_qmsiPtr->MINE_ICON_72);
                    tempMsb->setChecked(true);
                    tempMsb->setStyleSheet(UNCOVERED_MINE_STYLESHEET);
                }
            } else if (tempMsb->hasFlag()) {
                tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_X);
                tempMsb->setChecked(true);
            }
            tempMsb->setIsRevealed(true);
        }
    }
}

QString MainWindow::saveStyleSheet() const
{
    return this->m_saveStyleSheet;
}

std::shared_ptr<QMineSweeperIcons> MainWindow::qmsiPtr() const
{
    return this->m_qmsiPtr;
}

std::shared_ptr<QMineSweeperSoundEffects> MainWindow::qmssePtr() const
{
    return this->m_qmssePtr;
}

void MainWindow::onResetButtonClicked()
{
    using namespace QMineSweeperStrings;
    emit(gamePaused());
    if (!this->m_gameController->gameOver() && !this->m_gameController->initialClickFlag()) {
        QMessageBox::StandardButton userReply;
        userReply = QMessageBox::question(this, START_NEW_GAME_WINDOW_TITLE, START_NEW_GAME_PROMPT, QMessageBox::Yes|QMessageBox::No);
        if (userReply == QMessageBox::Yes) {
            this->doGameReset();
        } else {
            emit(gameResumed());
        }
    } else {
        this->doGameReset();
    }
}

void MainWindow::doGameReset()
{
    using namespace QMineSweeperStrings;
    this->m_boardSizeWindow->hide();
    for (int columnIndex = 0; columnIndex < this->m_gameController->numberOfColumns(); columnIndex++) {
        for (int rowIndex = 0; rowIndex < this->m_gameController->numberOfRows(); rowIndex++) {
            std::shared_ptr<QMineSweeperButton> tempMsb{this->m_gameController->mineSweeperButtonAtIndex(columnIndex, rowIndex)};
            tempMsb->setChecked(false);
            tempMsb->setFlat(false);
            tempMsb->setStyleSheet(this->m_saveStyleSheet);
            tempMsb->setIcon(this->m_qmsiPtr->COUNT_MINES_0);
            tempMsb->setEnabled(true);
            tempMsb->setIsRevealed(false);
        }
    }
    emit(resetGame());
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
    this->m_ui->statusBar->showMessage(START_NEW_GAME_INSTRUCTION);
}

void MainWindow::eventLoop()
{
    updateVisibleGameTimer();
    updateUserIdleTimer();
    //updateGeometry();
}

void MainWindow::updateGeometry()
{
    if (this->size() != this->minimumSize()) {
        centerAndFitWindow();
    }
}

void MainWindow::startGameTimer()
{
    this->m_playTimer->start();
}

void MainWindow::startUserIdleTimer()
{
    this->m_userIdleTimer->start();
}

void MainWindow::updateVisibleGameTimer()
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    QString gameTime{""};
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_playTimer->isPaused()) {
            this->m_playTimer->unpause();
        }
        this->m_playTimer->update();
        if (this->m_playTimer->hours() == 0) {
            gameTime = toQString(this->m_playTimer->minutes())
                    + ":"
                    + toQString(this->m_playTimer->seconds())
                    + "." + toQString(this->m_playTimer->milliseconds());
        } else {
            gameTime = toQString(this->m_playTimer->hours())
                    + ":"
                    + toQString(this->m_playTimer->minutes())
                    + ":"
                    + toQString(this->m_playTimer->seconds())
                    + "."
                    + toQString(this->m_playTimer->milliseconds());
        }
        this->m_ui->statusBar->showMessage(gameTime);
    } else if (!this->m_gameController->initialClickFlag()){
        this->m_playTimer->pause();
        this->m_playTimer->update();
        if (this->m_playTimer->hours() == 0) {
            gameTime = toQString(this->m_playTimer->minutes())
                    + ":"
                    + toQString(this->m_playTimer->seconds())
                    + "."
                    + toQString(this->m_playTimer->milliseconds());
        } else {
            gameTime = toQString(this->m_playTimer->hours())
                                 + ":" + toQString(this->m_playTimer->minutes())
                                 + ":" + toQString(this->m_playTimer->seconds())
                                 + "." + toQString(this->m_playTimer->milliseconds());
        }
        this->m_ui->statusBar->showMessage(gameTime);
    } else {
        this->m_ui->statusBar->showMessage(START_NEW_GAME_INSTRUCTION);
    }
}

void MainWindow::updateUserIdleTimer()
{
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_userIdleTimer->isPaused()) {
            this->m_userIdleTimer->unpause();
        }
        this->m_playTimer->update();
    } else if (!this->m_gameController->initialClickFlag()) {
        this->m_userIdleTimer->pause();
        this->m_userIdleTimer->update();
    }
    if (this->m_gameController->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_userIdleTimer->totalTime() >= this->m_gameController->DEFAULT_SLEEPY_FACE_TIMEOUT()) {
            this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SLEEPY);
        }
    }
}

std::string MainWindow::getLCDDisplayPadding(int howMuch)
{
    std::string returnString{""};
    for (int i = 0; i < howMuch; i++) {
        returnString += "0";
    }
    return returnString;
}

void MainWindow::updateNumberOfMinesLCD(int numberOfMines)
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    if (numberOfMines > 999) {
        this->m_ui->minesRemaining->display(LCD_OVERFLOW_STRING);
    } else if (numberOfMines <= 0) {
        this->m_ui->minesRemaining->display(toQString(this->getLCDDisplayPadding(3)));
    } else if (numberOfMines < 10) {
        this->m_ui->minesRemaining->display(toQString(this->getLCDDisplayPadding(2)) + toQString(numberOfMines));
    } else if (numberOfMines < 100) {
        this->m_ui->minesRemaining->display(toQString(this->getLCDDisplayPadding(1)) + toQString(numberOfMines));
    } else {
        this->m_ui->minesRemaining->display(numberOfMines);
    }
}

void MainWindow::updateNumberOfMovesMadeLCD(int numberOfMovesMade)
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    if (numberOfMovesMade > 999) {
        this->m_ui->numberOfMoves->display(LCD_OVERFLOW_STRING);
    } else if (numberOfMovesMade <= 0) {
        this->m_ui->numberOfMoves->display(toQString(this->getLCDDisplayPadding(3)));
    } else if (numberOfMovesMade < 10) {
        this->m_ui->numberOfMoves->display(toQString(this->getLCDDisplayPadding(2)) + toQString(numberOfMovesMade));
    } else if (numberOfMovesMade < 100) {
        this->m_ui->numberOfMoves->display(toQString(this->getLCDDisplayPadding(1)) + toQString(numberOfMovesMade));
    } else {
        this->m_ui->numberOfMoves->display(numberOfMovesMade);
    }

}

void MainWindow::onMineExplosionEventTriggered()
{
    using namespace QMineSweeperUtilities;
    displayAllMines();
    this->m_qmssePtr->explosionEffect.play();
    emit(mineExplosionEvent());
}

void MainWindow::onChangeBoardSizeActionTriggered()
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    this->setEnabled(false);
    this->m_bsui->statusBar->showMessage(RESIZE_BOARD_WINDOW_CURRENT_BOARD_SIZE_STRING
                                         + toQString(this->m_gameController->numberOfColumns())
                                         + RESIZE_BOARD_WINDOW_CONFIRMATION_MIDDLE
                                         + toQString(this->m_gameController->numberOfRows()));
    this->m_bsui->columnsBox->setValue(this->m_gameController->numberOfColumns());
    this->m_bsui->rowsBox->setValue(this->m_gameController->numberOfRows());
#if defined(__ANDROID__)
    if (!this->m_boardSizeGeometrySet) {
        this->m_bsui->okayButton->setFixedSize(this->m_bsui->okayButton->size()*4);
        this->m_bsui->cancelButton->setFixedSize(this->m_bsui->cancelButton->size()*4);
        this->m_boardSizeWindow->setFixedHeight(this->m_qDesktopWidget->availableGeometry().height() / 5);
        this->m_boardSizeWindow->setFixedWidth(this->m_qDesktopWidget->availableGeometry().width());
        QFont font;
        font.setPointSize(font.pointSize() + 50);
        this->m_bsui->columnsBox->setFont(font);
        this->m_bsui->rowsBox->setFont(font);

        this->m_bsui->columnsBox->setFixedSize(this->m_bsui->columnsBox->size()*4);
        this->m_bsui->rowsBox->setFixedSize(this->m_bsui->rowsBox->size()*4);
        this->m_boardSizeGeometrySet = true;
    }
#endif
    this->m_boardSizeWindow->show();
    emit(gamePaused());
}

void MainWindow::onBsuiOkayButtonClicked()
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    this->setEnabled(true);
    this->m_boardSizeWindow->hide();
    int maybeNewColumns{this->m_bsui->columnsBox->value()};
    int maybeNewRows{this->m_bsui->rowsBox->value()};
    if ((maybeNewColumns == this->m_gameController->numberOfColumns()) && (maybeNewRows == this->m_gameController->numberOfRows())) {
        if (!this->m_gameController->gameOver()) {
            emit(gameResumed());
        }
        return;
    }
    QMessageBox::StandardButton userReply;
    QString questionBoxMessage = RESIZE_BOARD_WINDOW_CONFIRMATION_BASE
            + toQString(maybeNewColumns)
            + RESIZE_BOARD_WINDOW_CONFIRMATION_MIDDLE
            + toQString(maybeNewRows)
            + RESIZE_BOARD_WINDOW_CONFIRMATION_TAIL;
    userReply = QMessageBox::question(this, START_NEW_GAME_WINDOW_TITLE, questionBoxMessage, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        this->invalidateSizeCaches();
        emit(boardResize(maybeNewColumns, maybeNewRows));
    } else {
        if (!this->m_gameController->gameOver()) {
            emit(gameResumed());
        }
    }
}

void MainWindow::onBsuiCancelButtonClicked()
{
    this->setEnabled(true);
    this->m_boardSizeWindow->hide();
    this->show();
    if (!this->m_gameController->gameOver()) {
        emit(gameResumed());
    }
}

/* onAboutQtActionTriggered() : Called when the About->About Qt menu
 * option is clicked, causing the About Qt window (supplied by the Qt toolchain)
 * to show. This separate event is also hooked, allowing the game to be paused if necessary */
void MainWindow::onAboutQtActionTriggered()
{
    /*
    if ((this->m_gcPtr->gameState() == GameState::GAME_ACTIVE) && (!this->m_gcPtr->gameOver())) {
        this->setEnabled(false);
        this->m_tempPauseFlag = true;
        emit(gamePaused());
    }
    */
}


/* onApplicationExit() : Called when the QApplication is about to close,
 * via hooking the QApplication::exit() event in main.cpp, empty by default */
void MainWindow::onApplicationExit()
{

}

/* bindGameController() : Convenience function for late binding of a shared_ptr
 * to the GameController instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindGameController(std::shared_ptr<GameController> gc)
{
    this->m_gameController.reset();
    this->m_gameController = gc;
}

/* bindQDesktopWidget() : Convenience function for late binding of a shared_ptr
 * to the QDesktopWidget instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qdw)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qdw;
}

/* bindQMineSweeperIcons() : Convenience function for late binding of a shared_ptr
 * to the QMineSweeperIcons instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQMineSweeperIcons(std::shared_ptr<QMineSweeperIcons> qmsiPtr)
{
    this->m_qmsiPtr.reset();
    this->m_qmsiPtr = qmsiPtr;
}

/* bindGameController() : Convenience function for late binding of a shared_ptr
 * to the GameController instance, to support dependancy injection for MainWindow,
 * as this shared_ptr is passed in via the constructor */
void MainWindow::bindQMineSweeperSoundEffects(std::shared_ptr<QMineSweeperSoundEffects> qmssePtr)
{
    this->m_qmssePtr.reset();
    this->m_qmssePtr = qmssePtr;
}

/* drawNumberOfSurroundingMines() : Called when a mine is about to be displayed
 * Queries the shared_ptr passed in for it's number of surrounding mines, then
 * uses the shared_ptr to the QMineSweeperIcons instance to set the graphic on the
 * QMineSweeperButton to the correct number via chained else-ifs */
void MainWindow::drawNumberOfSurroundingMines(std::shared_ptr<QMineSweeperButton> msb)
{
    if (msb->numberOfSurroundingMines() == 0) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_0);
    } else if (msb->numberOfSurroundingMines() == 1) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_1);
    } else if (msb->numberOfSurroundingMines() == 2) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_2);
    } else if (msb->numberOfSurroundingMines() == 3) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_3);
    } else if (msb->numberOfSurroundingMines() == 4) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_4);
    } else if (msb->numberOfSurroundingMines() == 5) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_5);
    } else if (msb->numberOfSurroundingMines() == 6) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_6);
    } else if (msb->numberOfSurroundingMines() == 7) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_7);
    } else if (msb->numberOfSurroundingMines() == 8) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_8);
    } else {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_0);
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
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
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
