#include "Transition.h"

Transition::Transition(std::string p_label, State *p_stateFrom, State *p_StateTo)
    : m_label(p_label), m_stateFrom(p_stateFrom), m_stateTo(p_StateTo)
{}

State* Transition::getStateFrom() {
    return m_stateFrom;
}

void Transition::setStateFrom(State *from) {
    m_stateFrom = from;
}

State* Transition::getStateTo() {
    return m_stateTo;
}

void Transition::setStateTo(State *to) {
    m_stateTo = to;
}

std::string& Transition::getLabel() {
    return m_label;
}