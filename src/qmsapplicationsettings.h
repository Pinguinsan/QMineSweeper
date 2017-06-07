

#ifndef QMINESWEEPER_QMSAPPLICATIONSETTINGS_H
#define QMINESWEEPER_QMSAPPLICATIONSETTINGS_H


class QmsApplicationSettings
{
public:
    QmsApplicationSettings();
    int numberOfColumns() const;
    int numberOfRows() const;

    void setNumberOfColumns(int columns);
    void setNumberOfRows(int rows);

private:
    int m_numberOfColumns;
    int m_numberOfRows;
};

#endif // QMINESWEEPER_QMSAPPLICATIONSETTINGS_H
