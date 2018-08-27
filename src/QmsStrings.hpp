#ifndef QMINESWEEPER_QMSSTRINGS_HPP
#define QMINESWEEPER_QMSSTRINGS_HPP

#include <string>

namespace QmsStrings {
    const char *const MAIN_WINDOW_TITLE{"QMineSweeper"};
    const char *const GENERIC_ERROR_MESSAGE{"Something has gone terribly wrong and the application must exit"};
    const char *const START_NEW_GAME_WINDOW_TITLE{"Start new game?"};
    const char *const START_NEW_GAME_PROMPT{"Are you sure you'd like to reset the current game?"};
    const char *const START_NEW_GAME_INSTRUCTION{"Click on a minesweeper button to begin"};
    const char *const CLOSE_APPLICATION_WINDOW_TITLE{"Quit QMineSweeper?"};
    const char *const CLOSE_APPLICATION_WINDOW_PROMPT{"Are you sure you'd like to quit?"};

    const char *const ERROR_LOADING_FILE_TITLE{"Error Loading File"};
    const char *const ERROR_LOADING_FILE_MESSAGE{"An error occurred while loading file %1 (%2)"};

    const char *const ERROR_SAVING_FILE_TITLE{"Error Saving File"};
    const char *const ERROR_SAVING_FILE_MESSAGE{"An error occurred while saving file %1 (%2)"};

    const char *const SUCCESSFULLY_SAVED_GAME_FILE_TITLE{"Saved File Successfully"};
    const char *const SUCCESSFULLY_SAVED_GAME_FILE_MESSAGE{"Successfully saved game file to %1"};

    const char *const RESIZE_BOARD_WINDOW_CURRENT_BOARD_SIZE_STRING{"Current (columns x rows): "};
    const char *const RESIZE_BOARD_WINDOW_CONFIRMATION{
            "Are you sure you'd like to end the current %1x%2 game and start a new %3x%4 game?"};

    const char *const FAILED_TO_LOAD_GAME_STATE_TITLE{"Failed to load saved game"};
    const char *const FAILED_TO_LOAD_GAME_STATE{"Load game failed with the following error: \"%1\""};

    const char *const WIN_DIALOG{"You win! It took %1 moves and your total play time was %2"};
    const char *const UNCOVERED_NON_MINE_STYLESHEET{"color: rgb(200, 170, 255); background-color: rgb(200, 170, 255);"};
    const char *const UNCOVERED_MINE_STYLESHEET{"color: rgb(255, 0, 0); background-color: rgb(255, 0, 0);"};
    const char *const LONG_CLICKED_MINE_STYLESHEET{"color: rgb(0, 255, 0); background-color: rgb(0, 255, 0);"};
    const char *const LICENSE_PATH_KEY{"LicensePath"};

    const char *const ENGLISH_STRING{"English"};
    const char *const FRENCH_STRING{"French"};
    const char *const SPANISH_STRING{"Spanish"};
    const char *const JAPANESE_STRING{"Japanese"};

    const char *const SAVED_GAME_FILE_EXTENSION{".qms"};
    const char *const SAVE_FILE_CAPTION{"Save game?"};
    const char *const OPEN_FILE_CAPTION{"Open existing game?"};

    const char *const ABOUT_QT_WINDOW_TITLE{"About Qt"};

    const char *const ENGLISH_TRANSLATION_PATH{":/translations/english.qm"};
    const char *const JAPANESE_TRANSLATION_PATH{":/translations/japanese.qm"};

    const char *const MINE_ICON_16_PATH{":/mine-icons/png/mine-icons/16-qminesweeper.png"};
    const char *const MINE_ICON_24_PATH{":/mine-icons/png/mine-icons/24-qminesweeper.png"};
    const char *const MINE_ICON_32_PATH{":/mine-icons/png/mine-icons/32-qminesweeper.png"};
    const char *const MINE_ICON_48_PATH{":/mine-icons/png/mine-icons/48-qminesweeper.png"};
    const char *const MINE_ICON_64_PATH{":/mine-icons/png/mine-icons/64-qminesweeper.png"};
    const char *const MINE_ICON_72_PATH{":/mine-icons/png/mine-icons/72-qminesweeper.png"};
    const char *const MINE_ICON_96_PATH{":/mine-icons/png/mine-icons/96-qminesweeper.png"};
    const char *const MINE_ICON_128_PATH{":/mine-icons/png/mine-icons/128-qminesweeper.png"};
    const char *const FACE_ICON_BIG_SMILEY_PATH{":/face-icons/png/face-icons/big-smiley-face.png"};
    const char *const FACE_ICON_CRAZY_PATH{":/face-icons/png/face-icons/crazy-face.png"};
    const char *const FACE_ICON_FROWNY_PATH{":/face-icons/png/face-icons/frowny-face.png"};
    const char *const FACE_ICON_SLEEPY_PATH{":/face-icons/png/face-icons/sleepy-face.png"};
    const char *const FACE_ICON_SMILEY_PATH{":/face-icons/png/face-icons/smiley-face.png"};
    const char *const FACE_ICON_WINKY_PATH{":/face-icons/png/face-icons/winky-face.png"};
    const char *const COUNT_MINES_0_PATH{};
    const char *const COUNT_MINES_1_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/1.png"};
    const char *const COUNT_MINES_2_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/2.png"};
    const char *const COUNT_MINES_3_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/3.png"};
    const char *const COUNT_MINES_4_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/4.png"};
    const char *const COUNT_MINES_5_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/5.png"};
    const char *const COUNT_MINES_6_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/6.png"};
    const char *const COUNT_MINES_7_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/7.png"};
    const char *const COUNT_MINES_8_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/8.png"};
    const char *const STATUS_ICON_FLAG_PATH{":/status-icons/png/status-icons/flag.png"};
    const char *const STATUS_ICON_QUESTION_PATH{":/status-icons/png/status-icons/question-mark.png"};
    const char *const STATUS_ICON_FLAG_CHECK_PATH{":/status-icons/png/status-icons/flag-check.png"};
    const char *const STATUS_ICON_FLAG_X_PATH{":/status-icons/png/status-icons/flag-x.png"};

    const char *const EXPLOSION_EFFECT_SOURCE_PATH{"qrc:///sounds/sounds/explosion-fast.wav"};

    const char *const QMINESWEEPER_LICENSE_PATH{":/licenses/licenses/LICENSE"};
    const char *const RESIZE_BOARD_BUTTON_GAME_DIMENSIONS_KEY{"GameDimensions"};
    const char *const RESIZE_BOARD_BUTTON_MINE_RATIO_KEY{"MineRatio"};
    const char *const RESIZE_BOARD_BUTTON_LAST_MOUSE_DOWN_TIME{"LastMouseDownTime"};
    const char *const RESIZE_BOARD_BUTTON_LAST_MOUSE_TIME{"IsBeingLongPressed"};

    static const char *const QMS_GAME_STATE_XML_KEY{"QmsGameState"};
    static const char *const COLUMN_COUNT_XML_KEY{"NumberOfColumns"};
    static const char *const ROW_COUNT_XML_KEY{"NumberOfRows"};
    static const char *const MINE_COUNT_XML_KEY{"NumberOfMines"};
    static const char *const MOVES_MADE_COUNT_XML_KEY{"NumberOfMovesMade"};
    static const char *const MINES_REMAINING_COUNT_XML_KEY{"NumberOfMinesRemaining"};
    static const char *const PLAY_TIMER_START_ELEMENT_XML_KEY{"PlayTimer"};
    static const char *const PLAY_TIMER_IS_PAUSED_XML_KEY{"IsPaused"};
    static const char *const PLAY_TIMER_TOTAL_TIME_XML_KEY{"TotalTime"};
    static const char *const MINE_COORDINATE_LIST_XML_KEY{"MineCoordinateList"};
    static const char *const MINE_COORDINATES_XML_KEY{"MineCoordinates"};
    static const char *const QMS_BUTTON_LIST_START_ELEMENT_XML_KEY{"QmsButtons"};
    static const char *const QMS_BUTTON_START_ELEMENT_XML_KEY{"QmsButton"};
    static const char *const QMS_BUTTON_MINE_COORDINATES_XML_KEY{"MineCoordinates"};
    static const char *const QMS_BUTTON_IS_BLOCKING_CLICKS_XML_KEY{"IsBlockingClicks"};
    static const char *const QMS_BUTTON_SURROUNDING_MINE_COUNT_XML_KEY{"NumberOfSurroundingMines"};
    static const char *const QMS_BUTTON_IS_CHECKED_XML_KEY{"IsChecked"};
    static const char *const QMS_BUTTON_HAS_FLAG_XML_KEY{"HasFlag"};
    static const char *const QMS_BUTTON_HAS_MINE_XML_KEY{"HasMine"};
    static const char *const QMS_BUTTON_IS_REVEALED_XML_KEY{"IsRevealed"};

}

#endif //QMINESWEEPER_QMSSTRINGS_H
