#ifndef ALGORITHMIQUE_TRANSITION_H
#define ALGORITHMIQUE_TRANSITION_H

#include "State.h"

#include <string>

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
