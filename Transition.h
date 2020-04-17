//
// Created by Sylvain on 31/03/2020.
//

#ifndef ALGORITHMIQUE_TRANSITION_H
#define ALGORITHMIQUE_TRANSITION_H

#include <string>

#include "State.h"

class Transition {
private:
    std::string m_label;
    State* m_stateFrom;
    State* m_stateTo;
public:
    Transition(std::string p_label, State* p_stateFrom, State* p_StateTo);
    virtual ~Transition();
    State* getStateFrom();
    State* getStateTo();
    std::string& getLabel();
};


#endif //ALGORITHMIQUE_TRANSITION_H
