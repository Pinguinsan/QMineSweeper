#include "src/qminesweeperapplicationsettings.h"

QMineSweeperApplicationSettings::QMineSweeperApplicationSettings()
{

}

void QMineSweeperApplicationSettings::setNumberOfColumns(int columns)
{
    this->m_numberOfColumns = columns;
}

void QMineSweeperApplicationSettings::setNumberOfRows(int rows)
{
    this->m_numberOfRows = rows;
}

int QMineSweeperApplicationSettings::numberOfColumns() const
{
    return this->m_numberOfColumns;
}

int QMineSweeperApplicationSettings::numberOfRows() const
{
    return this->m_numberOfRows;
}
