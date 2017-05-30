#ifndef QMINESWEEPERSETTINGSLOADER_H
#define QMINESWEEPERSETTINGSLOADER_H

#include <QObject>

class QMineSweeperSettingsLoader : public QObject
{
    Q_OBJECT
public:
    explicit QMineSweeperSettingsLoader(QObject *parent = nullptr);

    static int DEFAULT_COLUMN_COUNT();
    static int DEFAULT_ROW_COUNT();

private:
    static const int s_DEFAULT_NUMBER_OF_COLUMNS;
    static const int s_DEFAULT_NUMBER_OF_ROWS;

public slots:
};

#endif // QMINESWEEPERSETTINGSLOADER_H
