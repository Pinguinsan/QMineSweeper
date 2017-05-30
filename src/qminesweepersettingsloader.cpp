#include "qminesweepersettingsloader.h"

const int QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS{9};
const int QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS{9};

QMineSweeperSettingsLoader::QMineSweeperSettingsLoader(QObject *parent) :
    QObject{parent}
{

}

int QMineSweeperSettingsLoader::DEFAULT_COLUMN_COUNT()
{
    return QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_COLUMNS;
}

int QMineSweeperSettingsLoader::DEFAULT_ROW_COUNT()
{
    return QMineSweeperSettingsLoader::s_DEFAULT_NUMBER_OF_ROWS;
}
