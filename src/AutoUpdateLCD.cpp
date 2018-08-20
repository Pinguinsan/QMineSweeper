#include "AutoUpdateLCD.hpp"

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