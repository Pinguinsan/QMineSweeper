#ifndef QMINESWEEPER_QMINESWEEPERAPPLICATIONSETTINGS_H
#define QMINESWEEPER_QMINESWEEPERAPPLICATIONSETTINGS_H


class QMineSweeperApplicationSettings
{
public:
    QMineSweeperApplicationSettings();
    int numberOfColumns() const;
    int numberOfRows() const;

    void setNumberOfColumns(int columns);
    void setNumberOfRows(int rows);

private:
    int m_numberOfColumns;
    int m_numberOfRows;
};

#endif // QMINESWEEPERAPPLICATIONSETTINGS_H
