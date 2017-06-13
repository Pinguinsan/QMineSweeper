/***********************************************************************
*    mainwindow.h:                                                     *
*    Main window for displaying the game                               *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a custom QMainWindow class    *
*    MainWindow is the default container for a Qt Desktop Application  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_MAINWINDOW_H
#define QMINESWEEPER_MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <iostream>
#include <memory>

#include "eventtimer.h"
#include "qmssettingsloader.h"


namespace Ui
{
    class MainWindow;
}

class QmsButton;
class GameController;
class AboutQmsDialog;
class BoardResizeDialog;
class QmsIcons;
class QmsSoundEffects;
class QmsApplicationSettings;
class QDesktopWidget;
class QMediaPlayer;
class QDialog;
class QWindow;
class QString;
class QTimer;
class QActionGroup;
class QTranslator;
class QLabel;
class QDialog;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<QmsIcons> gameIcons,
                           std::shared_ptr<QmsSoundEffects> gameSoundEffects,
                           std::shared_ptr<QmsSettingsLoader> settingsLoader,
                           std::shared_ptr<GameController> gameController,
                           std::shared_ptr<QDesktopWidget> desktopWidget,
                           QmsSettingsLoader::SupportedLanguage initialDisplayLanguage,
                           QWidget *parent = nullptr);
    ~MainWindow();

    void populateMineField();
    void displayAllMines();
    void resizeResetIcon();
    void displayMine(QmsButton *msb);
    void bindGameController(std::shared_ptr<GameController> gameController);
    void bindQMineSweeperIcons(std::shared_ptr<QmsIcons> programIcons);
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void bindQMineSweeperSoundEffects(std::shared_ptr<QmsSoundEffects> programSoundEffects);
    void bindQMineSweeperSettingsLoader(std::shared_ptr<QmsSettingsLoader> programSettingsLoader);
    void setResetButtonIcon(const QIcon &icon);
    void drawNumberOfSurroundingMines(QmsButton *msb);
    void setLanguage(QmsSettingsLoader::SupportedLanguage newLanguage);
    int xPlacement() const;
    int yPlacement() const;
    void centerAndFitWindow();
    bool boardResizeDialogVisible();

    QmsApplicationSettings collectApplicationSettings() const;
    std::shared_ptr<QmsIcons> gameIcons() const;
    std::shared_ptr<QmsSoundEffects> gameSounds() const;
    QString saveStyleSheet() const;

private:
    std::unique_ptr<QTimer> m_eventTimer;
    std::unique_ptr<EventTimer<std::chrono::steady_clock>> m_userIdleTimer;
    std::unique_ptr<Ui::MainWindow> m_ui;
    std::unique_ptr<AboutQmsDialog> m_aboutQmsDialog;
    std::unique_ptr<BoardResizeDialog>  m_boardSizeDialog;
    std::unique_ptr<QActionGroup> m_languageActionGroup;
    std::unique_ptr<QTranslator> m_translator;
    std::unique_ptr<QLabel> m_statusBarLabel;
    std::unique_ptr<QMediaPlayer> m_soundEffects;
    std::shared_ptr<QmsIcons> m_gameIcons;
    std::shared_ptr<QmsSoundEffects> m_gameSoundEffects;
    std::shared_ptr<QmsSettingsLoader> m_settingsLoader;
    std::shared_ptr<GameController> m_gameController;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;


    QmsSettingsLoader::SupportedLanguage m_language;
    double m_reductionSizeScaleFactor;
    QString m_saveStyleSheet;
    int m_xPlacement;
    int m_yPlacement;
    QSize m_currentDefaultMineSize;
    QSize m_currentMaxMineSize;
    QSize m_currentIconReductionSize;
    bool m_maxMineSizeCacheIsValid;
    bool m_iconReductionSizeCacheIsValid;
    bool m_tempPauseFlag;
    bool m_boardSizeGeometrySet;
    QString m_saveFilePath;

    static const int s_TASKBAR_HEIGHT;
    static const int s_GAME_TIMER_INTERVAL;
    static const double s_DEFAULT_NUMBER_OF_MINES;
    static const int s_HEIGHT_SCALE_FACTOR;
    static const int s_WIDTH_SCALE_FACTOR;
    static const int s_NUMBER_OF_HORIZONTAL_MARGINS;
    static const int s_NUMBER_OF_VERTIAL_MARGINS;
    static const int s_DEFAULT_MINE_SIZE_SCALE_FACTOR;
    static const int s_STATUS_BAR_FONT_POINT_SIZE;
    static const double s_MINE_ICON_REDUCTION_SCALE_FACTOR;

    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void invalidateSizeCaches();
    void doGameReset();
    QSize getMaxMineSize();
    QSize getIconReductionSize();
    void calculateXYPlacement();
    std::string getLCDPadding(uint8_t howMuch);
    static const long long int constexpr MILLISECONDS_PER_SECOND{1000};
    static const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
    static const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
    static const long long int constexpr MILLISECONDS_PER_HOUR{3600000};

    void displayStatusMessage(QString statusMessage);
    void doSaveGame(const QString &filePath);
signals:
    void resetButtonClicked();
    void resetGame();
    void mineExplosionEvent();
    void boardResize(int columns, int rows);
    void gamePaused();
    void gameResumed();
    void mineDisplayed();
    void winEvent();
    void mineSweeperButtonCreated(std::shared_ptr<QmsButton> msb);

public slots:
    void resetResetButtonIcon();
    void onGameStarted();
    void onGamePaused();
    void onGameResumed();
    void onMineExplosionEventTriggered();
    void setupNewGame();
    void onGameWon();
    void onBoardResizeDialogClosed(int columns, int rows, QDialog::DialogCode userResult);
    void startUserIdleTimer();

private slots:
    void eventLoop();
    void onResetButtonClicked();
    void onAboutQtActionTriggered();
    void onAboutQMineSweeperActionTriggered();
    void onAboutQmsWindowClosed();
    void onApplicationExit();
    void updateVisibleGameTimer();
    void updateGeometry();
    void updateUserIdleTimer();
    void startGameTimer();
    void onChangeBoardSizeActionTriggered();

    void updateNumberOfMovesMadeLCD(int numberOfMovesMade);
    void updateNumberOfMinesLCD(int numberOfMines);
    void onActionMuteSoundChecked(bool checked);

    void onLanguageSelected(bool triggered);
    void onSaveActionTriggered();
    void onSaveAsActionTriggered();
    void onOpenActionTriggered();
};

#endif // QMINESWEEPER_MAINWINDOW_H
