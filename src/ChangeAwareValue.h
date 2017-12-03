#ifndef QMINESWEEPER_CHANGEAWAREVALUE_H
#define QMINESWEEPER_CHANGEAWAREVALUE_H

#include <unordered_set>

template <typename Value>
class ChangeAwareValue;

template <typename Value>
struct ChangeAwareValueEventListener {
    friend class ChangeAwareValue<Value>;
    virtual ~ChangeAwareValueEventListener() = default;
protected:
    virtual void valueChanged(Value value) = 0;
};


template <typename Value>
class ChangeAwareValue
{
    using EventListener = ChangeAwareValueEventListener<Value>;

public:
    explicit ChangeAwareValue(Value value, EventListener *initialListener = nullptr) :
        m_value{value}
    {
        if (initialListener) {
            this->addEventListener(initialListener);
        }
        if (this->m_value != Value{}) {
            this->notifyValueChanged();
        }
    }

    ChangeAwareValue(const ChangeAwareValue &rhs) = default;
    ChangeAwareValue(ChangeAwareValue &&rhs) noexcept = default;
    ChangeAwareValue &operator=(const ChangeAwareValue &rhs) = default;
    ChangeAwareValue &operator=(ChangeAwareValue &&rhs) = default;
    ~ChangeAwareValue() = default;

    void addEventListener(EventListener *eventListener) {
        this->m_eventListeners.insert(eventListener);
    }

    void removeEventListener(EventListener *eventListener) {
        this->m_eventListeners.erase(eventListener);
    }

    ChangeAwareValue &setValue(Value value) {
        bool emitNeeded{value != this->m_value};
        this->m_value = value;
        if (emitNeeded) {
            this->notifyValueChanged();
        }
        return *this;
    }

    ChangeAwareValue &operator++(Value value) {
        (void)value;
        ++this->m_value;
        this->notifyValueChanged();
        return *this;
    }

    ChangeAwareValue &operator--(Value value) {
        (void)value;
        --this->m_value;
        this->notifyValueChanged();
        return *this;
    }

    ChangeAwareValue &operator=(Value value) {
        this->setValue(value);
        return *this;
    }

    operator Value() {
        return this->m_value;
    }

    Value value() const {
        return this->m_value;
    }

private:
    Value m_value;
    std::unordered_set<ChangeAwareValueEventListener<Value> *> m_eventListeners;

    void notifyValueChanged() {
        for (auto &it : this->m_eventListeners) {
            if (it) it->valueChanged(this->m_value);
        }
    }
};

typedef ChangeAwareValue<int> ChangeAwareInt;
typedef ChangeAwareValue<double> ChangeAwareDouble;

typedef ChangeAwareValueEventListener<int> ChangeAwareIntEventListener;
typedef ChangeAwareValueEventListener<double> ChangeAwareDoubleEventListener;



#endif //QMINESWEEPER_CHANGEAWAREVALUE_H
