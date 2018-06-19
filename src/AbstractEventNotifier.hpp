#ifndef QMINESWEEPER_ABSTRACTEVENTNOTIFIER_HPP
#define QMINESWEEPER_ABSTRACTEVENTNOTIFIER_HPP

#include <unordered_set>

template <typename EventListenerType>
class AbstractEventNotifier
{
public:
    inline AbstractEventNotifier &addEventListener(EventListenerType *eventListener) {
        this->m_eventListeners.insert(eventListener);
        return *this;
    }

    inline AbstractEventNotifier &removeEventListener(EventListenerType *eventListener) {
        this->m_eventListeners.erase(eventListener);
        return *this;
    }
protected:
    std::unordered_set<EventListenerType *> &eventListeners() {
        return this->m_eventListeners;
    }
private:
    std::unordered_set<EventListenerType *> m_eventListeners;
};

#endif //QMINESWEEPER_ABSTRACTEVENTNOTIFIER_HPP
