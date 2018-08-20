#ifndef QMINESWEEPER_CHANGEAWAREVALUE_HPP
#define QMINESWEEPER_CHANGEAWAREVALUE_HPP

#include "AbstractEventNotifier.hpp"

template<typename ValueType>
class ChangeAwareValue;

template<typename ValueType>
struct ChangeAwareValueEventListener {
    friend class ChangeAwareValue<ValueType>;

    virtual ~ChangeAwareValueEventListener() = default;
protected:
    virtual void valueChanged(ValueType value) = 0;
};

template<typename ValueType>
class ChangeAwareValue : public AbstractEventNotifier<ChangeAwareValueEventListener<ValueType>> {
public:
    explicit ChangeAwareValue(const ValueType &value,
                              ChangeAwareValueEventListener<ValueType> *initialListener = nullptr) :
            m_value{value} {
        if (initialListener) {
            this->addEventListener(initialListener);
        }
        if (this->m_value != ValueType{}) {
            this->notifyValueChanged();
        }
    }

    ChangeAwareValue() :
            ChangeAwareValue{ValueType{}, nullptr} {

    }

    ChangeAwareValue(const ChangeAwareValue &rhs) = default;
    ChangeAwareValue(ChangeAwareValue &&rhs) noexcept = default;
    ChangeAwareValue &operator=(const ChangeAwareValue &rhs) = default;
    ChangeAwareValue &operator=(ChangeAwareValue &&rhs) noexcept = default;
    virtual ~ChangeAwareValue() = default;

    virtual ChangeAwareValue &setValue(const ValueType &value) {
        bool emitNeeded{value != this->m_value};
        this->m_value = value;
        if (emitNeeded) {
            this->notifyValueChanged();
        }
        return *this;
    }

    /*
    virtual ChangeAwareValue &operator=(const ValueType &value) {
        this->setValue(value);
        return *this;
    }
    */

    virtual operator ValueType() {
        return this->m_value;
    }

    virtual ValueType value() const {
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

template<typename ValueType>
class ChangeAwarePrimitive : public ChangeAwareValue<ValueType> {
public:
    explicit ChangeAwarePrimitive(const ValueType &value,
                                  ChangeAwareValueEventListener<ValueType> *initialListener = nullptr) :
            ChangeAwareValue<ValueType>{value, initialListener} {

    }

    ChangeAwarePrimitive(const ChangeAwarePrimitive &rhs) = default;
    ChangeAwarePrimitive(ChangeAwarePrimitive &&rhs) noexcept = default;
    ChangeAwarePrimitive &operator=(const ChangeAwarePrimitive &rhs) = default;
    ChangeAwarePrimitive &operator=(ChangeAwarePrimitive &&rhs) noexcept = default;
    virtual ~ChangeAwarePrimitive() = default;

    ChangeAwarePrimitive &operator=(const ValueType &value) {
        this->setValue(value);
        return *this;
    }

    const ChangeAwarePrimitive<ValueType> operator++(int value) {
        (void) value;
        this->setValue(this->value() + 1);
        return *this;
    }

    const ChangeAwarePrimitive<ValueType> operator--(int value) {
        (void) value;
        this->setValue(this->value() - 1);
        return *this;
    }

};

typedef ChangeAwarePrimitive<int> ChangeAwareInt;
typedef ChangeAwareValueEventListener<int> ChangeAwareIntEventListener;

typedef ChangeAwarePrimitive<double> ChangeAwareDouble;
typedef ChangeAwareValueEventListener<double> ChangeAwareDoubleEventListener;

#endif //QMINESWEEPER_CHANGEAWAREVALUE_HPP
