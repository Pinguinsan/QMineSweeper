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

#include "boardresizewidget.h"
#include "ui_boardresizewidget.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QString>
#include <QRect>

#include "qmsutilities.h"
#include "qmsstrings.h"
#include "qmsapplicationsettings.h"
#include "globaldefinitions.h"

#include <memory>

BoardResizeWidget::BoardResizeWidget(QWidget *parent) :
    MouseMoveableQWidget{parent},
    m_ui{new Ui::BoardResizeWidget{}},
    m_numberOfColumns{0},
    m_numberOfRows{0},
    m_resultToEmit{0, 0, BoardResizeWidget::ResizeWidgetExitCode::Rejected}
{
    this->m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    this->connect(this->m_ui->btnOkay, &QPushButton::clicked, this, &BoardResizeWidget::onOkayButtonClicked);
    this->connect(this->m_ui->btnCancel, &QPushButton::clicked, this, &BoardResizeWidget::onCancelButtonClicked);
    this->connect(this->m_ui->btnIncrementRows, &QPushButton::clicked, this, &BoardResizeWidget::onBoardResizeActionClicked);
    this->connect(this->m_ui->btnDecrementRows, &QPushButton::clicked, this, &BoardResizeWidget::onBoardResizeActionClicked);
    this->connect(this->m_ui->btnIncrementColumns, &QPushButton::clicked, this, &BoardResizeWidget::onBoardResizeActionClicked);
    this->connect(this->m_ui->btnDecrementColumns, &QPushButton::clicked, this, &BoardResizeWidget::onBoardResizeActionClicked);

    this->connect(this->m_ui->btnBeginner, &QPushButton::clicked, this, &BoardResizeWidget::onPresetBoardSizeActionTriggered);
    this->connect(this->m_ui->btnIntermediate, &QPushButton::clicked, this, &BoardResizeWidget::onPresetBoardSizeActionTriggered);
    this->connect(this->m_ui->btnAdvanced, &QPushButton::clicked, this, &BoardResizeWidget::onPresetBoardSizeActionTriggered);
    this->connect(this->m_ui->btnExtreme, &QPushButton::clicked, this, &BoardResizeWidget::onPresetBoardSizeActionTriggered);

}

BoardResizeWidget::~BoardResizeWidget()
{

}

void BoardResizeWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->m_resultToEmit.columns = 0;
    this->m_resultToEmit.rows = 0;
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

void BoardResizeWidget::onPresetBoardSizeActionTriggered(bool checked)
{
    using namespace QmsUtilities;
    Q_UNUSED(checked);
    if (QPushButton *clickedButton{dynamic_cast<QPushButton *>(QObject::sender())}) {
        std::string rawString{clickedButton->toolTip().toStdString()};
        std::transform(rawString.begin(), rawString.end(), rawString.begin(), ::tolower);
        rawString.erase(std::remove_if(rawString.begin(), rawString.end(), [](auto c) { return (!std::isdigit(c) && (c != 'x') && (c != ' ')); }), rawString.end());
        auto foundBeginningSpace = rawString.find_last_of(" ");
        auto maybeDimensions = rawString.substr(foundBeginningSpace + 1);
        auto dimensions = tryParseDimensions(maybeDimensions);
        if ((dimensions.first == -1) || (dimensions.second == -1)) {
            throw std::runtime_error(TStringFormat("In BoardResizeDialog::onBeginnerBoardSizeTriggered() : dimensions parsed from QPushButton menu item are not valid (QAction::text() = \"{0}\", this should never happen)", maybeDimensions));
        }
        this->m_ui->lblColumns->setText(QS_NUMBER(dimensions.first));
        this->m_ui->lblRows->setText(QS_NUMBER(dimensions.second));
    }

}

void BoardResizeWidget::onOkayButtonClicked(bool checked)
{
    Q_UNUSED(checked);
    using namespace QmsUtilities;
    using namespace QmsStrings;
    int maybeNewColumns{STRING_TO_INT(this->m_ui->lblColumns->text().toStdString().c_str())};
    int maybeNewRows{STRING_TO_INT(this->m_ui->lblRows->text().toStdString().c_str())};
    if ((maybeNewColumns == this->m_numberOfColumns) && (maybeNewRows == this->m_numberOfRows)) {
        this->onCancelButtonClicked(false);
        return;
    }
    QMessageBox::StandardButton userReply{};
    QString questionBoxMessage{QStringFormat("%s%i%s%i%s", RESIZE_BOARD_WINDOW_CONFIRMATION_BASE,
                                                         maybeNewColumns,
                                                         RESIZE_BOARD_WINDOW_CONFIRMATION_MIDDLE,
                                                         maybeNewRows,
                                                         RESIZE_BOARD_WINDOW_CONFIRMATION_TAIL)};
    userReply = static_cast<QMessageBox::StandardButton>(QMessageBox::question(this, START_NEW_GAME_WINDOW_TITLE, questionBoxMessage, QMessageBox::Yes|QMessageBox::No));
    if (userReply == QMessageBox::Yes) {
        this->m_resultToEmit.columns = this->m_ui->lblColumns->text().toInt();
        this->m_resultToEmit.rows = this->m_ui->lblRows->text().toInt();
        this->m_resultToEmit.userAction = BoardResizeWidget::ResizeWidgetExitCode::Accepted;
        this->close();
    } else {
        this->onCancelButtonClicked(false);
    }
}

void BoardResizeWidget::onBoardResizeActionClicked()
{
    using namespace QmsUtilities;
    if (QPushButton *pressedButton{dynamic_cast<QPushButton *>(QObject::sender())}) {
        int currentColumns{STRING_TO_INT(this->m_ui->lblColumns->text().toStdString().c_str())};
        int currentRows{STRING_TO_INT(this->m_ui->lblRows->text().toStdString().c_str())};
        if (pressedButton == this->m_ui->btnIncrementColumns) {
            this->m_ui->lblColumns->setText(QS_NUMBER(currentColumns + 1));
        } else if (pressedButton == this->m_ui->btnDecrementColumns) {
            if (currentColumns != 0) {
                this->m_ui->lblColumns->setText(QS_NUMBER(currentColumns - 1));
            }
        } else if (pressedButton == this->m_ui->btnIncrementRows) {
            this->m_ui->lblRows->setText(QS_NUMBER(currentRows + 1));
        } else if (pressedButton == this->m_ui->btnDecrementRows) {
            if (currentRows != 0) {
                this->m_ui->lblRows->setText(QS_NUMBER(currentRows - 1));
            }
        }
    }
}

void BoardResizeWidget::onCancelButtonClicked(bool checked)
{
    Q_UNUSED(checked);
    this->m_resultToEmit.columns = 0;
    this->m_resultToEmit.rows = 0;
    this->m_resultToEmit.userAction = BoardResizeWidget::ResizeWidgetExitCode::Rejected;
    this->close();
}


