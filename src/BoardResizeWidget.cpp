/***********************************************************************
*    boardresizedialog.cpp:                                            *
*    Custom QDialog for settings the board dimensions                  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a BoardResizeDialog class   *
*    The BoardResizeDialog is used to change the size of the game board*
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "BoardResizeWidget.h"
#include "ui_BoardResizeWidget.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QString>
#include <QRect>

#include "QmsUtilities.h"
#include "QmsStrings.h"
#include "QmsApplicationSettings.h"
#include "GlobalDefinitions.h"

const std::pair<int, int> BoardResizeWidget::BEGINNER_GAME_DIMENSIONS{9, 9};
const std::pair<int, int> BoardResizeWidget::INTERMEDIATE_GAME_DIMENSIONS{18, 9};
const std::pair<int, int> BoardResizeWidget::ADVANCED_GAME_DIMENSIONS{18, 18};
const std::pair<int, int> BoardResizeWidget::EXTREME_GAME_DIMENSIONS{24, 24};

BoardResizeWidget::BoardResizeWidget(QWidget *parent) :
    MouseMoveableQWidget{parent},
    m_ui{new Ui::BoardResizeWidget{}},
    m_numberOfColumns{0},
    m_numberOfRows{0},
    m_resultToEmit{ GameBoardSize{0, 0, 0}, BoardResizeWidget::ResizeWidgetExitCode::Rejected}
{
    this->m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    this->connect(this->m_ui->btnOkay, &QPushButton::clicked, this, &BoardResizeWidget::onOkayButtonClicked);
    this->connect(this->m_ui->btnCancel, &QPushButton::clicked, this, &BoardResizeWidget::onCancelButtonClicked);

    this->connect(this->m_ui->btnIncrementRows, &QPushButton::clicked, this, &BoardResizeWidget::onBtnIncrementRowsClicked);
    this->connect(this->m_ui->btnDecrementRows, &QPushButton::clicked, this, &BoardResizeWidget::onBtnDecrementRowsClicked);
    this->connect(this->m_ui->btnIncrementColumns, &QPushButton::clicked, this, &BoardResizeWidget::onBtnIncrementColumnsClicked);
    this->connect(this->m_ui->btnDecrementColumns, &QPushButton::clicked, this, &BoardResizeWidget::onBtnDecrementColumnsClicked);

    this->connect(this->m_ui->btnBeginner, &QPushButton::clicked, this, &BoardResizeWidget::onBtnBeginnerPresetClicked);
    this->connect(this->m_ui->btnIntermediate, &QPushButton::clicked, this, &BoardResizeWidget::onBtnIntermediatePresetClicked);
    this->connect(this->m_ui->btnAdvanced, &QPushButton::clicked, this, &BoardResizeWidget::onBtnAdvancedPresetClicked);
    this->connect(this->m_ui->btnExtreme, &QPushButton::clicked, this, &BoardResizeWidget::onBtnExtremePresetClicked);

    
    
    this->m_ui->btnBeginner->setToolTip(
            QString{this->m_ui->btnBeginner->toolTip()}.arg(
                    QString{"%1x%2"}.arg(QS_NUMBER(BEGINNER_GAME_DIMENSIONS.first), QS_NUMBER(BEGINNER_GAME_DIMENSIONS.second))
            )
    );

    this->m_ui->btnIntermediate->setToolTip(
            QString{this->m_ui->btnIntermediate->toolTip()}.arg(
                    QString{"%1x%2"}.arg(QS_NUMBER(INTERMEDIATE_GAME_DIMENSIONS.first), QS_NUMBER(INTERMEDIATE_GAME_DIMENSIONS.second))
            )
    );

    this->m_ui->btnAdvanced->setToolTip(
            QString{this->m_ui->btnAdvanced->toolTip()}.arg(
                    QString{"%1x%2"}.arg(QS_NUMBER(ADVANCED_GAME_DIMENSIONS.first), QS_NUMBER(ADVANCED_GAME_DIMENSIONS.second))
            )
    );

    this->m_ui->btnExtreme->setToolTip(
            QString{this->m_ui->btnExtreme->toolTip()}.arg(
                    QString{"%1x%2"}.arg(QS_NUMBER(EXTREME_GAME_DIMENSIONS.first), QS_NUMBER(EXTREME_GAME_DIMENSIONS.second))
            )
    );

    this->m_ui->btnBeginner->setProperty(
            QmsStrings::RESIZE_BOARD_BUTTON_GAME_DIMENSIONS_KEY,
            QString{"%1x%2"}.arg(QS_NUMBER(BEGINNER_GAME_DIMENSIONS.first), QS_NUMBER(BEGINNER_GAME_DIMENSIONS.second))
    );

    this->m_ui->btnIntermediate->setProperty(
            QmsStrings::RESIZE_BOARD_BUTTON_GAME_DIMENSIONS_KEY,
            QString{"%1x%2"}.arg(QS_NUMBER(INTERMEDIATE_GAME_DIMENSIONS.first), QS_NUMBER(INTERMEDIATE_GAME_DIMENSIONS.second))
    );

    this->m_ui->btnAdvanced->setProperty(
            QmsStrings::RESIZE_BOARD_BUTTON_GAME_DIMENSIONS_KEY,
            QString{"%1x%2"}.arg(QS_NUMBER(ADVANCED_GAME_DIMENSIONS.first), QS_NUMBER(ADVANCED_GAME_DIMENSIONS.second))
    );

    this->m_ui->btnExtreme->setProperty(
            QmsStrings::RESIZE_BOARD_BUTTON_GAME_DIMENSIONS_KEY,
            QString{"%1x%2"}.arg(QS_NUMBER(EXTREME_GAME_DIMENSIONS.first), QS_NUMBER(EXTREME_GAME_DIMENSIONS.second))
    );
}

void BoardResizeWidget::onBtnBeginnerPresetClicked(bool down)
{
    Q_UNUSED(down);
    this->onPresetBoardSizeActionTriggered(this->m_ui->btnBeginner);
}

void BoardResizeWidget::onBtnIntermediatePresetClicked(bool down)
{
    Q_UNUSED(down);
    this->onPresetBoardSizeActionTriggered(this->m_ui->btnIntermediate);
}

void BoardResizeWidget::onBtnAdvancedPresetClicked(bool down)
{
    Q_UNUSED(down);
    this->onPresetBoardSizeActionTriggered(this->m_ui->btnAdvanced);
}

void BoardResizeWidget::onBtnExtremePresetClicked(bool down)
{
    Q_UNUSED(down);
    this->onPresetBoardSizeActionTriggered(this->m_ui->btnExtreme);
}

void BoardResizeWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->m_resultToEmit.boardSize.columns = 0;
    this->m_resultToEmit.boardSize.rows = 0;
    this->m_resultToEmit.userAction = BoardResizeWidget::ResizeWidgetExitCode::Rejected;
}

void BoardResizeWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit(this->aboutToClose(this->m_resultToEmit));
}

void BoardResizeWidget::show(int columns, int rows)
{
    this->m_ui->lblColumns->setText(QS_NUMBER(columns));
    this->m_ui->lblRows->setText(QS_NUMBER(rows));
    this->m_numberOfColumns = columns;
    this->m_numberOfRows = rows;
    this->setVisible(true);
}

void BoardResizeWidget::onPresetBoardSizeActionTriggered(QPushButton *pressedButton)
{
    using namespace QmsUtilities;
    std::string rawString{pressedButton->property(QmsStrings::RESIZE_BOARD_BUTTON_GAME_DIMENSIONS_KEY).toString().toStdString()};
    std::transform(rawString.begin(), rawString.end(), rawString.begin(), ::tolower);
    rawString.erase(std::remove_if(rawString.begin(), rawString.end(), [](auto c) { return (!std::isdigit(c) && (c != 'x') && (c != ' ')); }), rawString.end());
    auto foundBeginningSpace = rawString.find_last_of(' ');
    auto maybeDimensions = rawString.substr(foundBeginningSpace + 1);
    auto dimensions = tryParseDimensions(maybeDimensions);
    if ((dimensions.first == -1) || (dimensions.second == -1)) {
        throw std::runtime_error(QString{"In BoardResizeDialog::onBeginnerBoardSizeTriggered() : dimensions parsed from QPushButton menu item are not valid (QAction::text() = \"%1\", this should never happen)"}.arg(maybeDimensions.c_str()).toStdString());
    }
    this->m_ui->lblColumns->setText(QS_NUMBER(dimensions.first));
    this->m_ui->lblRows->setText(QS_NUMBER(dimensions.second));
}

void BoardResizeWidget::onOkayButtonClicked(bool checked)
{
    Q_UNUSED(checked);
    using namespace QmsUtilities;
    using namespace QmsStrings;
    int maybeNewColumns{STRING_TO_INT(this->m_ui->lblColumns->text().toStdString())};
    int maybeNewRows{STRING_TO_INT(this->m_ui->lblRows->text().toStdString())};
    if ((maybeNewColumns == this->m_numberOfColumns) && (maybeNewRows == this->m_numberOfRows)) {
        this->onCancelButtonClicked(false);
        return;
    }
    QMessageBox::StandardButton userReply{};
    QString questionBoxMessage{QString{"%1%2%3%4%5"}.arg(RESIZE_BOARD_WINDOW_CONFIRMATION_BASE,
                                                         QS_NUMBER(maybeNewColumns),
                                                         RESIZE_BOARD_WINDOW_CONFIRMATION_MIDDLE,
                                                         QS_NUMBER(maybeNewRows),
                                                         RESIZE_BOARD_WINDOW_CONFIRMATION_TAIL)};
    userReply = static_cast<QMessageBox::StandardButton>(QMessageBox::question(this, START_NEW_GAME_WINDOW_TITLE, questionBoxMessage, QMessageBox::Yes|QMessageBox::No));
    if (userReply == QMessageBox::Yes) {
        this->m_resultToEmit.boardSize.columns = this->m_ui->lblColumns->text().toInt();
        this->m_resultToEmit.boardSize.rows = this->m_ui->lblRows->text().toInt();
        this->m_resultToEmit.userAction = BoardResizeWidget::ResizeWidgetExitCode::Accepted;
        this->close();
    } else {
        this->onCancelButtonClicked(false);
    }
}

void BoardResizeWidget::onBtnIncrementRowsClicked(bool down)
{
    Q_UNUSED(down);
    int currentRows{STRING_TO_INT(this->m_ui->lblRows->text().toStdString())};
    this->m_ui->lblRows->setText(QS_NUMBER(currentRows + 1));
}

void BoardResizeWidget::onBtnDecrementRowsClicked(bool down) {
    Q_UNUSED(down);
    int currentRows{STRING_TO_INT(this->m_ui->lblRows->text().toStdString())};
    if (currentRows != 0) {
        this->m_ui->lblRows->setText(QS_NUMBER(currentRows - 1));
    }
}


void BoardResizeWidget::onBtnIncrementColumnsClicked(bool down)
{
    Q_UNUSED(down);
    int currentColumns{STRING_TO_INT(this->m_ui->lblColumns->text().toStdString())};
    this->m_ui->lblColumns->setText(QS_NUMBER(currentColumns + 1));
}

void BoardResizeWidget::onBtnDecrementColumnsClicked(bool down)
{
    Q_UNUSED(down);
    int currentColumns{STRING_TO_INT(this->m_ui->lblColumns->text().toStdString())};
    if (currentColumns != 0) {
        this->m_ui->lblColumns->setText(QS_NUMBER(currentColumns - 1));
    }
}


void BoardResizeWidget::onCancelButtonClicked(bool checked)
{
    Q_UNUSED(checked);
    this->m_resultToEmit.boardSize.columns = 0;
    this->m_resultToEmit.boardSize.rows = 0;
    this->m_resultToEmit.userAction = BoardResizeWidget::ResizeWidgetExitCode::Rejected;
    this->close();
}



BoardResizeWidget::~BoardResizeWidget()
{

}

