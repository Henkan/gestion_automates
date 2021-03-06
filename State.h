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

    std::string &getName();

    void setName(std::string name);

    bool isInitial();

    void setInitial(bool initial);

    bool isFinal();

    void setFinal(bool final);
};

#endif //ALGORITHMIQUE_STATE_H
