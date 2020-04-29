//
// Created by Sylvain on 31/03/2020.
//

#include "State.h"

State::State() : m_isInitial(false), m_isFinal(false)
{}

State::State(bool p_isInitial, bool p_isFinal, std::string p_name)
    : m_isInitial(p_isInitial), m_isFinal(p_isFinal), m_name(p_name)
{}

std::string & State::getName() {
    return m_name;
}

void State::setName(std::string name) {
    m_name = name;
}

bool State::isInitial() {
    return m_isInitial;
}

void State::setInitial(bool initial) {
    m_isInitial = initial;
}

bool State::isFinal() {
    return m_isFinal;
}

void State::setFinal(bool final) {
    m_isFinal = final;
}