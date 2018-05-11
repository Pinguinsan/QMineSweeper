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

#include "BoardResizeWidget.hpp"
#include "ui_BoardResizeWidget.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QString>
#include <QRect>

#include "QmsUtilities.hpp"
#include "QmsStrings.hpp"
#include "QmsApplicationSettings.hpp"
#include "GlobalDefinitions.hpp"

const std::pair<int, int> BoardResizeWidget::DEFAULT_ROW_MIN_MAX{ 3, 1000 };
const std::pair<int, int> BoardResizeWidget::DEFAULT_COLUMN_MIN_MAX{ 3, 1000 };
const std::pair<int, int> BoardResizeWidget::BEGINNER_GAME_DIMENSIONS{9, 9};
const std::pair<int, int> BoardResizeWidget::INTERMEDIATE_GAME_DIMENSIONS{18, 9};
const std::pair<int, int> BoardResizeWidget::ADVANCED_GAME_DIMENSIONS{18, 18};
const std::pair<int, int> BoardResizeWidget::EXTREME_GAME_DIMENSIONS{24, 24};

BoardResizeWidget::BoardResizeWidget(QWidget *parent) :
    MouseMoveableQWidget{parent},
    m_ui{new Ui::BoardResizeWidget{}},
    m_numberOfColumns{0},
    m_numberOfRows{0},
	m_minimumColumns{DEFAULT_COLUMN_MIN_MAX.first},
	m_maximumColumns{DEFAULT_COLUMN_MIN_MAX.second},
	m_minimumRows{DEFAULT_ROW_MIN_MAX.first},
	m_maximumRows{DEFAULT_ROW_MIN_MAX.second},
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
    rawString.erase(std::remove_if(rawString.begin(), rawString.end(), [](auto c) { return (!::isdigit(c) && (c != 'x') && (c != ' ')); }), rawString.end());
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
    int maybeNewColumns{stringToInt(this->m_ui->lblColumns->text().toStdString())};
    int maybeNewRows{stringToInt(this->m_ui->lblRows->text().toStdString())};
    if ((maybeNewColumns == this->m_numberOfColumns) && (maybeNewRows == this->m_numberOfRows)) {
        this->onCancelButtonClicked(false);
        return;
    }
    QMessageBox::StandardButton userReply{};
    QString questionBoxMessage{QString{QmsStrings::RESIZE_BOARD_WINDOW_CONFIRMATION}.arg(
                               QS_NUMBER(this->m_numberOfColumns),
                               QS_NUMBER(this->m_numberOfRows),
                                     QS_NUMBER(maybeNewColumns),
                                     QS_NUMBER(maybeNewRows))};
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

void BoardResizeWidget::setMaximumColumns(int maximumColumns) {
	using QmsUtilities::stringToInt;
	using QmsUtilities::toStdString;
	if (maximumColumns < this->m_minimumColumns) {
		throw std::runtime_error("BoardResizeWidget::setMaximumColumns(int): column count cannot be less than current minimum column count (" + toStdString(maximumColumns) + " < " + toStdString(this->m_minimumColumns) + ")");
	} else if (maximumColumns < 0) {
		throw std::runtime_error("BoardResizeWidget::setMaximumColumns(int): column count cannot be less than or equal to zero (" + toStdString(maximumColumns) + " <= 0");
	}
	this->m_maximumColumns = maximumColumns;
	int currentColumns{ stringToInt(this->m_ui->lblColumns->text().toStdString()) };
	if (currentColumns > maximumColumns) {
		this->m_ui->lblColumns ->setText(QS_NUMBER(this->m_maximumColumns));
	}
}

void BoardResizeWidget::setMaximumRows(int maximumRows) {
	using QmsUtilities::stringToInt;
	using QmsUtilities::toStdString;
	if (maximumRows < this->m_minimumRows) {
		throw std::runtime_error("BoardResizeWidget::setMaximumRows(int): row count cannot be less than current minimum row count (" + toStdString(maximumRows) + " < " + toStdString(this->m_minimumRows) + ")");
	} else if (maximumRows < 0) {
		throw std::runtime_error("BoardResizeWidget::setMaximumRows(int): row count cannot be less than or equal to zero (" + toStdString(maximumRows) + " <= 0");
	}
	this->m_maximumRows = maximumRows;
	int currentRows{ stringToInt(this->m_ui->lblRows->text().toStdString()) };
	if (currentRows > this->m_maximumRows) {
		this->m_ui->lblRows->setText(QS_NUMBER(this->m_maximumRows));
	}
}

void BoardResizeWidget::setMinimumColumns(int minimumColumns) {
	using QmsUtilities::stringToInt;
	using QmsUtilities::toStdString;
	if (minimumColumns > this->m_maximumColumns) {
		throw std::runtime_error("BoardResizeWidget::setMinimumColumns(int): column count cannot be greater than current maximum column count (" + toStdString(minimumColumns) + " < " + toStdString(this->m_maximumColumns) + ")");
	} else if (minimumColumns < 0) {
		throw std::runtime_error("BoardResizeWidget::setMinimumColumns(int): column count cannot be less than or equal to zero (" + toStdString(minimumColumns) + " <= 0");
	}
	this->m_minimumColumns = minimumColumns;
	int currentColumns{ stringToInt(this->m_ui->lblColumns->text().toStdString()) };
	if (currentColumns < this->m_minimumColumns) {
		this->m_ui->lblColumns->setText(QS_NUMBER(this->m_minimumColumns));
	}
}

void BoardResizeWidget::setMinimumRows(int minimumRows) {
	using QmsUtilities::stringToInt;
	using QmsUtilities::toStdString;
	if (minimumRows > this->m_maximumRows) {
		throw std::runtime_error("BoardResizeWidget::setMinimumRows(int): row count cannot be greater than current maximum row count (" + toStdString(minimumRows) + " < " + toStdString(this->m_maximumRows) + ")");
	} else if (minimumRows < 0) {
		throw std::runtime_error("BoardResizeWidget::setMinimumRows(int): row count cannot be less than or equal to zero (" + toStdString(minimumRows) + " <= 0");
	}
	this->m_minimumRows = minimumRows;
	int currentRows{ stringToInt(this->m_ui->lblRows->text().toStdString()) };
	if (currentRows < this->m_minimumRows) {
		this->m_ui->lblRows->setText(QS_NUMBER(this->m_minimumRows));
	}
}

int BoardResizeWidget::minimumRows() const {
	return this->m_minimumRows;
}

int BoardResizeWidget::minimumColumns() const {
	return this->m_minimumColumns;
}

int BoardResizeWidget::maximumRows() const {
	return this->m_maximumRows;
}

int BoardResizeWidget::maximumColumns() const {
	return this->m_maximumColumns;
}

void BoardResizeWidget::onBtnIncrementRowsClicked(bool down)
{
    using QmsUtilities::stringToInt;
    Q_UNUSED(down);
    int currentRows{stringToInt(this->m_ui->lblRows->text().toStdString())};
	if (currentRows < this->m_maximumRows) {
		this->m_ui->lblRows->setText(QS_NUMBER(currentRows + 1));
	}
}

void BoardResizeWidget::onBtnDecrementRowsClicked(bool down) {
    using QmsUtilities::stringToInt;
    Q_UNUSED(down);
    int currentRows{stringToInt(this->m_ui->lblRows->text().toStdString())};
    if (currentRows > this->m_minimumRows) {
        this->m_ui->lblRows->setText(QS_NUMBER(currentRows - 1));
    }
}


void BoardResizeWidget::onBtnIncrementColumnsClicked(bool down)
{
    using QmsUtilities::stringToInt;
    Q_UNUSED(down);
    int currentColumns{stringToInt(this->m_ui->lblColumns->text().toStdString())};
	if (currentColumns < this->m_maximumColumns) {
		this->m_ui->lblColumns->setText(QS_NUMBER(currentColumns + 1));
	}
}

void BoardResizeWidget::onBtnDecrementColumnsClicked(bool down)
{
    using QmsUtilities::stringToInt;
    Q_UNUSED(down);
    int currentColumns{stringToInt(this->m_ui->lblColumns->text().toStdString())};
    if (currentColumns > this->m_maximumColumns) {
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
