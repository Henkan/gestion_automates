//
// Created by Sylvain on 31/03/2020.
//

#ifndef ALGORITHMIQUE_STATE_H
#define ALGORITHMIQUE_STATE_H

#include <string>

class State {
private:
    bool m_isInitial;
    bool m_isFinal;
    std::string m_name;
public:
    State();
    State(bool p_isInitial, bool p_isFinal, std::string p_name);
    std::string& getName();
    virtual ~State();
};


#endif //ALGORITHMIQUE_STATE_H
