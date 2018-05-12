#ifndef QMINESWEEPER_BOARDRESIZEWIDGET_HPP
#define QMINESWEEPER_BOARDRESIZEWIDGET_HPP

namespace Ui {
    class BoardResizeWidget;
}

#include "MouseMoveableQWidget.hpp"

class QDesktopWidget;
class QPushButton;

class BoardResizeWidget : public MouseMoveableQWidget
{
    Q_OBJECT
public:
    enum class ResizeWidgetExitCode {
        Accepted,
        Rejected
    };

    struct GameBoardSize {
        float mineRatio;
        int columns;
        int rows;
    };

    struct ResizeWidgetResult {
        GameBoardSize boardSize;
        ResizeWidgetExitCode userAction;
    };

    explicit BoardResizeWidget(QWidget *parent = nullptr);
    ~BoardResizeWidget() override = default;
    void show(int columns, int rows);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

    void setMaximumColumns(int maximumColumns);
    void setMaximumRows(int maximumRows);
    void setMinimumColumns(int minimumColumns);
    void setMinimumRows(int minimumRows);
    int minimumRows() const;
    int minimumColumns() const;
    int maximumRows() const;
    int maximumColumns() const;

signals:
    void aboutToClose(ResizeWidgetResult result);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void onOkayButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);
    void onBtnBeginnerPresetClicked(bool down);
    void onBtnIntermediatePresetClicked(bool down);
    void onBtnAdvancedPresetClicked(bool down);
    void onBtnExtremePresetClicked(bool down);



private:
    Ui::BoardResizeWidget *m_ui;
    int m_numberOfColumns;
    int m_numberOfRows;
	int m_minimumColumns;
	int m_maximumColumns;
	int m_minimumRows;
	int m_maximumRows;
    ResizeWidgetResult m_resultToEmit;

    void onPresetBoardSizeActionTriggered(QPushButton *pressedButton);

    void onBtnIncrementRowsClicked(bool down);
    void onBtnDecrementRowsClicked(bool down);
    void onBtnIncrementColumnsClicked(bool down);
    void onBtnDecrementColumnsClicked(bool down);

	static const std::pair<int, int> DEFAULT_COLUMN_MIN_MAX;
	static const std::pair<int, int> DEFAULT_ROW_MIN_MAX;
    static const std::pair<int, int> BEGINNER_GAME_DIMENSIONS;
    static const std::pair<int, int> INTERMEDIATE_GAME_DIMENSIONS;
    static const std::pair<int, int> ADVANCED_GAME_DIMENSIONS;
    static const std::pair<int, int> EXTREME_GAME_DIMENSIONS;

};

#endif //QMINESWEEPER_BOARDRESIZEWIDGET_HPP
