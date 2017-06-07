

#include "qmsapplicationsettings.h"

QmsApplicationSettings::QmsApplicationSettings()
{

}

void QmsApplicationSettings::setNumberOfColumns(int columns)
{
    this->m_numberOfColumns = columns;
}

void QmsApplicationSettings::setNumberOfRows(int rows)
{
    this->m_numberOfRows = rows;
}

int QmsApplicationSettings::numberOfColumns() const
{
    return this->m_numberOfColumns;
}

int QmsApplicationSettings::numberOfRows() const
{
    return this->m_numberOfRows;
}
