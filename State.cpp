//
// Created by Sylvain on 31/03/2020.
//

#include "State.h"

State::State() : m_isInitial(false), m_isFinal(false)
{}

State::State(bool p_isInitial, bool p_isFinal, std::string p_name)
    : m_isInitial(p_isInitial), m_isFinal(p_isFinal), m_name(p_name)
{}

State::~State() {}

std::string & State::getName() {
    return m_name;
}

bool State::isInitial() {
    return m_isInitial;
}

bool State::isFinal() {
    return m_isFinal;
}