//
// Created by pinguinsan on 12/3/17.
//

#include "AutoUpdateLCD.h"

void AutoUpdateLCD::setDataSource(ChangeAwareInt *dataSource) {
    if (this->m_dataSource) {
        this->m_dataSource->removeEventListener(this);
    }
    this->m_dataSource = dataSource;
    this->m_dataSource->addEventListener(this);
    this->displayValue(this->m_dataSource->value());
}

void AutoUpdateLCD::displayValue(int value) {
    QString displayValue{QString::number(value)};
    auto digitsInValue = displayValue.length();
    QString toDisplay{""};
    if (digitsInValue > this->digitCount()) {
        this->display(std::string(static_cast<unsigned long>(this->digitCount()), 'X').c_str());
    } else {
        QString toDisplay{""};
        for (int i = digitsInValue; i <= digitCount(); i++) {
            toDisplay += '0';
        }
        toDisplay += displayValue;
        this->display(toDisplay);
    }
}

void AutoUpdateLCD::valueChanged(int value) {
    this->displayValue(value);
}

/*
 *
void MainWindow::updateNumberOfMinesLCD(int numberOfMines)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    if (numberOfMines > 999) {
        this->m_ui->minesRemaining->display(LCD_OVERFLOW_STRING);
    } else if (numberOfMines <= 0) {
        this->m_ui->minesRemaining->display(toQString(this->getLCDPadding(3)));
    } else if (numberOfMines < 10) {
        this->m_ui->minesRemaining->display(QStringFormat("%s%i", this->getLCDPadding(2).c_str(), numberOfMines));
    } else if (numberOfMines < 100) {
        this->m_ui->minesRemaining->display(QStringFormat("%s%i", this->getLCDPadding(1).c_str(), numberOfMines));
    } else {
        this->m_ui->minesRemaining->display(numberOfMines);
    }
}

void MainWindow::updateNumberOfMovesMadeLCD(int numberOfMovesMade)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    if (numberOfMovesMade > 999) {
        this->m_ui->numberOfMoves->display(LCD_OVERFLOW_STRING);
    } else if (numberOfMovesMade <= 0) {
        this->m_ui->numberOfMoves->display(toQString(this->getLCDPadding(3)));
    } else if (numberOfMovesMade < 10) {
        this->m_ui->numberOfMoves->display(QString{"%1%2"}.arg(this->getLCDPadding(2).c_str(), QS_NUMBER(numberOfMovesMade)));
    } else if (numberOfMovesMade < 100) {
        this->m_ui->numberOfMoves->display(QString{"%1%2"}.arg(this->getLCDPadding(2).c_str(), QS_NUMBER(numberOfMovesMade)));
    } else {
        this->m_ui->numberOfMoves->display(numberOfMovesMade);
    }

}

*/