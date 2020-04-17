#include "Transition.h"

Transition::Transition(std::string p_label, State *p_stateFrom, State *p_StateTo)
    : m_label(p_label), m_stateFrom(p_stateFrom), m_stateTo(p_StateTo)
{}

Transition::~Transition() {
    //TODO: delete ?
}

State* Transition::getStateTo() {
    return m_stateTo;
}

State* Transition::getStateFrom() {
    return m_stateFrom;
}

std::string& Transition::getLabel() {
    return m_label;
}