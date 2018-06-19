#ifndef QMINESWEEPER_CHANGEAWAREVALUE_HPP
#define QMINESWEEPER_CHANGEAWAREVALUE_HPP

#include "AbstractEventNotifier.hpp"

template <typename ValueType>
class ChangeAwareValue;

template <typename ValueType>
struct ChangeAwareValueEventListener {
    friend class ChangeAwareValue<ValueType>;
    virtual ~ChangeAwareValueEventListener() = default;
protected:
    virtual void valueChanged(ValueType value) = 0;
};


template <typename ValueType>
class ChangeAwareValue : public AbstractEventNotifier<ChangeAwareValueEventListener<ValueType>>
{
    using EventListener = ChangeAwareValueEventListener<ValueType>;

public:
    explicit ChangeAwareValue(const ValueType &value, EventListener *initialListener = nullptr) :
        m_value{value}
    {
        if (initialListener) {
            this->addEventListener(initialListener);
        }
        if (this->m_value != ValueType{}) {
            this->notifyValueChanged();
        }
    }

    ChangeAwareValue(const ChangeAwareValue &rhs) = default;
    ChangeAwareValue(ChangeAwareValue &&rhs) noexcept = default;
    ChangeAwareValue &operator=(const ChangeAwareValue &rhs) = default;
    ChangeAwareValue &operator=(ChangeAwareValue &&rhs) noexcept = default;
    ~ChangeAwareValue() = default;

    ChangeAwareValue &setValue(const ValueType &value) {
        bool emitNeeded{value != this->m_value};
        this->m_value = value;
        if (emitNeeded) {
            this->notifyValueChanged();
        }
        return *this;
    }

    ChangeAwareValue &operator++(const ValueType &value) {
        (void)value;
        ++this->m_value;
        this->notifyValueChanged();
        return *this;
    }

    ChangeAwareValue &operator--(const ValueType &value) {
        (void)value;
        --this->m_value;
        this->notifyValueChanged();
        return *this;
    }

    ChangeAwareValue &operator=(const ValueType &value) {
        this->setValue(value);
        return *this;
    }

    operator ValueType() {
        return this->m_value;
    }

    ValueType value() const {
        return this->m_value;
    }

private:
    ValueType m_value;

    void notifyValueChanged() {
        for (auto &it : this->eventListeners()) {
            if (it) it->valueChanged(this->m_value);
        }
    }
};

typedef ChangeAwareValue<int> ChangeAwareInt;
typedef ChangeAwareValueEventListener<int> ChangeAwareIntEventListener;

typedef ChangeAwareValue<double> ChangeAwareDouble;
typedef ChangeAwareValueEventListener<double> ChangeAwareDoubleEventListener;



#endif //QMINESWEEPER_CHANGEAWAREVALUE_HPP
