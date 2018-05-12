#ifndef QMINESWEEPER_AUTOUPDATELCD_HPP
#define QMINESWEEPER_AUTOUPDATELCD_HPP

#include "ChangeAwareValue.hpp"
#include <QLCDNumber>


class AutoUpdateLCD : public ChangeAwareIntEventListener, public QLCDNumber
{
public:
    explicit AutoUpdateLCD(QWidget *parent = nullptr) :
        QLCDNumber{parent},
        m_dataSource{nullptr} {}
    void setDataSource(ChangeAwareInt *dataSource);
    inline ChangeAwareInt &dataSource() const { return *this->m_dataSource; }
protected:
    void valueChanged(int value) override;
private:
    ChangeAwareInt *m_dataSource;

    void displayValue(int value);
};


#endif //QMINESWEEPER_AUTOUPDATELCD_HPP
